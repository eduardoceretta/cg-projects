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
#include <cmath>
#include <limits>

#include "MathUtils/Matrix4.h"
#include "MathUtils/Vector4.h"
#include "GLUtils/GLProjectionMatrix.h"

#define UINT_BIT_SIZE 32
#define VOXELIZATION_BITMAP_FULLONE 

KernelVoxelization::KernelVoxelization(char* path, int width, int height, int size, GLuint texIdEyeNearest)
: KernelBase(path, "voxelization.vert", "voxelization.frag", width, height)
,m_width(width)
,m_height(height)
,m_depth(size*UINT_BIT_SIZE)
,m_renderMode(0)
,m_BBCalculated(false)
,m_far(0.0f)
,m_top(0.0f)
,m_near(0.0f)
,m_right(0.0f)
,m_voxBBMin(0,0,0)
,m_voxBBMax(1,1,1)
,m_voxData(NULL)
,m_projectionMatrix(GLProjectionMatrix())
,m_texObj(GLTextureObject())
,m_stepX(1)
,m_stepY(1)
,m_stepZ(1)
,m_texIdEyeNearest(texIdEyeNearest)
{
  m_fbo->attachToDepthBuffer(GL_FBOBufferType::RenderBufferObject);

  createGridBitMapTexture();
  createGridFuncTextures(4.0f);

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
    
    addInputTexture(GL_TEXTURE_2D, "eyeNearest", m_texIdEyeNearest);
    addInputTexture(GL_TEXTURE_2D, "gridBitMap", m_texIdGridBitMap);
    addInputTexture(GL_TEXTURE_1D, "gridFunction", m_texIdGridFunc);
    addInputTexture(GL_TEXTURE_1D, "gridInvFunction", m_texIdGridInvFunc);
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

void KernelVoxelization::setActive(bool op)
{
  m_BBCalculated = false;
  if(op)
  {
    m_voxData = NULL;
    glGetFloatv(GL_MODELVIEW_MATRIX, m_modelviewMatrix);
    m_projectionMatrix.readGLProjection();
    m_near = m_projectionMatrix.getNear();
    m_far = m_projectionMatrix.getFar();
    m_right = m_projectionMatrix.getRight();
    m_top = m_projectionMatrix.getTop();

    m_shader->setActive(true);
    addInputFloat("near", m_near);
    addInputFloat("far", m_far);
    addInputFloat("right", m_right);
    addInputFloat("top", m_top);
    m_shader->setActive(false);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
  }else
  {
    glPopAttrib();
  }

  KernelBase::setActive(op);

  if(op)
  {
    glClearColor(0,0,0,0);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    //glEnableIndexedEXT(GL_BLEND, 0);

    glEnable(GL_DEPTH_CLAMP_NV);//ONLY WORKS ON NVIDIA!!!!!!!!!!!
    glEnable(GL_COLOR_LOGIC_OP);
    glDisable(GL_DEPTH_TEST);
    glLogicOp(GL_XOR);
  }
}

void KernelVoxelization::setActiveShaderOnly( bool op)
{
  m_BBCalculated = false;
  if(op)
  {
    glGetFloatv(GL_MODELVIEW_MATRIX, m_modelviewMatrix);
    m_projectionMatrix.readGLProjection();
    m_near = m_projectionMatrix.getNear();
    m_far = m_projectionMatrix.getFar();
    m_right = m_projectionMatrix.getRight();
    m_top = m_projectionMatrix.getTop();

    m_shader->setActive(true);
    addInputFloat("near", m_near);
    addInputFloat("far", m_far);
    addInputFloat("right", m_right);
    addInputFloat("top", m_top);
    m_shader->setActive(false);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
  }else
  {
    glPopAttrib();
  }

  KernelBase::setActiveShaderOnly(op);

  if(op)
  {
    glClearColor(0,0,0,0);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    //glEnableIndexedEXT(GL_BLEND, 0);

    glEnable(GL_DEPTH_CLAMP_NV);//ONLY WORKS ON NVIDIA!!!!!!!!!!!
    glEnable(GL_COLOR_LOGIC_OP);
    glDisable(GL_DEPTH_TEST);
    glLogicOp(GL_XOR);
  }
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


void KernelVoxelization::createGridBitMapTexture()
{
  int texsize = m_depth*4*4;
  GLfloat* texData = new GLfloat[texsize];

  for(unsigned int i = 0; i < (unsigned int)m_depth; ++i)
  {
    
#ifdef VOXELIZATION_BITMAP_FULLONE
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

#ifdef VOXELIZATION_BITMAP_FULLONE
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



void KernelVoxelization::createGridFuncTextures(float power)
{
  GLint max_tex_size;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);
  m_gridFuncWidth = min(4096, max_tex_size);

  GLfloat* texData = new GLfloat[m_gridFuncWidth];

  for(int i = 0; i < m_gridFuncWidth; ++i)
  {
    float value = float(i)/(m_gridFuncWidth-1); 
    texData[i] = pow(value,power);
  }

  GLTextureObject t;
  t.createTexture1D(m_gridFuncWidth, GL_ALPHA32F_ARB, GL_ALPHA, GL_FLOAT, texData);
  t.setFilters(GL_NEAREST, GL_NEAREST);
  t.setWraps(GL_CLAMP_TO_EDGE);
  m_texIdGridFunc = t.getId();

  for(int i = 0; i < m_gridFuncWidth; ++i)
  {
    float value = float(i)/(m_gridFuncWidth-1); 
    texData[i] = pow(value, 1.0f/power);
  }

  t.createTexture1D(m_gridFuncWidth, GL_ALPHA32F_ARB, GL_ALPHA, GL_FLOAT, texData);
  t.setFilters(GL_NEAREST, GL_NEAREST);
  t.setWraps(GL_CLAMP_TO_EDGE);
  m_texIdGridInvFunc = t.getId();

  delete [] texData;
}


void KernelVoxelization::reloadGridFuncTextures( float power )
{
  createGridFuncTextures(power);
  m_shader->setActive(true);
    addInputTexture(GL_TEXTURE_1D, "gridFunction", m_texIdGridFunc);
    addInputTexture(GL_TEXTURE_1D, "gridInvFunction", m_texIdGridInvFunc);
  m_shader->setActive(false);
}

void KernelVoxelization::renderFrustum()
{
  Matrix4 mvInvT(m_modelviewMatrix);
  mvInvT.Invert();
  mvInvT.Transpose();

  Matrix4 prjInvT(m_projectionMatrix);
  prjInvT.Invert();
  prjInvT.Transpose();
  
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
    p = mvInvT*Vector3(m_right, m_top, -m_near);
    glVertex3f(p.x, p.y, p.z);
    p = mvInvT*Vector3(-m_right, m_top, -m_near);
    glVertex3f(p.x, p.y, p.z);
    p = mvInvT*Vector3(-m_right, -m_top, -m_near);
    glVertex3f(p.x, p.y, p.z);
    p = mvInvT*Vector3(m_right, -m_top, -m_near);
    glVertex3f(p.x, p.y, p.z);

    p = mvInvT*Vector3(m_right, m_top, -m_near);
    glVertex3f(p.x, p.y, p.z);

    float c;
    if(m_projectionMatrix.isOrthographic())
      c = 1.0f;
    else c = m_far/m_near;
    p = mvInvT*Vector3(m_right*c, m_top*c, -m_far);
    glVertex3f(p.x, p.y, p.z);
    p = mvInvT*Vector3(-m_right*c, m_top*c, -m_far);
    glVertex3f(p.x, p.y, p.z);
    p = mvInvT*Vector3(-m_right*c, -m_top*c, -m_far);
    glVertex3f(p.x, p.y, p.z);
    p = mvInvT*Vector3(m_right*c, -m_top*c, -m_far);
    glVertex3f(p.x, p.y, p.z);

    p = mvInvT*Vector3(m_right*c, m_top*c, -m_far);
    glVertex3f(p.x, p.y, p.z);
  glEnd();

  glBegin(GL_LINES);
    glColor3f(1,0,0);
    p = mvInvT*Vector3(-m_right, m_top, -m_near);
    glVertex3f(p.x, p.y, p.z);
    p = mvInvT*Vector3(-m_right*c, m_top*c, -m_far);
    glVertex3f(p.x, p.y, p.z);


    p = mvInvT*Vector3(-m_right, -m_top, -m_near);
    glVertex3f(p.x, p.y, p.z);
    p = mvInvT*Vector3(-m_right*c, -m_top*c, -m_far);
    glVertex3f(p.x, p.y, p.z);

    p = mvInvT*Vector3(m_right, -m_top, -m_near);
    glVertex3f(p.x, p.y, p.z);
    p = mvInvT*Vector3(m_right*c, -m_top*c, -m_far);
    glVertex3f(p.x, p.y, p.z);

    p = mvInvT*Vector3(m_right, m_top, -m_near);
    glVertex3f(p.x, p.y, p.z);
    p = mvInvT*Vector3(m_right*c, m_top*c, -m_far);
    glVertex3f(p.x, p.y, p.z);
  glEnd();
  glPopMatrix();
  glPopAttrib();
}


void KernelVoxelization::renderVoxelization()
{
  updateData();

  glPushMatrix();
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  
  //renderFrustum();
  
  glPushMatrix();
  int maxZ = -1;
  if(m_renderMode == 1)
  {
    //Matrix4 mv(m_modelviewMatrix);
    //mv.Transpose();
    //Matrix4 mvInvT(m_modelviewMatrix);
    //mvInvT.Invert();
    //mvInvT.Transpose();

    //Matrix4 prjInvT(m_projectionMatrix);
    //prjInvT.Invert();
    //prjInvT.Transpose();

    //for(int y = 0; y < m_height; y += m_stepX)
    //  for(int x = 0; x < m_width; x += m_stepX)
    //  {
    //    unsigned int *p = (GLuint*)&m_voxData[y*m_width*4+x*4];
   
    //    for(int z = 0; z < (m_gridBitMapHeight*m_gridBitMapWidth); z += 1)
    //    {
    //      unsigned int pp = *(p + z/UINT_BIT_SIZE);
    //      if(isBitActive(pp, z%UINT_BIT_SIZE))
    //      {
    //        float xn = 2. * float(x+.5)/m_width - 1.;
    //        float yn = 2. * float(y+.5)/m_height- 1.;
   
    //        float ze = -((float(z)/(m_gridBitMapHeight*m_gridBitMapWidth))*m_far + m_near);
    //        float zn = - ze * (m_far + m_near)/(m_far - m_near) - 2. * m_far * m_near/(m_far - m_near);
    //        zn = zn / -ze;
   
    //        Vector4 pe = Vector4(xn, yn, zn, 1.0f);
    //        pe = (prjInvT * pe);
    //        pe = pe.homogeneous();
    //        Vector3 pw = (mvInvT * pe).vec3();
   
    //        glPushMatrix();
   
    //        glTranslatef(pw.x, pw.y, pw.z);
   
    //        //glScalef(10./m_width, 10./m_height,1./(m_gridBitMapHeight*m_gridBitMapWidth));
    //        //glScalef(1./(m_gridBitMapHeight*m_gridBitMapWidth), 10./m_height, 10./m_width);
    //        //glEnable(GL_NORMALIZE);
    //        //glutSolidCube(1);
    //        //glutSolidSphere(10./512, 5,5);
    //        glutSolidCube(2.0f*float(m_stepX)/512);
    //        glPopMatrix();
    //      }
    //    }
    //  }
  }
  else
  {
    m_stepX = m_stepY = 5;
    m_stepZ = 1;
    int x = 256;
    int y = 256;
    int z = 64;
    GLTextureObject funcTex(m_texIdGridFunc, GL_TEXTURE_1D);
    GLfloat* func = funcTex.read1DTextureFloatData(GL_ALPHA);
    int funcSize = funcTex.readTextureWidth();


    for(int y = 3.37*m_height/5; y < 4*m_height/5; y += m_stepY)
    //for(int y = 4*m_height/5; y < m_height; y += m_stepY)
    //for(int y = 0; y < m_height; y += m_stepY)
    {
      for(int x = 2*m_width/5; x < 3*m_width/5; x+=m_stepX)
      //for(int x = 2*m_width/5; x < 3*m_width/5; x+=m_stepX)
      //for(int x = 0; x < m_width; x+=m_stepX)
      {
        int somZ = 0;
        unsigned int *p = (GLuint*)&m_voxData[y*m_width*4+x*4];
        for(int z = 0; z < (m_gridBitMapHeight*m_gridBitMapWidth); z += m_stepZ)
        {
          unsigned int pp = *(p + z/UINT_BIT_SIZE);
          if(isBitActive(pp, z%UINT_BIT_SIZE))
          {
            float xm; 
            float ym;
            if(m_projectionMatrix.isOrthographic())
            {
              xm = (2.0f*m_top)/m_width; 
              ym = (2.0f*m_right)/m_height;
            }else
            {
              xm = (2.0f*m_top*(m_near+(m_far - m_near)/2)/m_near)/m_width; //Half Frustum Width
              ym = (2.0f*m_top*(m_near+(m_far - m_near)/2)/m_near)/m_height;//Half Frustum Height 
            }

            //float zm = (m_far - m_near)/(m_gridBitMapHeight*m_gridBitMapWidth);
            //float zm2 = (m_far - m_near)/((m_gridBitMapHeight*m_gridBitMapWidth)*(m_gridBitMapHeight*m_gridBitMapWidth));
            float zm = (m_far - m_near);

            float xe = xm*float(x + .5);
            float ye = ym*float(y + .5);
            //float ze = zm*float(z + .5);
            //float ze2 = zm2*(float(z + .5)*float(z + .5));
            //float ze = zm*func[(int)floor((float(z + .5)/(m_gridBitMapHeight*m_gridBitMapWidth))*funcSize + .5f)];
            float ze = func[(int)floor((float(z + .5)/(m_gridBitMapHeight*m_gridBitMapWidth))*funcSize + .5f)]*m_far + m_near ;


            Vector3 pw = (Vector3(xe, ye, -ze));
            glPushMatrix();
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.1,.0);
            glTranslatef(pw.x, pw.y, pw.z);

            //glScalef(m_stepX*xm, m_stepY*ym, m_stepZ*zm);

            //glScalef(m_stepX*xm, m_stepY*ym, m_stepZ*zm*(
            //  func[(int)floor((float(z + 1.5)/(m_gridBitMapHeight*m_gridBitMapWidth))*funcSize + .5f)]-
            //  func[(int)floor((float(z + 0.5)/(m_gridBitMapHeight*m_gridBitMapWidth))*funcSize + .5f)]
            //));

            glScalef(m_stepX*xm, m_stepY*ym, m_stepZ*m_far*(
              func[(int)floor((float(z + 1.5)/(m_gridBitMapHeight*m_gridBitMapWidth))*funcSize + .5f)]-
              func[(int)floor((float(z + 0.5)/(m_gridBitMapHeight*m_gridBitMapWidth))*funcSize + .5f)]
            ));

            if(isBitActive(pp, z%UINT_BIT_SIZE))
            {
              somZ++;
              maxZ = max(maxZ, somZ);
              glEnable(GL_NORMALIZE);
              glEnable(GL_COLOR_MATERIAL);
              glColorMaterial(GL_FRONT, GL_DIFFUSE);
              glColor3f(.5,.2,.2);
              glutSolidCube(1.0);
              glDisable(GL_COLOR_MATERIAL);
            }
            else
            {
              glPushAttrib(GL_ALL_ATTRIB_BITS);
              glEnable(GL_POLYGON_OFFSET_LINE);
              glPolygonOffset(.1,.0);
              glDisable(GL_LIGHTING);
              glColor3f(1,1,1);
              glutWireCube(1.0);
              glPopAttrib();
            }


            glPopMatrix();
          }
        }
      }
    }
  }

  glPopMatrix();

  glPopAttrib();
  glPopMatrix();
}

Vector3 KernelVoxelization::getVoxBBMax() 
{
  updateBB();
  return m_voxBBMax;
}

Vector3 KernelVoxelization::getVoxBBMin()
{
  updateBB();
  return m_voxBBMin;
}

void KernelVoxelization::updateBB()
{
  if(m_BBCalculated)
    return;
  
  updateData();

  m_voxBBMin = Vector3(numeric_limits<float>::infinity( ), numeric_limits<float>::infinity( ), numeric_limits<float>::infinity( ));
  m_voxBBMax = Vector3(-numeric_limits<float>::infinity( ),-numeric_limits<float>::infinity( ),-numeric_limits<float>::infinity( ));

  if(m_renderMode >=1)
  {
    m_voxBBMin = Vector3(numeric_limits<float>::infinity( ), numeric_limits<float>::infinity( ), numeric_limits<float>::infinity( ));
    m_voxBBMax = Vector3(-numeric_limits<float>::infinity( ),-numeric_limits<float>::infinity( ),-numeric_limits<float>::infinity( ));
  }else
  {
    int x = 256;
    int y = 256;
    GLTextureObject funcTex(m_texIdGridFunc, GL_TEXTURE_1D);
    GLfloat* func = funcTex.read1DTextureFloatData(GL_ALPHA);
    int funcSize = funcTex.readTextureWidth();
    for(int y = 3.37*m_height/5; y < 4*m_height/5; y += m_stepY)
      //for(int y = 4*m_height/5; y < m_height; y += m_stepY)
      //for(int y = 0; y < m_height; y += m_stepY)
    {
      for(int x = 2*m_width/5; x < 3*m_width/5; x+=m_stepX)
        //for(int x = 2*m_width/5; x < 3*m_width/5; x+=m_stepX)
        //for(int x = 0; x < m_width; x+=m_stepX)
      {
        unsigned int *p = (GLuint*)&m_voxData[y*m_width*4+x*4];
        for(int z = 0; z < (m_gridBitMapHeight*m_gridBitMapWidth); z += m_stepZ)
        {
          unsigned int pp = *(p + z/UINT_BIT_SIZE);
          if(isBitActive(pp, z%UINT_BIT_SIZE))
          {
            float xm; 
            float ym;
            if(m_projectionMatrix.isOrthographic())
            {
              xm = (2.0f*m_top)/m_width; 
              ym = (2.0f*m_right)/m_height;
            }else
            {
              xm = (2.0f*m_top*(m_near+(m_far - m_near)/2)/m_near)/m_width; //Half Frustum Width
              ym = (2.0f*m_top*(m_near+(m_far - m_near)/2)/m_near)/m_height;//Half Frustum Height 
            }

            //float zm = (m_far - m_near)/(m_gridBitMapHeight*m_gridBitMapWidth);
            //float zm2 = (m_far - m_near)/((m_gridBitMapHeight*m_gridBitMapWidth)*(m_gridBitMapHeight*m_gridBitMapWidth));
            float zm = (m_far - m_near);

            float xe = xm*float(x + .5);
            float ye = ym*float(y + .5);
            //float ze = zm*float(z + .5);
            //float ze2 = zm2*(float(z + .5)*float(z + .5));
            //float ze = zm*func[(int)floor((float(z + .5)/(m_gridBitMapHeight*m_gridBitMapWidth))*funcSize + .5f)];
            float ze = func[(int)floor((float(z + .5)/(m_gridBitMapHeight*m_gridBitMapWidth))*funcSize + .5f)]*m_far + m_near ;
            Vector3 pw = (Vector3(xe, ye, -ze));


            m_voxBBMin.x = min(pw.x, m_voxBBMin.x);
            m_voxBBMin.y = min(pw.y, m_voxBBMin.y);
            m_voxBBMin.z = min(pw.z, m_voxBBMin.z);

            m_voxBBMax.x = max(pw.x, m_voxBBMax.x);
            m_voxBBMax.y = max(pw.y, m_voxBBMax.y);
            m_voxBBMax.z = max(pw.z, m_voxBBMax.z);
          }
        }
      }
    }
  }
  m_BBCalculated = true;
}

void KernelVoxelization::updateData()
{
  if(!m_voxData)
  {
    m_texObj.setId(getOutputTexture(1));
    m_voxData = m_texObj.read2DTextureUIntData();
  }
}

int KernelVoxelization::getRenderMode() const
{
  return m_renderMode;
}

void KernelVoxelization::setRenderMode( int renderMode )
{
  if(m_renderMode != renderMode)
  {
    m_BBCalculated = false;
    m_renderMode = renderMode;
  }

}
