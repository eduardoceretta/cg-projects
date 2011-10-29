/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Oct 2011
 *
 *  Calculate the AmbientOcclusion of a Scene using the voxelization of it.
 *  Make Cone Tracing to calculate the occlusion. Each Cone Integral is achieved 
 *  by calculating n sphere integrals.
 */
#include "KernelSSAO_Voxelization_Cone.h"
#include <cmath>
#include <limits>

#include "MathUtils/Matrix4.h"
#include "MathUtils/Vector4.h"
#include "MathUtils/Bit.h"
#include "GLUtils/GLProjectionMatrix.h"
#include "MathUtils/UniformPoissonDiskSampler.h"

#define ARITPROG_SUM(a, d, n) int((n)*((a)+ARITPROG_AN(a,d,n))/2)
#define ARITPROG_AN(a, d, n) int((a) + floor((n-1.0)*(d) + .5))

#define func(i, numSpheresByCone) \
  (pow(((float(i) + 2.0)/(float(numSpheresByCone) + 1.0)),3.0))

#define func2(i, numSpheresByCone) \
  (pow(((float(i) + 1.0)/(float(numSpheresByCone) + 1.0)),1.5))

#define getSphereCenter(eyePosition, coneDir, i, rfar, numSpheresByCone)  \
  ((eyePosition) + (coneDir)*func(i, numSpheresByCone)*(rfar*.8))

#define getSphereRadius(i, rfar, numSpheresByCone, numCones) \
  func2(i, numSpheresByCone)*(rfar*.8)*sqrt(2.0/float(numCones)) 


KernelSSAO_Voxelization_Cone::KernelSSAO_Voxelization_Cone(char* path, int width, int height, 
                                                 GLuint texIdEyePos, GLuint texIdNormalDepth, 
                                                 GLuint texIdVoxelGrid, GLuint texIdGridInvFunction)
