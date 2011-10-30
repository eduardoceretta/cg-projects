/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Sep 2011
 *
 *  Calculate the AmbientOcclusion of a Scene using the voxelization of it.
 *  Verifies the number of full voxels inside the influence sphere.
 */
#include "KernelSSAO_Voxelization_Volume.h"
#include <cmath>
#include <limits>

#include "MathUtils/Matrix4.h"
#include "MathUtils/Vector4.h"
#include "MathUtils/Bit.h"
#include "GLUtils/GLProjectionMatrix.h"
#include "MathUtils/UniformPoissonDiskSampler.h"

KernelSSAO_Voxelization_Volume::KernelSSAO_Voxelization_Volume(char* path, int width, int height, 
                                                 GLuint texIdEyePos, GLuint texIdNormalDepth, 
                                                 GLuint texIdVoxelGrid, GLuint texIdGridInvFunction)
: KernelBase(path, "ssao_vox_vol.vert", "ssao_vox_vol.frag", width, height)
,m_width(width)
,m_height(height)
,m_numSamplers(16)
,m_numSamplersDistributions(5)
,m_samplersWidth(0)
,m_bitCount16Height(0)
,m_bitCount16Width(0)
{
  m_fbo->attachToDepthBuffer(GL_FBOBufferType::RenderBufferObject);

  createSamplerTexture();
  createBitCount16Texture();

  GLTextureObject t;
  t.createTexture2D(width, height, GL_RGBA32UI_EXT, GL_RGBA_INTEGER_EXT, GL_UNSIGNED_INT);
  t.setFilters(GL_NEAREST, GL_NEAREST);
  t.setWraps(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
  //Output
  m_texIdSSAO = addOutput(KernelSSAO_Voxelization_Volume::SSAO);
  m_texIdDebug = addOutput(KernelSSAO_Voxelization_Volume::Debug, t.getId());
  
	//Input
	m_shader->setActive(true);
    addInputFloat("screenWidth", width);
    addInputFloat("screenHeight", height);
    addInputInt("numSamplers", m_numSamplers);
    addInputInt("numSamplersDistributions", m_numSamplersDistributions);
    addInputInt("samplersWidth", m_samplersWidth);
    addInputInt("bitCount16Height", m_bitCount16Height);
    addInputInt("bitCount16Width", m_bitCount16Width);

    addInputTexture(GL_TEXTURE_1D, "samplers", m_texIdSamplers);
    addInputTexture(GL_TEXTURE_1D, "gridInvFunc", texIdGridInvFunction);
    addInputTexture(GL_TEXTURE_2D, "bitCount16", m_texIdBitCount16);
    addInputTexture(GL_TEXTURE_2D, "eyePos", texIdEyePos);
    addInputTexture(GL_TEXTURE_2D, "normalDepth", texIdNormalDepth);
    addInputTexture(GL_TEXTURE_2D, "voxelGrid", texIdVoxelGrid);
    addInputTexture(GL_TEXTURE_2D, "voxelGridDEBUG", texIdVoxelGrid);
    
	m_shader->setActive(false);
}

KernelSSAO_Voxelization_Volume::~KernelSSAO_Voxelization_Volume(){

}


void KernelSSAO_Voxelization_Volume::setActive(bool op, GLProjectionMatrix *projectionMatrix)
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

void KernelSSAO_Voxelization_Volume::setActiveShaderOnly(bool op, GLProjectionMatrix *projectionMatrix)
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


void KernelSSAO_Voxelization_Volume::step( GLProjectionMatrix *projectionMatrix )
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


GLuint KernelSSAO_Voxelization_Volume::getTexIdSSAO() const
{
  return m_texIdSSAO;
}



void KernelSSAO_Voxelization_Volume::createSamplerTexture()
{
  UniformPoissonDiskSampler u;
  
  m_samplersWidth = m_numSamplersDistributions*m_numSamplers;
  GLfloat* texData = new GLfloat[m_samplersWidth*3];

  for(int k = 0; k < m_numSamplersDistributions; ++k)
  {
    vector<Vector3> v = u.sampleCircle(Vector3(0,0,0), 1.0f, sqrt(1.0f/m_numSamplers)*1.35f);
    while(v.size() > m_numSamplers)
    {
      int k = rand()%v.size();
      v.erase(v.begin()+k);
    }
    assert(v.size() == m_numSamplers);

    GLfloat *d = &texData[k*m_numSamplers*3];
    for(int i = 0; i < m_numSamplers; ++i)
    {
      d[i*3 + 0] = v[i].x;
      d[i*3 + 1] = v[i].y;
      d[i*3 + 2] = v[i].z;
    }

  }


  GLTextureObject t;
  t.createTexture1D(m_samplersWidth, GL_RGB32F_ARB, GL_RGB, GL_FLOAT, texData);
  t.setFilters(GL_NEAREST, GL_NEAREST);
  t.setWraps(GL_CLAMP_TO_EDGE);
  m_texIdSamplers = t.getId();

  delete [] texData;
}

void KernelSSAO_Voxelization_Volume::renderSamplerDistribution(int distribution)
{
  glPushMatrix();
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  GLTextureObject t(m_texIdSamplers, GL_TEXTURE_1D);
  GLfloat *texData =  &t.read1DTextureFloatData(GL_RGB)[distribution*m_numSamplers*3];
  glPushMatrix();
  glScalef(2.0f,2.0f, 0.000000001f);
  //glTranslatef(0.0f, .5f, 0.0f);
  glutWireCube(1);
  glPopMatrix();
  glPointSize(3);
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_POINTS);



  for(int i = 0; i < m_numSamplers; ++i)
  {
    Vector3 pos(
    texData[i*3 + 0],
    texData[i*3 + 1],
    texData[i*3 + 2]);
    glVertex3f(pos.x, pos.y, pos.z);
  }

  glEnd();
  glPopAttrib();
  glPopMatrix();
}

void KernelSSAO_Voxelization_Volume::createBitCount16Texture()
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