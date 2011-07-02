/**
 *	Eduardo Ceretta Dalla Favera with modifications of Cesar Tadeu Pozzer
 *  eduardo.ceretta@gmail.com, pozzer3@gmail.com
 *  Mar 2010  
 *
 *  OpenGL VertexBufferObject utility class.
 *  Considers 3D Vertexes, 3D Normals and RGB Colors
 */
#include <GL/glew.h>
#include <iostream>
#include <limits>
#include "defines.h"

#include "GLVertexBufferObject.h"

using namespace std;


/************************************************************************/
/* GLVBOBuffer - struct vbobuffer                                       */
/************************************************************************/
vbobuffer::vbobuffer() :data(NULL)
,type(sizeof(GL_BYTE))
,n(0)
,offset(0)
,clientState(GL_VERTEX_ARRAY)
{
}

void vbobuffer::setPointer()
{
  switch(clientState)
  {
  case GL_NORMAL_ARRAY:
    glNormalPointer(type, 0, (GLvoid*)offset);
    break;

  case GL_COLOR_ARRAY: 
    glColorPointer(3, type,  0, (GLvoid*)offset);
    break;

  case GL_VERTEX_ARRAY:
    glVertexPointer(3, type,  0, (GLvoid*)offset);
    break;

  case GL_TEXTURE_COORD_ARRAY:
    glTexCoordPointer(2, type,  0, (GLvoid*)offset);
    break;

  case GL_INDEX_ARRAY:
    glIndexPointer(type, 0, (GLvoid*)offset);
    break;

  case GL_EDGE_FLAG_ARRAY:
    glEdgeFlagPointer(0, (GLvoid*)offset);
    break;
  }
}

int vbobuffer::sizeInBytes()
{
  switch(clientState)
  {
  case GL_NORMAL_ARRAY:
  case GL_COLOR_ARRAY:  
  case GL_VERTEX_ARRAY:
    return n*sizeof(type)*3;

  case GL_TEXTURE_COORD_ARRAY:
    return n*sizeof(type)*2;

  case GL_INDEX_ARRAY:
  case GL_EDGE_FLAG_ARRAY:
    return n*sizeof(type);
  }
}



/************************************************************************/
/* GLVertexBufferObject                                                 */
/************************************************************************/
bool GLVertexBufferObject :: s_IsSupported()
{
   static int supported = -1;
   if(supported != -1)
      return supported == 1;

   supported = 1;

   GLenum err = glewInit();
   if (GLEW_OK != err)
   {
      supported = 0;
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
   }

   if (!glewIsSupported("GL_VERSION_2_0"))
   {
      supported = 0;
      printf("OpenGL 2.0 not supported\n");
   }

   if(!glGenBuffersARB || !glBindBufferARB || !glBufferDataARB || !glBufferSubDataARB ||
         !glMapBufferARB || !glUnmapBufferARB || !glDeleteBuffersARB || !glGetBufferParameterivARB)
   {
      supported = 0;
      cout << "Video card does NOT support GL_ARB_vertex_buffer_object." << endl;
   }

   return supported == 1;
}

GLVertexBufferObject::GLVertexBufferObject(GLenum primitive)
:m_primitive(primitive)
,m_calculated(false)
,m_hasIndices(false)
,m_vboId(0)
,m_vboIndicesId(0)
,m_VBOBuffersTotalSize(0)
,m_bb_size(Vector3(-1, -1, -1))
,m_bb_center(Vector3(0, 0, 0))
,m_bb_min(Vector3(numeric_limits<float>::infinity( ), numeric_limits<float>::infinity( ), numeric_limits<float>::infinity( )))
,m_bb_max(Vector3(-numeric_limits<float>::infinity( ),-numeric_limits<float>::infinity( ),-numeric_limits<float>::infinity( )))
{
   m_supported = s_IsSupported();

   assert(m_supported);
}

GLVertexBufferObject :: ~GLVertexBufferObject()
{
   if(m_supported && m_vboId)
   {
      glDeleteBuffersARB(1, &m_vboId);
      if(m_vboIndicesId)
         glDeleteBuffersARB(1, &m_vboIndicesId);
   }
}

