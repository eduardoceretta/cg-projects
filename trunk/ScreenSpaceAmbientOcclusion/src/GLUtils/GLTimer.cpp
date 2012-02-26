/**
 *	Eduardo Ceretta Dalla Favera from Fabio Markus Miranda.
 *  eduardo.ceretta@gmail.com
 *
 *  Feb 2012  
 *
 *  OpenGL Timer utility class.
 */

#include "GLTimer.h"

GLTimer::GLTimer()
:m_query(0u)
{
}

GLTimer::~GLTimer()
{
  reset();
}


void GLTimer::reset()
{
  if(m_query)
    glDeleteQueries(1, &m_query);
  m_query = 0u;
}

void GLTimer::init()
{
  glGenQueries(1, &m_query);
  m_available = 0;
}

void GLTimer::start()
{
  glBeginQuery(GL_TIME_ELAPSED_EXT, m_query);
}

void GLTimer::stop()
{
  glEndQuery(GL_TIME_ELAPSED_EXT);
}

GLuint64EXT GLTimer::getElapsedTime()
{
  while (!m_available)
    glGetQueryObjectiv(m_query, GL_QUERY_RESULT_AVAILABLE, &m_available);
  glGetQueryObjectui64vEXT(m_query, GL_QUERY_RESULT, &m_timeElapsed);
  m_available = 0;
  return m_timeElapsed;
}

double GLTimer::getElapsedTimeInMilliseconds()
{
  long long nanotime = (long long) getElapsedTime();
  return nanotime*1e-6;
}