: KernelBase(path, "ssao_vox_cone.vert", "ssao_vox_cone.frag", width, height)
,m_width(width)
,m_height(height)
,m_numSamplersDistributions(5)
,m_coneDirSamplersWidth(0)
,m_bitCount16Height(0)
,m_bitCount16Width(0)
,m_texIdBitCount16(0)
,m_numCones(6) //HINT
,m_numSpheresByCone(3)
,m_numSamplersAlpha(0)
,m_numSamplersBeta(0)
,m_sphereSamplers(NULL)
{
  m_fbo->attachToDepthBuffer(GL_FBOBufferType::RenderBufferObject);

  createConeSamplerTexture();
  createSphereSamplerTexture();
  createBitCount16Texture();

  GLTextureObject t;
  t.createTexture2D(width, height, GL_RGBA32UI_EXT, GL_RGBA_INTEGER_EXT, GL_UNSIGNED_INT);
  t.setFilters(GL_NEAREST, GL_NEAREST);
  t.setWraps(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
  //Output
  m_texIdSSAO = addOutput(KernelSSAO_Voxelization_Cone::SSAO);
  m_texIdDebug = addOutput(KernelSSAO_Voxelization_Cone::Debug, t.getId());
  
	//Input
	m_shader->setActive(true);
    addInputFloat("screenWidth", width);
    addInputFloat("screenHeight", height);
    addInputInt("numCones", m_numCones);
    addInputInt("numSpheresByCone", m_numSpheresByCone);
    addInputInt("numSamplersDistributions", m_numSamplersDistributions);
    addInputInt("coneDirSamplersWidth", m_coneDirSamplersWidth);
    addInputInt("bitCount16Height", m_bitCount16Height);
    addInputInt("bitCount16Width", m_bitCount16Width);
    addInputInt("sphereSamplersWidth", m_sphereSamplersWidth);
    
    addInputVec3Array("sphereSamplersArray", m_sphereSamplers, m_sphereSamplersWidth);

    addInputTexture(GL_TEXTURE_1D, "sphereSamplers", m_texIdSphereSamplers);
    addInputTexture(GL_TEXTURE_1D, "coneDirSamplers", m_texIdConeDirSamplers);
    addInputTexture(GL_TEXTURE_1D, "gridInvFunc", texIdGridInvFunction);
    addInputTexture(GL_TEXTURE_2D, "bitCount16", m_texIdBitCount16);
    addInputTexture(GL_TEXTURE_2D, "eyePos", texIdEyePos);
    addInputTexture(GL_TEXTURE_2D, "normalDepth", texIdNormalDepth);
    addInputTexture(GL_TEXTURE_2D, "voxelGrid", texIdVoxelGrid);
    addInputTexture(GL_TEXTURE_2D, "voxelGridDEBUG", texIdVoxelGrid);
    
	m_shader->setActive(false);
  m_pointLight.setAmbientColor(Color(.2, .2, .2));
  m_pointLight.setDiffuseColor(Color(.5, .5, .5));
  m_pointLight.setSpecularColor(Color(1,1,1));
  m_pointLight.setPosition(Vector3(0,2,0));
  m_pointLight.setRenderSphereRadius(.2);
  m_pointLight.setRenderSphereEnabled(true);
}

KernelSSAO_Voxelization_Cone::~KernelSSAO_Voxelization_Cone(){
  if(m_sphereSamplers)
    delete[] m_sphereSamplers;
}


void KernelSSAO_Voxelization_Cone::setActive(bool op, GLProjectionMatrix *projectionMatrix)
{
  if(op)
  {
    float znear = projectionMatrix->getNear();
    float zfar = projectionMatrix->getFar();
    float right = projectionMatrix->getRight();
    float top = projectionMatrix->getTop();
    int perspective = (int)!(projectionMatrix->isOrthographic());

    m_shader->setActive(true);
      addInputFloat("near", znear);
      addInputFloat("far", zfar);
      addInputFloat("right", right);
      addInputFloat("top", top);
      addInputInt("perspective", perspective);
    m_shader->setActive(false);
  }
  KernelBase::setActive(op);
}

void KernelSSAO_Voxelization_Cone::setActiveShaderOnly(bool op, GLProjectionMatrix *projectionMatrix)
{
  if(op)
  {
    float znear = projectionMatrix->getNear();
    float zfar = projectionMatrix->getFar();
    float right = projectionMatrix->getRight();
    float top = projectionMatrix->getTop();
    int perspective = (int)!(projectionMatrix->isOrthographic());

    m_shader->setActive(true);
    addInputFloat("near", znear);
    addInputFloat("far", zfar);
    addInputFloat("right", right);
    addInputFloat("top", top);
    addInputInt("perspective", perspective);
    m_shader->setActive(false);
  }

  KernelBase::setActiveShaderOnly(op);
}


void KernelSSAO_Voxelization_Cone::step( GLProjectionMatrix *projectionMatrix )
{
  float znear = projectionMatrix->getNear();
  float zfar = projectionMatrix->getFar();
  float right = projectionMatrix->getRight();
  float top = projectionMatrix->getTop();
  int perspective = (int)!(projectionMatrix->isOrthographic());
  GLfloat invProjGL[16];
  for(int i = 0; i < 4; ++i)
    for(int j = 0; j < 4; ++j)
      invProjGL[i*4+j] = projectionMatrix->getValue(i, j);

  Matrix4 invProj = Matrix4(invProjGL);
  //invProj.Invert();
  //invProj.Transpose();
  for(int i = 0; i < 4; ++i)
    for(int j = 0; j < 4; ++j)
      invProjGL[i*4+j] = invProj.getValue(i, j);

  m_fbo->setActive(true);
  if(m_shader)
  {
    m_shader->setActive(true);
    addInputFloat("near", znear);
    addInputFloat("far", zfar);
    addInputFloat("right", right);
    addInputFloat("top", top);
    addInputInt("perspective", perspective);
    addInputMatrix4("projInv", invProjGL);
  }
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  renderScreenQuad();
  
  if(m_shader)
    m_shader->setActive(false);
  m_fbo->setActive(false);
}


GLuint KernelSSAO_Voxelization_Cone::getTexIdSSAO() const
{
  return m_texIdSSAO;
}

void KernelSSAO_Voxelization_Cone::createConeSamplerTexture()
{
  m_numSamplersAlpha = ceil(sqrt(float(m_numCones)));
  m_numSamplersBeta = ceil(float(m_numCones)/m_numSamplersAlpha);
  m_numCones = m_numSamplersAlpha*m_numSamplersBeta;

  m_coneDirSamplersWidth = m_numSamplersDistributions * m_numSamplersAlpha * m_numSamplersBeta;
  GLfloat* texData = new GLfloat[m_coneDirSamplersWidth*3];
  
  for(int k = 0; k < m_numSamplersDistributions; ++k)
  {
    float soma = 0;
    GLfloat *d = &texData[k*m_numSamplersBeta*m_numSamplersAlpha*3];
    for(int i = m_numSamplersAlpha-1; i >= 0; i--)
    {
      float decPar = m_numSamplersAlpha%2==0 && (m_numSamplersAlpha/2) - i <=0;
      float step = 1.0f/(1*m_numSamplersAlpha);
      float value = 1 + ((m_numSamplersAlpha/2) - i - decPar)*step;
      //printf("%d %d %f %f\n", i,m_numSamplersAlpha,  value, (i > 0)? floor(m_numSamplersBeta*value): m_numSamplersBeta*m_numSamplersAlpha - soma );
      int betaSize = int((i > 0)? floor(m_numSamplersBeta*value): m_numSamplersBeta*m_numSamplersAlpha - soma );
      soma += float(betaSize);
      for(int j = 0; j < betaSize; ++j)
      {
        float r = 1.0f;

        if(k > 0)
          r = (1.0f + float(rand()%10)/100 - .15f);

        float  angleBeta=((m_numSamplersAlpha-i-1)*((.9*PI/2)/(m_numSamplersAlpha-1))-.1*PI/2)*r;

        if(k > 0)
          r = (1.0f + float(rand()%30)/100 - .15f);

        float  angleAlpha = ((j)*(2*PI)/(betaSize) + ((i%2))*(2*PI)/(2*betaSize))*r ;

        Vector3 dir = Vector3(sin(angleBeta)*sin(angleAlpha), cos(angleBeta), sin(angleBeta)*cos(angleAlpha));
        d[int(soma-betaSize)*3 + j*3 + 0] = dir.x;
        d[int(soma-betaSize)*3 + j*3 + 1] = dir.y;
        d[int(soma-betaSize)*3 + j*3 + 2] = dir.z;
      }
    }
    //printf("%f %d %f!!!!!!!!!!!\n\n", soma, m_numSamplersBeta*m_numSamplersAlpha, soma/(m_numSamplersBeta*m_numSamplersAlpha));
  }
  
  GLTextureObject t;
  t.createTexture1D(m_coneDirSamplersWidth, GL_RGB32F_ARB, GL_RGB, GL_FLOAT, texData);
  t.setFilters(GL_NEAREST, GL_NEAREST);
  t.setWraps(GL_CLAMP_TO_EDGE);
  m_texIdConeDirSamplers = t.getId();

  delete [] texData;
}






void KernelSSAO_Voxelization_Cone::renderSamplerDistribution(int distribution)
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glPushMatrix();

  GLTextureObject t(m_texIdConeDirSamplers, GL_TEXTURE_1D);
  GLfloat *texData =  &t.read1DTextureFloatData(GL_RGB)[distribution*m_numSamplersAlpha*m_numSamplersBeta*3];
  glPushMatrix();
    glScalef(2.0f,1.0f,2.0f);
    glTranslatef(0.0f, .5f, 0.0f);
    glutWireCube(1);
    glCullFace(GL_FRONT);
    glColor3f(.60f,0.2f,0.31f);
    glEnable(GL_CULL_FACE);
    glutSolidCube(1);
    glDisable(GL_CULL_FACE);
  glPopMatrix();
  
  m_pointLight.setPosition(Vector3(0,2,0));
  m_pointLight.configure();
  static float f = 0;
  glPushMatrix();
    glRotated(f+=3, 0,1,0);
    glTranslated(2,0,0);
    m_pointLight.render();
  glPopMatrix();
  

  glEnable(GL_NORMALIZE);
  for(int i = 0; i < m_numSamplersAlpha; ++i)
  {
    for(int j = 0; j < m_numSamplersBeta; ++j)
    {
      glPushMatrix();
        Vector3 dir(
          texData[i*m_numSamplersBeta*3 + j*3 + 0],
          texData[i*m_numSamplersBeta*3 + j*3 + 1],
          texData[i*m_numSamplersBeta*3 + j*3 + 2]);
        dir = dir.unitary();
        float angle = dir.angle(Vector3(0,1,0));
        Vector3 axis = dir^Vector3(0,1,0);
        if(!(axis == Vector3(0,0,0)))
        {
          axis = axis.unitary();
          glRotated(-angle, axis.x, axis.y, axis.z);
        }

        //SPHERES
        //SPHERES
        //SPHERES
        GLfloat color2[4] = {0,1,1,1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  color2);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  color2);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color2);

        for(int i = 0; i < m_numSpheresByCone; ++i)
        {
          glPushMatrix();
          float radius = getSphereRadius(i, 1, m_numSpheresByCone,  m_numCones) ;
          Vector3 center = getSphereCenter(Vector3(0,0,0), Vector3(0,1,0), i, 1, m_numSpheresByCone);
          glTranslatef(center.x, center.y, center.z);
          glutSolidSphere(radius, 30, 30);
          glPopMatrix();
        }

        //CONES
        //CONES
        //CONES
        glRotated(90,1,0,0);
        glTranslatef(0,0,-1);
        float coneHeight = 1.0f;
        float coneAngle = atan(sqrt(2.0f/(m_numCones)));
        float coneRadius = coneHeight*tan(coneAngle);

        GLfloat color[4] = {1,1,0,1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  color);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  color);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);

        glutWireCone(coneRadius, coneHeight, 50, 50);
      glPopMatrix();

    }
  }



 /* 
 glPushMatrix();
    float coneHeight = 1.0f;
    float coneAngle = atan(sqrt(2.0f/(m_numCones)));
    float coneRadius = coneHeight*tan(coneAngle);
    glRotated(90,1,0,0);
    glTranslatef(0,0,-1);
    glutWireCone(coneRadius, coneHeight, 50, 50);
  glPopMatrix();

  glPushAttrib(GL_LIGHTING_BIT);
  GLfloat color2[4] = {0,1,1,1};
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  color2);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  color2);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color2);

  for(int i = 0; i < m_numSpheresByCone; ++i)
  {
    glPushMatrix();
    float radius = getSphereRadius(i, 1, m_numSpheresByCone,  m_numCones) ;
    Vector3 center = getSphereCenter(Vector3(0,0,0), Vector3(0,1,0), i, 1, m_numSpheresByCone);
    glTranslatef(center.x, center.y, center.z);
    glutSolidSphere(radius, 30, 30);
    glPopMatrix();
  }
  glPopAttrib();
  /**/
  glPopMatrix();
  glPopAttrib();
}


