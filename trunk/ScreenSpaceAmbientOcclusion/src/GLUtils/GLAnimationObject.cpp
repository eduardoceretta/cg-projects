/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Aug 2012
 *
 *  Animation Renderer
 *  Several key frames with a material and a transformation
 */


#include <GL/glew.h>
#include <iostream>
#include <limits>
#include "defines.h"

#include "GLUtils/GLAnimationObject.h"
#include "GLUtils/GLShader.h"
#include "MeshLoaders/KeyFrameMesh.h"

using namespace std;

/************************************************************************/
/* GLAnimationObject                                                 */
/************************************************************************/


GLAnimationObject::GLAnimationObject(float animSpeed /*= 0.1f*/, AnimType animType /*= Regular*/)
:m_animationSpeed(animSpeed)
,m_curFrame(0)       
,m_curAnimTween(0.0f)
,m_animType(animType)
,m_animShader(NULL)
,m_keyFramesOutdated(true)
{
  m_curTime = clock();
  //m_animShader = new GLShader();
}

GLAnimationObject :: ~GLAnimationObject()
{
  //if(m_animShader)
  //  delete m_animShader;
}

void GLAnimationObject :: clear()
{
  m_keyFrames.clear();
}


void GLAnimationObject::addKeyFrame( KeyFrameMesh* kf )
{
  if(kf)
  {
    m_keyFrames.push_back(kf);
    m_keyFramesOutdated = true;
  }
}


void GLAnimationObject :: render(bool useShader)
{
  if(m_keyFrames.size() <= 0)
    return;
  if(useShader == false)
  {
    KeyFrameMesh* kf = m_keyFrames[m_curFrame];
    GLVertexBufferObject* vbo = kf->getVbo();
    vbo->render();
  }else
  {

  }
}


void GLAnimationObject::configure()
{
  setupKeyFrames();

  int numFrames = m_keyFrames.size();
  clock_t time = clock();
  double diff = (double)(time - m_curTime)/CLOCKS_PER_SEC;

  if(diff >= (1.0f/m_animationSpeed))
  {
    m_curTime = time;
    m_curAnimTween = 0.0;
    m_curFrame = incKeyFrame();
  }
  m_curAnimTween = m_animationSpeed * (diff);
}

int GLAnimationObject::incKeyFrame()
{
  int numFrames = m_keyFrames.size();
  switch(m_animType)
  {
  case Regular:
    return min(m_curFrame + 1, numFrames - 1);

  case Reverse:
    return min(m_curFrame - 1, 0);

  case Loop:
    return (m_curFrame + 1) % numFrames;
  }
  return m_curFrame;
}

void GLAnimationObject::setupKeyFrames()
{
  if(m_keyFramesOutdated == false)
    return;
  if(m_keyFrames.size() > 1)
  {
    vector<KeyFrameMesh*> :: iterator it;
    vector<KeyFrameMesh*> :: iterator itprev = m_keyFrames.end() - 1;
    vector<KeyFrameMesh*> :: iterator itnext = m_keyFrames.begin() + 1;
    for(it = m_keyFrames.begin(); it != m_keyFrames.end(); ++it, ++itprev, ++itnext)
    {
      if(itprev == m_keyFrames.end())
        itprev = m_keyFrames.begin();

      if(itnext == m_keyFrames.end())
        itnext = m_keyFrames.begin();

      KeyFrameMesh* kf = *it;
      kf->setNextKeyFrame(*itnext);
      kf->setPreviousKeyFrame(*itprev);
    }
  }

  m_keyFramesOutdated = false;
}

float GLAnimationObject::getAnimTweenFactor() const
{
  return m_curAnimTween;
}

int GLAnimationObject::getCurFrame() const
{
  return m_curFrame;
}


Vector3 GLAnimationObject::getBoundingBoxSize(int frameIndex/* = 0*/) const
{
  return m_keyFrames.at(frameIndex)->getVbo()->getBoundingBoxSize();
}

Vector3 GLAnimationObject::getBoundingBoxCenter(int frameIndex/* = 0*/) const
{
  return m_keyFrames.at(frameIndex)->getVbo()->getBoundingBoxCenter();
}

Vector3 GLAnimationObject::getBoundingBoxMin(int frameIndex/* = 0*/) const
{
  return m_keyFrames.at(frameIndex)->getVbo()->getBoundingBoxMin();
}

Vector3 GLAnimationObject::getBoundingBoxMax(int frameIndex/* = 0*/) const
{
  return m_keyFrames.at(frameIndex)->getVbo()->getBoundingBoxMax();
}




