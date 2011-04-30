// gridbox.cpp
// malf@tecgraf.puc-rio.br
// Feb 2010

#ifndef SCIUTILS_GRID_BOX_H
#define SCIUTILS_GRID_BOX_H

#include "defines.h"
#include <stdlib.h>
#include <ugl/gridtex.h>

class UGLFrameBuffer;
class UGLTexture;

class SCIH_API SciGridBox
{
public:
  typedef void (*ModelRenderFunc) (void *data); 

  /**
  * Constructs a new SciGridBox object.
  */
  SciGridBox ();  
  /**
  * Destroys a VglCamera object.
  */
  ~SciGridBox ();

  /**
  * Sets the grid's bounding box.
  */
  void SetBoundingBox (float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
  /**
  * Sets the grid's color.
  */
  void SetGridColor (float r, float g, float b);
  /**
  * Sets the box's color.
  */
  void SetBoxColor (float r, float g, float b);
  /**
  * Enables/Disables the background box.
  */
  void SetEnabledBox (bool state);
  /**
  * Enables/Disables the grid.
  */
  void SetEnabledGrid (bool state);
  /**
  * Enables/Disables the projection.
  */
  void SetEnabledProjection (bool state);
  /**
  * Sets the bounding box offset in percentage related to the model.
  */
  void SetBoxOffset (float percentage);
  /**
  * Sets the render callback.
  */
  void SetModelRenderFunc (ModelRenderFunc func, void *data=NULL);
  /**
  * Enable the fbo to draw the texture again.
  */
  void SetOutDated ();
  /**
  * Gets the grid's bounding box coordinates.
  */
  void GetGridBoundingBox (float* xmin, float* xmax, float* ymin, float* ymax, float* zmin, float* zmax);
  /**
  * Gets the object's bounding box coordinates.
  */
  void GetObjectBoundingBox (float* xmin, float* xmax, float* ymin, float* ymax, float* zmin, float* zmax);
  /**
  * Render method.
  */
  void Render ();
  /**
  * Sets the numbers of intervals for the grid.
  */
  void SetNumOfIntervals (int count);
  /**
  * Sets the tick enabled.
  */
  void SetGridTickEnabled (bool state);
  /**
  * 
  */
  void SetGridPosAtFront (bool state);

  void RecalculateGridBoundingBox ();

private:
  void DrawBox ();
  void DrawGrid ();
  void DrawProjection ();
  void GenerateTextures ();
  void RenderModelToTexture ();
  void GetFaceSize (int id, int *face_w, int *face_h);
  void SetObjectBoundingBox (float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
  void SetGridBoundingBox (float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);

private:
  bool m_box_draw;            // Boolean for controlling the drawing of the box.
  bool m_grid_draw;           // Boolean for controlling the drawing of the grid.
  bool m_proj_draw;           // Boolean for controlling the drawing of the projection.
  bool m_updated;             // Boolean for updating the fbo textures.
  bool m_grid_tick;
  bool m_grid_pos_front;

  float m_grid_bbox[6];       // Bounding Box xmin, xmax, ymin, ymax, zmin, zmax.
  float m_obj_bbox[6];        // Object Bounding Box (xmin, xmax, ymin, ymax, zmin, zmax).
  float m_bbox_offsetperc;    // Offset in % related to the object's Bounding box.
  float m_bbox_offset;        // Offset in pixels related to the object's Bounding box.

  float m_box_color[3];       // Color array fot the box (r,g,b).
  float m_grid_color[3];      // Color array fot the grid (r,g,b).

  UGLFrameBuffer *m_proj_fbo; // FBO for the projective images.
  UGLTexture *m_proj_tex[6];  // Texture array for the projective image (FF, BF, TF, BF, RF & LF).
  UGLGridTex * m_grid_tex;    // Texture for the grid.

  ModelRenderFunc m_render;   // Render Callback for the model.
  void* m_data;               // Data for the render callback.

  int m_num_intervals;        // Number of intervals of the grid.
  float m_gridscale_x;
  float m_gridscale_y;
  float m_gridscale_z;
  float m_grid_sqsize;
};

#endif
