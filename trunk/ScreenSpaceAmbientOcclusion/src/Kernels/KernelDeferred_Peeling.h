/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Apr 2011
 *
 *  Does a deferred shading and depth peeling.
 *  At each step peels the geometry and writes the render targets 
 *  defined in the enum textures.
 */
#ifndef _KERNEL_DEFERRED_PEELING_H_
#define _KERNEL_DEFERRED_PEELING_H_

#include "Kernels/KernelBase.h"
#include <vector>

using namespace std;

class KernelDeferred_Peeling : public KernelBase {

public:
  /**
   * The render targets produced by the deferred shading
   */
  enum textures {Position = 0, Normal, Diffuse};

  /**
   * Creates the kernel and create the textures for the expected number of peelings.
   *  The depthTexId is an input texture that is the initial depth of the scene.
   *  for a cleared scene should be all 0.0.
   */
  KernelDeferred_Peeling(char* path, int width, int height, GLuint dephtTexId, int numPeelings = 1);
	
  /**
   * Destroy the kernel
   */
  ~KernelDeferred_Peeling();

  /**
   * Do a render cycle.
   *  The pass variable is the depth peeling actual level and sets the output texture.
   */
  void step(int pass);

  /**
   * Get the id of the respective render target
   */
  GLuint getTexIdPosition(int index) const;
  GLuint getTexIdNormal(int index) const;
  GLuint getTexIdDiffuse(int index) const;
  
private:
  /**
   * Texture ids of the render targets
   */
  vector<GLuint> m_texIdPosition;
  vector<GLuint> m_texIdNormal;
  vector<GLuint> m_texIdDiffuse;

  /**
   * Texture id of the intial depth of the scene
   */
  GLuint m_dummyTexId;
};


#endif