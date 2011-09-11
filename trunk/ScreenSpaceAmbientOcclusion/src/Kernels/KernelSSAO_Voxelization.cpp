/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Sep 2011
 *
 *  Calculate the AmbientOcclusion of a Scene using the voxelization of it.
 *  Create some random ray distribution around the hemisphere and passes it as a texture.
 *  Access each ray in the voxel structure and verifies if it is empty, iterate each ray.
 */
#include "KernelSSAO_Voxelization.h"
#include <cmath>
#include <limits>

#include "MathUtils/Matrix4.h"
#include "MathUtils/Vector4.h"
#include "GLUtils/GLProjectionMatrix.h"

KernelSSAO_Voxelization::KernelSSAO_Voxelization(char* path, int width, int height, GLuint texIdNormal, GLuint texIdVoxelGrid)
: KernelBase(path, "ssao_vox.vert", "ssao_vox.frag", width, height)
,m_width(width)
,m_height(height)
{
  m_fbo->attachToDepthBuffer(GL_FBOBufferType::RenderBufferObject);

  createRayDirectionsTexture();


  //Output
  m_texIdSSAO = addOutput(0);
  
	//Input
	m_shader->setActive(true);
    addInputFloat("screenWidth", width);
    addInputFloat("screenHeight", height);
    addInputFloat("rayDirectionsWidth", (float)m_rayDirectionsWidth);
    
    addInputTexture(GL_TEXTURE_1D, "rayDirections", m_texIdRayDirections);
    addInputTexture(GL_TEXTURE_2D, "normalTex", texIdNormal);
    addInputTexture(GL_TEXTURE_2D, "voxelGrid", texIdVoxelGrid);
	m_shader->setActive(false);
}

KernelSSAO_Voxelization::~KernelSSAO_Voxelization(){

}
//
//void KernelSSAO_Voxelization::setActive(bool op)
//{
//  m_BBCalculated = false;
//  if(op)
//  {
//    m_voxData = NULL;
//    m_eyeNearestData = NULL;
//    glGetFloatv(GL_MODELVIEW_MATRIX, m_modelviewMatrix);
//    m_projectionMatrix.readGLProjection();
//    m_near = m_projectionMatrix.getNear();
//    m_far = m_projectionMatrix.getFar();
//    m_right = m_projectionMatrix.getRight();
//    m_top = m_projectionMatrix.getTop();
//
//    m_shader->setActive(true);
//    addInputFloat("near", m_near);
//    addInputFloat("far", m_far);
//    addInputFloat("right", m_right);
//    addInputFloat("top", m_top);
//    m_shader->setActive(false);
//    glPushAttrib(GL_ALL_ATTRIB_BITS);
//    
//  }else
//  {
//    glPopAttrib();
//  }
//
//  KernelBase::setActive(op);
//
//  if(op)
//  {
//    glClearColor(0,0,0,0);
//    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
//    //glEnableIndexedEXT(GL_BLEND, 0);
//
//    glEnable(GL_DEPTH_CLAMP_NV);//ONLY WORKS ON NVIDIA!!!!!!!!!!!
//    glEnable(GL_COLOR_LOGIC_OP);
//    glDisable(GL_DEPTH_TEST);
//    glLogicOp(GL_XOR);
//  }
//}
//
//void KernelSSAO_Voxelization::setActiveShaderOnly( bool op)
//{
//  m_BBCalculated = false;
//  if(op)
//  {
//    glGetFloatv(GL_MODELVIEW_MATRIX, m_modelviewMatrix);
//    m_projectionMatrix.readGLProjection();
//    m_near = m_projectionMatrix.getNear();
//    m_far = m_projectionMatrix.getFar();
//    m_right = m_projectionMatrix.getRight();
//    m_top = m_projectionMatrix.getTop();
//
//    m_shader->setActive(true);
//    addInputFloat("near", m_near);
//    addInputFloat("far", m_far);
//    addInputFloat("right", m_right);
//    addInputFloat("top", m_top);
//    m_shader->setActive(false);
//    glPushAttrib(GL_ALL_ATTRIB_BITS);
//  }else
//  {
//    glPopAttrib();
//  }
//
//  KernelBase::setActiveShaderOnly(op);
//
//  if(op)
//  {
//    glClearColor(0,0,0,0);
//    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
//    //glEnableIndexedEXT(GL_BLEND, 0);
//
//    glEnable(GL_DEPTH_CLAMP_NV);//ONLY WORKS ON NVIDIA!!!!!!!!!!!
//    glEnable(GL_COLOR_LOGIC_OP);
//    glDisable(GL_DEPTH_TEST);
//    glLogicOp(GL_XOR);
//  }
//}


