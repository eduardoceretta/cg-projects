/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Sep 2011
 *
 *  Calculate the AmbientOcclusion of a Scene using the voxelization of it.
 *  Create some random ray distribution around the hemisphere and passes it as a texture.
 *  Access each ray in the voxel structure and verifies if it is empty, iterate each ray.
 */
#include "KernelSSAO_Vox_RayMarch.h"
#include <cmath>
#include <limits>

#include "MathUtils/Matrix4.h"
#include "MathUtils/Vector4.h"
#include "GLUtils/GLProjectionMatrix.h"

KernelSSAO_Vox_RayMarch::KernelSSAO_Vox_RayMarch(char* path, int width, int height, 
                                                 GLuint texIdEyePos, GLuint texIdNormalDepth, 
                                                 GLuint texIdVoxelGrid, GLuint texIdGridInvFunction)
: KernelBase(path, "ssao_vox.vert", "ssao_vox.frag", width, height)
,m_width(width)
,m_height(height)
,m_numRayDirections(8)
,m_numRayHemispherDivision(8)
,m_numRaySteps(8)
,m_numRayDistribution(25)
{
  m_fbo->attachToDepthBuffer(GL_FBOBufferType::RenderBufferObject);

  createRayDirectionsTexture();

  GLTextureObject t;
  t.createTexture2D(width, height, GL_RGBA32UI_EXT, GL_RGBA_INTEGER_EXT, GL_UNSIGNED_INT);
  t.setFilters(GL_NEAREST, GL_NEAREST);
  t.setWraps(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
  //Output
  m_texIdSSAO = addOutput(KernelSSAO_Vox_RayMarch::SSAO);
  m_texIdDebug = addOutput(KernelSSAO_Vox_RayMarch::Debug, t.getId());
  
	//Input
	m_shader->setActive(true);
    addInputFloat("screenWidth", width);
    addInputFloat("screenHeight", height);
    addInputInt("rayDirectionsWidth", m_rayDirectionsWidth);
    addInputInt("numRayDistribution", m_numRayDistribution);
    addInputInt("numRayHemispherDivision",m_numRayHemispherDivision);
    addInputInt("numRayDirections", m_numRayDirections);
    addInputInt("numRaySteps", m_numRaySteps);
    
    addInputTexture(GL_TEXTURE_1D, "gridInvFunc", texIdGridInvFunction);
    addInputTexture(GL_TEXTURE_1D, "rayDirections", m_texIdRayDirections);
    addInputTexture(GL_TEXTURE_2D, "eyePos", texIdEyePos);
    addInputTexture(GL_TEXTURE_2D, "normalDepth", texIdNormalDepth);
    addInputTexture(GL_TEXTURE_2D, "voxelGrid", texIdVoxelGrid);
	m_shader->setActive(false);
}

KernelSSAO_Vox_RayMarch::~KernelSSAO_Vox_RayMarch(){

}


void KernelSSAO_Vox_RayMarch::setActive(bool op, GLProjectionMatrix *projectionMatrix)
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

void KernelSSAO_Vox_RayMarch::setActiveShaderOnly(bool op, GLProjectionMatrix *projectionMatrix)
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


void KernelSSAO_Vox_RayMarch::step( GLProjectionMatrix *projectionMatrix )
{
  float znear = projectionMatrix->getNear();
  float zfar = projectionMatrix->getFar();
  float right = projectionMatrix->getRight();
  float top = projectionMatrix->getTop();
  int perspective = (int)!(projectionMatrix->isOrthographic());

  m_fbo->setActive(true);
  if(m_shader)
  {
    m_shader->setActive(true);
    addInputFloat("near", znear);
    addInputFloat("far", zfar);
    addInputFloat("right", right);
    addInputFloat("top", top);
    addInputInt("perspective", perspective);
  }
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  renderScreenQuad();
  
  if(m_shader)
    m_shader->setActive(false);
  m_fbo->setActive(false);
}

void KernelSSAO_Vox_RayMarch::createRayDirectionsTexture()
{
  m_rayDirectionsWidth = m_numRayDistribution*m_numRayDirections*m_numRayHemispherDivision;

  GLfloat* texData = new GLfloat[m_rayDirectionsWidth*3];

  for(int k = 0; k < m_numRayDistribution; ++k)
  {
    for(int i = 0; i < m_numRayDirections; ++i)
    {
      for(int j = 0; j < m_numRayHemispherDivision; ++j)
      {
        float r = 1.0f;

        if(k > 0)
          r = (1.0f + float(rand()%10)/100 - .05f);

        float angle = ((i+float(j%2)/2)*(2.0f*PI/(m_numRayDirections)))*r;

        if(k > 0)
          r = (1.0f + float(rand()%30)/100 - .15f);
  
        float stepAngle = clamp((j+.5)*(PI/2)/(m_numRayHemispherDivision)*r, .05*PI/2, .9*PI/2);


        Vector3 dir = Vector3(sin(stepAngle)*sin(angle), cos(stepAngle), sin(stepAngle)*cos(angle));
        texData[k*m_numRayDirections*m_numRayHemispherDivision*3 + i*m_numRayHemispherDivision*3 + j*3 + 0] = dir.x;
        texData[k*m_numRayDirections*m_numRayHemispherDivision*3 + i*m_numRayHemispherDivision*3 + j*3 + 1] = dir.y;
        texData[k*m_numRayDirections*m_numRayHemispherDivision*3 + i*m_numRayHemispherDivision*3 + j*3 + 2] = dir.z;
      }
    }
  }
  
  GLTextureObject t;
  t.createTexture1D(m_rayDirectionsWidth, GL_RGB32F_ARB, GL_RGB, GL_FLOAT, texData);
  t.setFilters(GL_NEAREST, GL_NEAREST);
  t.setWraps(GL_CLAMP_TO_EDGE);
  m_texIdRayDirections = t.getId();

  delete [] texData;
}

void KernelSSAO_Vox_RayMarch::renderRayDistribution(int distribution)
{
  glPushMatrix();
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  GLTextureObject t(m_texIdRayDirections, GL_TEXTURE_1D);
  GLfloat *texData =  &t.read1DTextureFloatData(GL_RGB)[distribution*m_numRayDirections*m_numRayHemispherDivision*3];
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
  
  glColor3f(1.0f, 1.0f, 1.0f);
  glBegin(GL_LINES);
  for(int i = 0; i < m_numRayDirections; ++i)
  {
    for(int j = 0; j < m_numRayHemispherDivision; ++j)
    {
      Vector3 dir(
        texData[i*m_numRayHemispherDivision*3 + j*3 + 0],
        texData[i*m_numRayHemispherDivision*3 + j*3 + 1],
        texData[i*m_numRayHemispherDivision*3 + j*3 + 2]);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3f(dir.x, dir.y, dir.z);
      //cout << dir;
    }
  }
  glEnd();
  glPopAttrib();
  glPopMatrix();
}

GLuint KernelSSAO_Vox_RayMarch::getTexIdSSAO() const
{
  return m_texIdSSAO;
}