void GLVertexBufferObject :: clear()
{
   m_VBOBuffers.clear();
   m_calculated = false;

   m_bb_size = Vector3(-1, -1, -1);
   m_bb_center = Vector3(0, 0, 0);
  
   glDeleteBuffersARB(1, &m_vboId);
   if(m_hasIndices == true)
   {
      glDeleteBuffersARB(1, &m_vboIndicesId);
   }
}


void GLVertexBufferObject :: calcVBO()
{
   if(m_calculated==true)
      return;
  
   vector<GLVBOBuffer> :: iterator it;
   m_VBOBuffersTotalSize = 0;
   for(it = m_VBOBuffers.begin(); it != m_VBOBuffers.end(); ++it)
   {
      it->offset = m_VBOBuffersTotalSize;
      m_VBOBuffersTotalSize += it->sizeInBytes();
   }

   glGenBuffersARB(1, &m_vboId);
   glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vboId);
   glBufferDataARB(GL_ARRAY_BUFFER_ARB, m_VBOBuffersTotalSize, 0, GL_STATIC_DRAW_ARB); // Allocates space for the memory block

   for(it = m_VBOBuffers.begin(); it != m_VBOBuffers.end(); ++it)
      glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, it->offset, it->sizeInBytes(), it->data); //writes the data to respective the buffer position
      
   if(m_hasIndices == true)
   {
      glGenBuffersARB(1, &m_vboIndicesId);
      glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_vboIndicesId);
      glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_VBOIndexBuffer.sizeInBytes(), m_VBOIndexBuffer.data, GL_STATIC_DRAW_ARB);
   }

   calcBoundingBox();

   m_calculated = true;
}

GLenum GLVertexBufferObject::getPrimitive() const
{
  return m_primitive;
}

void GLVertexBufferObject::setPrimitive( GLenum val )
{
  m_primitive = val;
}


void GLVertexBufferObject :: render()
{
   glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vboId);
   if(m_vboIndicesId)
      glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, m_vboIndicesId);

   vector<GLVBOBuffer> :: iterator it;
   for(it = m_VBOBuffers.begin(); it != m_VBOBuffers.end(); ++it)
   {
      glEnableClientState(it->clientState);
      it->setPointer();
   }

   if(m_hasIndices == true)
      glDrawElements(m_primitive, m_VBOIndexBuffer.n, m_VBOIndexBuffer.type, 0);
   else
      glDrawArrays(m_primitive, 0, m_VBOBuffers.begin()->n);

   for(it = m_VBOBuffers.begin(); it != m_VBOBuffers.end(); ++it)
      glDisableClientState(it->clientState);

   if(m_hasIndices == true)
      glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
   glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}


void GLVertexBufferObject :: setVBOBuffer(GLenum clientState, GLenum type, int n, void* data)
{
   m_calculated = false;
   
   if(clientState == GL_INDEX_ARRAY)
   {
      setVBOIndexBuffer(type, n, data);
      return;
   }

   GLVBOBuffer buff;
   buff.clientState = clientState;
   buff.type = type;
   buff.n = n;
   buff.data = data;

   m_VBOBuffers.push_back(buff);
}

void GLVertexBufferObject :: setVBOIndexBuffer(GLenum type, int n, void* data)
{
   m_hasIndices = true;
   m_VBOIndexBuffer.clientState = GL_INDEX_ARRAY;
   m_VBOIndexBuffer.type = type;
   m_VBOIndexBuffer.n = n;
   m_VBOIndexBuffer.data = data;
}

void GLVertexBufferObject::writeToFile( FILE * fp )
{
  MyAssert("Invalid File Pointer.", fp);

  vector<GLVBOBuffer> :: iterator it;
  int totalSize = sizeof(int)+ sizeof(bool);
  
  if(m_hasIndices)
    totalSize += sizeof(GLVBOBuffer) +m_VBOIndexBuffer.sizeInBytes();
  
  for(it = m_VBOBuffers.begin(); it != m_VBOBuffers.end(); ++it )
    totalSize += sizeof(GLVBOBuffer) + it->sizeInBytes();
  
  
  fwrite(&totalSize, sizeof(int), 1, fp);
  fwrite(&m_hasIndices, sizeof(bool), 1, fp);

  if(m_hasIndices)
  {
    fwrite(&m_VBOIndexBuffer, sizeof(GLVBOBuffer), 1, fp);
    fwrite(m_VBOIndexBuffer.data, m_VBOIndexBuffer.sizeInBytes(), 1, fp);
  }

  for(it = m_VBOBuffers.begin(); it != m_VBOBuffers.end(); ++it)
  {
    fwrite(&(*it), sizeof(GLVBOBuffer), 1, fp);
    fwrite(it->data, it->sizeInBytes(), 1, fp);
  }
}

