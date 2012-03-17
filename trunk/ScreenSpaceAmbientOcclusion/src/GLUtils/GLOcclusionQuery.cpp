/**
 *	Eduardo Ceretta Dalla Favera.
 *  eduardo.ceretta@gmail.com
 *
 *  Mar 2012  
 *
 *  OpenGL Occlusion Query utility class.
 */

#include "GLOcclusionQuery.h"

GLOcclusionQuery::GLOcclusionQuery()
:m_query(0u)
,m_numFragments(0u)
,m_available(0)
{
}

GLOcclusionQuery::~GLOcclusionQuery()
{
  reset();
}


void GLOcclusionQuery::reset()
{
  if(m_query)
    glDeleteQueries(1, &m_query);
  m_query = 0u;
}

void GLOcclusionQuery::init()
{
  glGenQueries(1, &m_query);
  m_available = 0;
}

void GLOcclusionQuery::begin()
{
  glBeginQuery(GL_SAMPLES_PASSED, m_query);
}

void GLOcclusionQuery::end()
{
  glEndQuery(GL_SAMPLES_PASSED);
}

GLuint GLOcclusionQuery::getNumberOfFragments()
{
  while (!m_available)
    glGetQueryObjectiv(m_query, GL_QUERY_RESULT_AVAILABLE, &m_available);
  glGetQueryObjectuiv(m_query, GL_QUERY_RESULT, &m_numFragments);
  m_available = 0;
  return m_numFragments;
}