void KernelSSAO_Voxelization_Cone::createBitCount16Texture()
{
  int numOfValues = (int)pow(2.0f, 16);

  GLint max_tex_size;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);

  m_bitCount16Width =  numOfValues > max_tex_size ? max_tex_size : numOfValues;
  m_bitCount16Height = numOfValues/max_tex_size + 1;

  GLubyte* texData = new GLubyte[m_bitCount16Width*m_bitCount16Height];
  
  
  for(int i = 0; i < numOfValues; ++i)
    texData[i] = (GLubyte)(unsigned char)Bit::numberOfSetBits(i);

  GLTextureObject t;
  t.createTexture2D(m_bitCount16Width, m_bitCount16Height, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE, texData);
  t.setFilters(GL_NEAREST, GL_NEAREST);
  t.setWraps(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
  m_texIdBitCount16 = t.getId();

  delete[] texData;
}

void KernelSSAO_Voxelization_Cone::createSphereSamplerTexture()
{
  UniformPoissonDiskSampler u;
  
  int prog_a = 4;
  float progStep = 2.3;
  int aritProgSum = ARITPROG_SUM(prog_a, progStep, m_numSpheresByCone);
 
  m_sphereSamplersWidth = aritProgSum;
  //GLfloat* texData = new GLfloat[m_sphereSamplersWidth*3];
  m_sphereSamplers = new GLfloat[m_sphereSamplersWidth*3];

  for(int k = 0; k < m_numSpheresByCone; ++k)
  {
    int numSphereSamplers = ARITPROG_AN(prog_a, progStep, k+1);

    vector<Vector3> v = u.sampleCircle(Vector3(0,0,0), 1.0f, sqrt(1.0f/numSphereSamplers)*1.35f);
    while(v.size() > numSphereSamplers)
    {
      int k = rand()%v.size();
      v.erase(v.begin()+k);
    }
    assert(v.size() == numSphereSamplers);

    int aritProgSum = ARITPROG_SUM(prog_a, progStep, k);

    GLfloat *d = &m_sphereSamplers[aritProgSum*3];

    for(int i = 0; i < numSphereSamplers; ++i)
    {
      d[i*3 + 0] = v[i].x;
      d[i*3 + 1] = v[i].y;
      d[i*3 + 2] = v[i].z;
    }
  }


  GLTextureObject t;
  t.createTexture1D(m_sphereSamplersWidth, GL_RGB32F_ARB, GL_RGB, GL_FLOAT, m_sphereSamplers);
  t.setFilters(GL_NEAREST, GL_NEAREST);
  t.setWraps(GL_CLAMP_TO_EDGE);
  m_texIdSphereSamplers = t.getId();

  //delete [] texData;
}