void GLVertexBufferObject::readFromFile( FILE * fp )
{
  MyAssert("Invalid File Pointer.", fp);

  int totalSize = -1;
  fread(&totalSize, sizeof(int), 1, fp);
  totalSize -= sizeof(int);
  fread(&m_hasIndices, sizeof(bool), 1, fp);
  totalSize -= sizeof(bool);

  if(m_hasIndices)
  {
    fread(&m_VBOIndexBuffer, sizeof(GLVBOBuffer), 1, fp);
    m_VBOIndexBuffer.data = malloc(m_VBOIndexBuffer.sizeInBytes());
    fread(m_VBOIndexBuffer.data, m_VBOIndexBuffer.sizeInBytes(), 1, fp);
    totalSize -= sizeof(GLVBOBuffer) + m_VBOIndexBuffer.sizeInBytes();
  }

  while(totalSize > 0)
  {
    GLVBOBuffer buff;
    fread(&buff, sizeof(GLVBOBuffer), 1, fp);
    buff.data = malloc(buff.sizeInBytes());

    fread(buff.data, buff.sizeInBytes(), 1, fp);
    m_VBOBuffers.push_back(buff);

    totalSize -= sizeof(GLVBOBuffer) + buff.sizeInBytes();
  }

  m_calculated = false;
}

int GLVertexBufferObject::getNumElements()
{
  int size = -1;
  if(m_hasIndices)
    size = m_VBOIndexBuffer.n;
  else
  {
    vector<GLVBOBuffer> :: iterator it;

    for(it = m_VBOBuffers.begin(); it != m_VBOBuffers.end(); ++it)
      if(it->clientState == GL_VERTEX_ARRAY)
        size = it->n;
  }

  switch(m_primitive)
  {
    case GL_POINTS:
    case GL_LINE_LOOP:
    case GL_POLYGON:
      return size;
    case GL_LINE_STRIP:
      return size - 1;
    case GL_LINES:
      return size/2;
    case GL_TRIANGLE_STRIP:
    case GL_TRIANGLE_FAN:
      return size - 2;
    case GL_TRIANGLES:
      return size/3;
    case GL_QUAD_STRIP:
      return size/2 - 1;
    case GL_QUADS:
      return size/4;
  }
  return size;
}

int GLVertexBufferObject::getNumVertexes()
{
  vector<GLVBOBuffer> :: iterator it;

  for(it = m_VBOBuffers.begin(); it != m_VBOBuffers.end(); ++it)
    if(it->clientState == GL_VERTEX_ARRAY)
      return it->n;
  return -1;
}

void GLVertexBufferObject::calcBoundingBox()
{
  vector<GLVBOBuffer> :: iterator it;

  for(it = m_VBOBuffers.begin(); it != m_VBOBuffers.end(); ++it)
    if(it->clientState == GL_VERTEX_ARRAY)
    {
      for(int i = 0; i < it->n; ++i)
      {
        m_bb_min.x = min(((float*)it->data)[i*3]  , m_bb_min.x);
        m_bb_min.y = min(((float*)it->data)[i*3+1], m_bb_min.y);
        m_bb_min.z = min(((float*)it->data)[i*3+2], m_bb_min.z);

        m_bb_max.x = max(((float*)it->data)[i*3]  , m_bb_max.x);
        m_bb_max.y = max(((float*)it->data)[i*3+1], m_bb_max.y);
        m_bb_max.z = max(((float*)it->data)[i*3+2], m_bb_max.z);
      }
      m_bb_size = m_bb_max - m_bb_min;
      m_bb_center = (m_bb_max + m_bb_min)*.5;
    }
}

Vector3 GLVertexBufferObject::getBoundingBoxSize() const
{
  return m_bb_size;
}

Vector3 GLVertexBufferObject::getBoundingBoxCenter() const
{
  return m_bb_center;
}

Vector3 GLVertexBufferObject::getBoundingBoxMin() const
{
  return m_bb_min;
}

Vector3 GLVertexBufferObject::getBoundingBoxMax() const
{
  return m_bb_max;
}

