/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Aug 2012
 *
 *  Animation Renderer
 *  Several key frames with a material and a transformation
 */

#pragma once

#include <GL/glut.h>
#include <vector>
#include <time.h>

#include "MathUtils/Vector3.h"

using namespace std;

class KeyFrameMesh;
class GLShader;

/**
 * VBO Animation Utility Class.
 *  Uses Shader for motion tween between key frames;
 */
class GLAnimationObject
{
public:  
  enum AnimType{
    Regular = 0,
    Reverse,
    Loop,

    MaxAnimType
  };

protected:

  vector<KeyFrameMesh*> m_keyFrames;  // Key Frames List

  float m_animationSpeed;             // Animation Speed (FPS)
  AnimType m_animType;                // Animation render type

  int m_curFrame;                     // Current Key Frame;
  clock_t m_curTime;                  // Current Time
  float m_curAnimTween;               // Current Animation TweenFactor 
  
  GLShader* m_animShader;             // Shader responsible by the vertex interpolation

  bool m_keyFramesOutdated;           // KeyFrame Setup OutDated 
public:
  /**
   * Constructs animation Obj
   *  anim speed is in frames per second
   *  anim type is the type of animation, Regular, Loop, Reverse...
   */
  GLAnimationObject(float animSpeed = 1.0f, AnimType animType = Loop);
  
  /**
   * Deletes allocated buffers in the GPU's memory
   */
  ~GLAnimationObject();

  /**
   * Remove all associated frames.
   *  Do not delete those key frames.
   */
  void clear();

  /**
   *	Update the status of the animation.
   */
  void configure();


  /**
   * Render the animation.
   *  Update must be called before.
   *  If useShader is true the animation will interpolate at each vertex of the keyframes
   */
  void render(bool useShader);

  /**
   *	Add a key frame in the end of the animation.
   */
  void addKeyFrame(KeyFrameMesh* kf);

  /**
   *	Get the current animation tween factor [0, 1]
   */
  float getAnimTweenFactor() const;

  /**
   *	Get the current animation frame index
   */
  int getCurFrame() const;

  /**
   * Get Bounding Box info
   */
  Vector3 getBoundingBoxSize(int frameIndex = 0) const;
  Vector3 getBoundingBoxCenter(int frameIndex = 0) const;
  Vector3 getBoundingBoxMin(int frameIndex = 0) const;
  Vector3 getBoundingBoxMax(int frameIndex = 0) const;

private:


  /**
   *	Calculate the next key frame based on the type of the animation
   */
  int incKeyFrame();

  /**
   *	Setup the key frames
   */
  void setupKeyFrames();
};
