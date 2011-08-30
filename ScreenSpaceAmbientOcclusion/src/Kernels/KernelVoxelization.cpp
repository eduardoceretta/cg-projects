/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Aug 2011
 *
 *  Voxelizes the scene in one pass using the gpu.
 *  Create a non regular grid and voxelizes the scene into it. Uses blending to make a bitmask that defines the grid.
 *  Based on: Elmar Eisemann, Xavier Decoret - Single-Pass GPU Solid Voxelization for Real-Time Applications - 2008
 */

#include "KernelVoxelization.h"
#include "GLUtils/GLTextureObject.h"
#include <cmath>

#include "MathUtils/Matrix4.h"
#include "MathUtils/Vector4.h"

#define UINT_BIT_SIZE 32

KernelVoxelization::KernelVoxelization(char* path, int width, int height, int size, GLuint gridFuncTexId)
: KernelBase(path, "voxelization.vert", "voxelization.frag", width, height)
,m_width(width)
,m_height(height)
,m_depth(size*UINT_BIT_SIZE)
{
  m_fbo->attachToDepthBuffer(GL_FBOBufferType::RenderBufferObject);

  createGridBitMapTexture();
  createGridFuncTexture();

  GLTextureObject t;
  t.createTexture2D(width, height, GL_RGBA32UI_EXT, GL_RGBA_INTEGER_EXT, GL_UNSIGNED_INT);
  t.setFilters(GL_NEAREST, GL_NEAREST);
  t.setWraps(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
  
  GLTextureObject t1;
  t1.createTexture2D(width, height, GL_RGBA32UI_EXT, GL_RGBA_INTEGER_EXT, GL_UNSIGNED_INT);
  t1.setFilters(GL_NEAREST, GL_NEAREST);
  t1.setWraps(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
  
  GLTextureObject t2;
  t2.createTexture2D(width, height, GL_RGBA32UI_EXT, GL_RGBA_INTEGER_EXT, GL_UNSIGNED_INT);
  t2.setFilters(GL_NEAREST, GL_NEAREST);
  t2.setWraps(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
  
  //Output
  m_texIdNormal = addOutput(0, t.getId());
  m_texIdColor = addOutput(1, t1.getId());
  m_texIdGrid0 = addOutput(2);//, t2.getId());
	
	//Input
	m_shader->setActive(true);
    addInputFloat("screenWidth", width);
    addInputFloat("screenHeight", height);
    addInputFloat("gridBitMapWidth", (float)m_gridBitMapWidth);
    addInputFloat("gridBitMapHeight", (float)m_gridBitMapHeight);
    addInputFloat("gridFuncWidth", (float)m_gridFuncWidth);
    addInputTexture(GL_TEXTURE_2D, "gridBitMap", m_texIdGridBitMap);
    addInputTexture(GL_TEXTURE_1D, "gridFunction", m_texIdGridFunc);
	m_shader->setActive(false);
}

KernelVoxelization::~KernelVoxelization(){

}



GLuint KernelVoxelization::getTexIdNormal(int index) const
{
  return m_texIdNormal;
}

GLuint KernelVoxelization::getTexIdColor(int index) const
{
  return m_texIdColor;
}

GLuint KernelVoxelization::getTexIdGrid0(int index) const
{
  return m_texIdGrid0;
}

void KernelVoxelization::setActive(bool op, GLfloat *projectionMatrix)
{
  if(op && projectionMatrix)
  {
    float x = projectionMatrix[0*4+0];
    float y = projectionMatrix[1*4+1];
    float z = projectionMatrix[2*4+2];
    float w = projectionMatrix[3*4+2];
    float z_near = w/(z - 1.0);
    float z_far = w * z_near/(w + 2.0 * z_near);
    float right = z_near/x;
    float top = z_near/y;

    m_shader->setActive(true);
    addInputFloat("near", z_near);
    addInputFloat("far", z_far);
    addInputFloat("right", right);
    addInputFloat("top", top);
    m_shader->setActive(false);
  }
  KernelBase::setActive(op);
}

void KernelVoxelization::setActiveShaderOnly( bool op, GLfloat *projectionMatrix)
{
  if(op && projectionMatrix)
  {
    float x = projectionMatrix[0*4+0];
    float y = projectionMatrix[1*4+1];
    float z = projectionMatrix[2*4+2];
    float w = projectionMatrix[3*4+2];
    float z_near = w/(z - 1.0);
    float z_far = w * z_near/(w + 2.0 * z_near);
    float right = z_near/x;
    float top = z_near/y;

    m_shader->setActive(true);
      addInputFloat("near", z_near);
      addInputFloat("far", z_far);
      addInputFloat("right", right);
      addInputFloat("top", top);
    m_shader->setActive(false);
  }
  KernelBase::setActiveShaderOnly(op);
}

void printBitLine(unsigned int v)
{
  char s[UINT_BIT_SIZE + 2];
  for(int i = UINT_BIT_SIZE - 1; i >= 0; --i)
  {
    unsigned mask = 1 << i;
    s[UINT_BIT_SIZE - 1 - i] = v & mask ? '1' : '0';
  }
  s[UINT_BIT_SIZE] = '\n';
  s[UINT_BIT_SIZE + 1] = '\0';
  printf(s);
}

void printBit(unsigned int v)
{
  char s[UINT_BIT_SIZE + 1];
  for(int i = UINT_BIT_SIZE - 1; i >= 0; --i)
  {
    unsigned mask = 1 << i;
    s[UINT_BIT_SIZE - 1 - i] = v & mask ? '1' : '0';
  }
  s[UINT_BIT_SIZE] = '\0';
  printf(s);
}


bool isBitActive(unsigned int v, int index)
{
  unsigned i = 1 << (UINT_BIT_SIZE - 1 - index);
  return v&i;
}

#define FULL_ONE 
void KernelVoxelization::createGridBitMapTexture()
{
  int texsize = m_depth*4*4;
  GLfloat* texData = new GLfloat[texsize];

  for(unsigned int i = 0; i < (unsigned int)m_depth; ++i)
  {
    
#ifdef FULL_ONE
    unsigned int v = ~0 << (m_depth%UINT_BIT_SIZE - 1 - i);
#else
    unsigned int v = 1 << (m_depth%UINT_BIT_SIZE - 1 - i);
#endif // FULL_ONE
    for(int j = 0; j < 4; ++j) //Diagonal 
      texData[4*(i+j*UINT_BIT_SIZE)+j] = *((float*)&v);
    
    v = 0;
    for(int j = 0; j < 3;++j) //Up Triangle
      for(int k = j+1; k < 4 ;++k)
        texData[4*(i+UINT_BIT_SIZE*j)+k] = *((float*)&v);

#ifdef FULL_ONE
    v = ~0;
#endif // FULL_ONE
    for(int j = 0; j < 3;++j) //Down Triangle
      for(int k = j+1; k < 4 ;++k)
        texData[4*(i+UINT_BIT_SIZE*k)+j] = *((float*)&v);
  }

  //for(unsigned int i = 0; i < (unsigned int)m_depth*4; ++i)
  //{
  //  printBit(*(unsigned int *)&texData[4*i]);
  //  printBit(*(unsigned int *)&texData[4*i+1]);
  //  printBit(*(unsigned int *)&texData[4*i+2]);
  //  printBitLine(*(unsigned int *)&texData[4*i+3]);
  //}
  GLint max_tex_size;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);
  texsize/=4;

  m_gridBitMapWidth =  texsize > max_tex_size? max_tex_size : texsize;
  m_gridBitMapHeight = texsize/max_tex_size + 1;

  GLTextureObject t;
  t.createTexture2D(m_gridBitMapWidth, m_gridBitMapHeight, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT, texData);
  t.setFilters(GL_NEAREST, GL_NEAREST);
  t.setWraps(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
  m_texIdGridBitMap = t.getId();

  delete [] texData;
}



void KernelVoxelization::createGridFuncTexture()
{
  GLint max_tex_size;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);
  m_gridFuncWidth = min(4096, max_tex_size);

  GLfloat* texData = new GLfloat[m_gridFuncWidth];

  for(int i = 0; i < m_gridFuncWidth; ++i)
  {
    float value = float(i)/(m_gridFuncWidth-1); 
    texData[i] = value*value;
  }


  GLTextureObject t;
  t.createTexture1D(m_gridFuncWidth, GL_ALPHA32F_ARB, GL_ALPHA, GL_FLOAT, texData);
  t.setFilters(GL_NEAREST, GL_NEAREST);
  t.setWraps(GL_CLAMP_TO_EDGE);
  m_texIdGridFunc = t.getId();

  delete [] texData;
}

void KernelVoxelization::renderFrustum(GLfloat* projectionMatrix, 
                                       GLfloat* modelviewMatrix)
{
  Matrix4 mvInvT(modelviewMatrix);
  mvInvT.Invert();
  mvInvT.Transpose();

  Matrix4 prjInvT(projectionMatrix);
  prjInvT.Invert();
  prjInvT.Transpose();

  float px = projectionMatrix[0*4+0];
  float py = projectionMatrix[1*4+1];
  float pz = projectionMatrix[2*4+2];
  float pw = projectionMatrix[3*4+2];
  float z_near = pw/(pz - 1.0);
  float z_far = pw * z_near/(pw + 2.0 * z_near);
  float right = z_near/px;
  float top = z_near/py;
  
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glDisable(GL_LIGHTING);
  glPointSize(5);
  glLineWidth(2);

  glPushMatrix();

  Vector3 p;
  glBegin(GL_POINTS);
    glColor3f(1,1,1);
    p = mvInvT*Vector3(0, 0, 0);
    glVertex3f(p.x, p.y, p.z);
  glEnd();

  glBegin(GL_LINE_STRIP);
    glColor3f(1,0,0);
    p = mvInvT*Vector3(right, top, -z_near);
    glVertex3f(p.x, p.y, p.z);
    p = mvInvT*Vector3(-right, top, -z_near);
    glVertex3f(p.x, p.y, p.z);
    p = mvInvT*Vector3(-right, -top, -z_near);
    glVertex3f(p.x, p.y, p.z);
    p = mvInvT*Vector3(right, -top, -z_near);
    glVertex3f(p.x, p.y, p.z);

    p = mvInvT*Vector3(right, top, -z_near);
    glVertex3f(p.x, p.y, p.z);

    float c = z_far/z_near;
    p = mvInvT*Vector3(right*c, top*c, -z_far);
    glVertex3f(p.x, p.y, p.z);
    p = mvInvT*Vector3(-right*c, top*c, -z_far);
    glVertex3f(p.x, p.y, p.z);
    p = mvInvT*Vector3(-right*c, -top*c, -z_far);
    glVertex3f(p.x, p.y, p.z);
    p = mvInvT*Vector3(right*c, -top*c, -z_far);
    glVertex3f(p.x, p.y, p.z);

    p = mvInvT*Vector3(right*c, top*c, -z_far);
    glVertex3f(p.x, p.y, p.z);
  glEnd();

  glBegin(GL_LINES);
    glColor3f(1,0,0);
    p = mvInvT*Vector3(-right, top, -z_near);
    glVertex3f(p.x, p.y, p.z);
    p = mvInvT*Vector3(-right*c, top*c, -z_far);
    glVertex3f(p.x, p.y, p.z);


    p = mvInvT*Vector3(-right, -top, -z_near);
    glVertex3f(p.x, p.y, p.z);
    p = mvInvT*Vector3(-right*c, -top*c, -z_far);
    glVertex3f(p.x, p.y, p.z);

    p = mvInvT*Vector3(right, -top, -z_near);
    glVertex3f(p.x, p.y, p.z);
    p = mvInvT*Vector3(right*c, -top*c, -z_far);
    glVertex3f(p.x, p.y, p.z);

    p = mvInvT*Vector3(right, top, -z_near);
    glVertex3f(p.x, p.y, p.z);
    p = mvInvT*Vector3(right*c, top*c, -z_far);
    glVertex3f(p.x, p.y, p.z);
  glEnd();
  glPopMatrix();
  glPopAttrib();
}

//#define RENDER_UNPROJECT
void KernelVoxelization::renderVoxelization(GLuint  *data, Vector3 sizeBB, Vector3 centerBB, 
                                            Vector3 camPos, Vector3 camAt, Vector3 camUp, 
                                            GLfloat* projectionMatrix, 
                                            GLfloat* modelviewMatrix,
                                            float fovy)
{
  Matrix4 mv(modelviewMatrix);
  mv.Transpose();
  Matrix4 mvInvT(modelviewMatrix);
  mvInvT.Invert();
  mvInvT.Transpose();

  Matrix4 prj(projectionMatrix);
  Matrix4 prjInvT(projectionMatrix);
  prjInvT.Invert();
  prjInvT.Transpose();

  float px = projectionMatrix[0*4+0];
  float py = projectionMatrix[1*4+1];
  float pz = projectionMatrix[2*4+2];
  float pw = projectionMatrix[3*4+2];
  float z_near = pw/(pz - 1.0);
  float z_far = pw * z_near/(pw + 2.0 * z_near);
  float right = z_near/px;
  float top = z_near/py;

  glPushMatrix();
  //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glPushAttrib(GL_ALL_ATTRIB_BITS);

//  renderFrustum(projectionMatrix, modelviewMatrix);

  glPushMatrix();

  int y = 256;
  int x = 256;
  int step = 10;
  int stepZ = 2;

  if(fovy >= .5)
  {
    for(int y = 0; y < m_height; y+=step)
      for(int x = 0; x < m_width; x+=step)
      {
        unsigned int *p = (GLuint*)&data[y*m_width*4+x*4];

        for(int z = 0; z < (m_gridBitMapHeight*m_gridBitMapWidth); z += 1)
        {
          unsigned int pp = *(p + z/UINT_BIT_SIZE);
          if(isBitActive(pp, z%UINT_BIT_SIZE))
          {
            float xn = 2. * float(x+.5)/m_width - 1.;
            float yn = 2. * float(y+.5)/m_height- 1.;

            float ze = -((float(z)/(m_gridBitMapHeight*m_gridBitMapWidth))*z_far + z_near);
            float zn = - ze * (z_far + z_near)/(z_far - z_near) - 2. * z_far * z_near/(z_far - z_near);
            zn = zn / -ze;

            Vector4 pe = Vector4(xn, yn, zn, 1.0f);
            pe = (prjInvT * pe);
            pe = pe.homogeneous();
            Vector3 pw = (mvInvT * pe).vec3();

            glPushMatrix();

            glTranslatef(pw.x, pw.y, pw.z);

            //glScalef(10./m_width, 10./m_height,1./(m_gridBitMapHeight*m_gridBitMapWidth));
            //glScalef(1./(m_gridBitMapHeight*m_gridBitMapWidth), 10./m_height, 10./m_width);
            //glEnable(GL_NORMALIZE);
            //glutSolidCube(1);
            //glutSolidSphere(10./512, 5,5);
            glutSolidCube(2.0f*float(step)/512);
            glPopMatrix();
          }
        }
      }
  }
  else
  {
    for(int y = 0; y < m_height; y+=step)
    {
      for(int x = 0; x < m_width; x+=step)
      {
        unsigned int *p = (GLuint*)&data[y*m_width*4+x*4];

        for(int z = 0; z < (m_gridBitMapHeight*m_gridBitMapWidth); z += stepZ)
        {
          unsigned int pp = *(p + z/UINT_BIT_SIZE);
          if(isBitActive(pp, z%UINT_BIT_SIZE))
          {
            Vector3 centerE = mv*centerBB;
            float xe = 2.0f * float(x + .5)/(m_width ) - 1.0f;
            float ye = 2.0f * float(y + .5)/(m_height) - 1.0f;

            float ze = 2.0f * (float(z)/(m_gridBitMapHeight*m_gridBitMapWidth)) - 1.0f;
            Vector3 pw = mvInvT*(Vector3(xe, ye, -ze*5.0f) + centerE);
            glPushMatrix();

            glTranslatef(pw.x, pw.y, pw.z);
            //glTranslatef(xe, ye, ze);

            glScalef(float(step)/m_width, float(step)/m_height,float(stepZ*5.0f)/(m_gridBitMapHeight*m_gridBitMapWidth));
            //glScalef(1./(m_gridBitMapHeight*m_gridBitMapWidth), 10./m_height, 10./m_width);
            glEnable(GL_NORMALIZE);
            glEnable(GL_COLOR_MATERIAL);
            glColorMaterial(GL_FRONT, GL_DIFFUSE);
            glColor3f(.5,.2,.2);
            //glutSolidCube(float(step)/512);
            glutSolidCube(1);
            glDisable(GL_COLOR_MATERIAL);
            //glutSolidSphere(10./512, 5,5);
            //glutSolidCube(2.0f*float(step)/512);
            glPopMatrix();
          }
        }
      }
    }
  }



  //    //for(int m = 0; m < 4; ++m)
  //    //  for(int k = 0; k < m_gridBitMapHeight; ++k)
  //    //    for(int l = 0; l < m_gridBitMapWidth;l+=1)
  //    //      //printBitLine(data[i*m_width*4+j*4+m]);
  //    //      if(isBitActive(data[i*m_width*4+j*4+m],k*m_gridBitMapWidth + l))
  //    //      {
  //    //        float x = 2.*float(j)/m_width - 1.;
  //    //        float y = 2.*float(i)/m_height - 1.;
  //    //        float z = 2.*float(k*m_gridBitMapWidth + l)/(m_gridBitMapHeight*m_gridBitMapWidth) - 1.;
  //    //        //glVertex3f(x*sizeBB.x , y*sizeBB.y , z*sizeBB.z );
  //    //        glVertex3f(z*sizeBB.z , y*sizeBB.y , x*sizeBB.x );
  //    //      }

  glPopMatrix();

  glPopAttrib();
  glPopMatrix();
}