void KernelSSAO_Voxelization::createRayDirectionsTexture()
{
  m_numRayDirections = 8;
  m_numRayHemispherDivision = 4;
  m_numRayDistribution = 5;
  m_rayDirectionsWidth = m_numRayDistribution*m_numRayDirections*m_numRayHemispherDivision;

  GLfloat* texData = new GLfloat[m_rayDirectionsWidth*3];

  for(int k = 0; k < m_numRayDistribution; ++k)
  {
    for(int i = 0; i < m_numRayDirections; ++i)
    {
      for(int j = 0; j < m_numRayHemispherDivision; ++j)
      {
        float r = (1.0f + float(rand()%30)/100 - .15f);
        float angle = DEG_TO_RAD(i*(360.0f/(m_numRayDirections-1)))*r;

        r = (1.0f + float(rand()%50)/100 - .25f);
        float stepAngle = clamp(DEG_TO_RAD(j*90.0f/(m_numRayHemispherDivision - 1))*r, 0.0, PI/2);
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


//void KernelSSAO_Voxelization::reloadGridFuncTextures(float power)
//{
//  createGridFuncTextures(power);
//  m_shader->setActive(true);
//    addInputTexture(GL_TEXTURE_1D, "gridFunction", m_texIdGridFunc);
//    addInputTexture(GL_TEXTURE_1D, "gridInvFunction", m_texIdGridInvFunc);
//  m_shader->setActive(false);
//}
//
//Vector3 KernelSSAO_Voxelization::getGridCellCenter(int x, int y, int z, float zNear)
//{
//  float xm; 
//  float ym;
//
//  if(m_projectionMatrix.isOrthographic())
//  {
//    xm = (2.0f*m_top)/m_width; 
//    ym = (2.0f*m_right)/m_height;
//  }else
//  {
//    xm = (2.0f*m_top*(m_near+(m_far - m_near)/2)/m_near)/m_width; //Half Frustum Width
//    ym = (2.0f*m_top*(m_near+(m_far - m_near)/2)/m_near)/m_height;//Half Frustum Height 
//  }
//
//  float xe = xm*float(x + .5);
//  float ye = ym*float(y + .5);
//
//#ifdef NEAREST_EYE
//  float zm = (m_far - 0.0);
//  float ze = m_funcData[(int)floor((float(z + 0.5)/(m_gridBitMapHeight*m_gridBitMapWidth))*(m_funcTexSize - 1) + .5f)]*zm + zNear ;
//#else
//  float zm = (m_far);
//  float ze = m_funcData[(int)floor((float(z + 0.5)/(m_gridBitMapHeight*m_gridBitMapWidth))*(m_funcTexSize - 1) + .5f)]*zm;
//#endif //NEARST_EYE
//
//  return Vector3(xe, ye, -ze);
//}
//
//Vector3 KernelSSAO_Voxelization::getGridCellSize(int x, int y, int z, float zNear)
//{
//  float xm; 
//  float ym;
//
//  if(m_projectionMatrix.isOrthographic())
//  {
//    xm = (2.0f*m_top)/m_width; 
//    ym = (2.0f*m_right)/m_height;
//  }else
//  {
//    xm = (2.0f*m_top*(m_near+(m_far - m_near)/2)/m_near)/m_width; //Half Frustum Width
//    ym = (2.0f*m_top*(m_near+(m_far - m_near)/2)/m_near)/m_height;//Half Frustum Height 
//  }
//
//#ifdef NEAREST_EYE
//  float zm = (m_far - 0.0);
//#else
//  float zm = (m_far);
//#endif //NEARST_EYE
//
//  return Vector3(xm, ym, zm*(
//    m_funcData[(int)floor((float(z + 1.0f)/(m_gridBitMapHeight*m_gridBitMapWidth))*(m_funcTexSize - 1) + .5f)]-
//    m_funcData[(int)floor((float(z + 0.0f)/(m_gridBitMapHeight*m_gridBitMapWidth))*(m_funcTexSize - 1) + .5f)]
//  ));
//}
