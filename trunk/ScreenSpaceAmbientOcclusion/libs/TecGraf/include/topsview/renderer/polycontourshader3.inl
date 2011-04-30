#include <GL/glew.h>

#include <alg/vector.h>
#include <ugl/extmanager.h>
#include <ugl/ugl.h>
#include <shader/cgshader.h>
#include <vgl/frustum.h>

#include "polycontourshader3.h"

//#define VERBOSE

#ifndef TPV_COMPILE_SHADER_FILE
const char* TpvPolyContourShaderGetQ4 ();
const char* TpvPolyContourShaderGetQ8 ();
#endif

MODEL_CLASS_TEMPLATE
int TpvPolyContourShaderRenderer3<MODELCLASS>::InitExtensions()
{
  static bool initialized = false;
  if (initialized)
    return 1;
  if (glewInit() != GLEW_NO_ERROR)
    return 0;
  if (GLEW_VERSION_1_3) {
    initialized = 1;
    return 1;
  } else {
    return 0;
  }
}

MODEL_CLASS_TEMPLATE
bool TpvPolyContourShaderRenderer3<MODELCLASS>::Init()
{
#ifdef TPV_COMPILE_SHADER_FILE
  char filename[256];
#endif

  if (m_initialized)
    return true;

  if (!InitExtensions())
    return false;

  // Initialize CG
  if (!CGShader::Init())
    return false;

  // Shader Q4
#ifdef TPV_COMPILE_SHADER_FILE
  strcpy(filename, m_shader_path);
  strcat(filename, "/contour_q4.cg");
  if (!m_shader_q4->CompileFile(Shader::VP, filename, "vmain", NULL))
    return false;
  if (!m_shader_q4->CompileFile(Shader::FP, filename, "fmain", NULL))
    return false;
#else
  if (!m_shader_q4->CompileString(Shader::VP, TpvPolyContourShaderGetQ4(), "vmain", NULL))
    return false;
  if (!m_shader_q4->CompileString(Shader::FP, TpvPolyContourShaderGetQ4(), "fmain", NULL))
    return false;
#endif
  m_shader_q4->TrackMatrix(Shader::VP, "ModelViewProj", 
                        Shader::MODELVIEW_PROJECTION,
                        Shader::IDENTITY);
  m_shader_q4->TrackMatrix(Shader::VP, "ModelViewProjInvTransp", 
                        Shader::MODELVIEW_PROJECTION,
                        Shader::INVERSE_TRANSPOSE);
  m_shader_q4->TrackMatrix(Shader::VP, "TextureMatrix", 
                        Shader::TEXTURE,
                        Shader::IDENTITY);
  m_shader_q4->BindTexture("LUT", m_tex_obj);

  // Shader Q8
#ifdef TPV_COMPILE_SHADER_FILE
  strcpy(filename, m_shader_path);
  strcat(filename, "/contour_q8.cg");
  if (!m_shader_q8->CompileFile(Shader::VP, filename, "vmain", NULL))
    return false;
  if (!m_shader_q8->CompileFile(Shader::FP, filename, "fmain", NULL))
    return false;
#else
  if (!m_shader_q8->CompileString(Shader::VP, TpvPolyContourShaderGetQ8(), "vmain", NULL))
    return false;
  if (!m_shader_q8->CompileString(Shader::FP, TpvPolyContourShaderGetQ8(), "fmain", NULL))
    return false;
#endif
  m_shader_q8->TrackMatrix(Shader::VP, "ModelViewProj", 
                        Shader::MODELVIEW_PROJECTION,
                        Shader::IDENTITY);
  m_shader_q8->TrackMatrix(Shader::VP, "ModelViewProjInvTransp", 
                        Shader::MODELVIEW_PROJECTION,
                        Shader::INVERSE_TRANSPOSE);
  m_shader_q8->TrackMatrix(Shader::VP, "TextureMatrix", 
                        Shader::TEXTURE,
                        Shader::IDENTITY);
  m_shader_q8->BindTexture("LUT", m_tex_obj);

  m_initialized = true;
  return true;
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::LoadTexture()
{
  if (!m_colorscale)
    return;
  glPushAttrib(GL_TEXTURE_BIT | GL_ENABLE_BIT);      
  glColor3f(1,1,1);
  glBindTexture(GL_TEXTURE_1D, m_tex_obj);
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_TEXTURE);
  glPushMatrix();
  glLoadMatrixf(m_tex_matrix);
  glPopAttrib();

  glEnable(GL_TEXTURE_1D);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_TEXTURE_3D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::UnloadTexture()
{
  if (!m_colorscale)
    return;
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_TEXTURE);
  glPopMatrix();
  glPopAttrib();

  glPopAttrib();
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::DrawArray(int n, int dim, 
    float* pos, float* normal, float* scalar)
{
  glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
  if (pos) {
    glVertexPointer(3, GL_FLOAT, 0, pos);
    glEnableClientState(GL_VERTEX_ARRAY);
  }
  if (normal) {
    glNormalPointer(GL_FLOAT, 0, normal);
    glEnableClientState(GL_NORMAL_ARRAY);
  }
  if (scalar) {
    glTexCoordPointer(1, GL_FLOAT, 0, scalar);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  }
  if (dim == 0) // points
    glDrawArrays(GL_POINTS, 0, n);
  else if (dim == 1) // lines
    glDrawArrays(GL_LINES, 0, 2*n);
  else // triangles
    glDrawArrays(GL_TRIANGLES, 0, 3*n);
  glPopClientAttrib();
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::DrawArrayQ(int n,
    float* pos, float* normal, float* param, float* scalar, float* scalar2)
{
  glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
  if (pos) { // (x,y,z)
    glVertexPointer(3, GL_FLOAT, 0, pos);
    glEnableClientState(GL_VERTEX_ARRAY);
  }
  if (normal) { // (nx,ny,nz)
    glNormalPointer(GL_FLOAT, 0, normal);
    glEnableClientState(GL_NORMAL_ARRAY);
  }
  if (param) { // (r,s)
    glClientActiveTexture(GL_TEXTURE0);
    glTexCoordPointer(2, GL_FLOAT, 0, param);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  }
  if (scalar) { // (v0,v1,v2,v3)
    glClientActiveTexture(GL_TEXTURE1);
    glTexCoordPointer(4, GL_FLOAT, 0, scalar);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  }
  if (scalar2) { // (v4,v5,v6,v7)
    glClientActiveTexture(GL_TEXTURE2);
    glTexCoordPointer(4, GL_FLOAT, 0, scalar2);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  }
  glDrawArrays(GL_TRIANGLES, 0, 3*n);
  glPopClientAttrib();
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::RenderTriangles()
{
  for (typename TriList::Iterator it = m_tri_geoms.Begin(); it.Valid(); ++it) {
    TpvTriGeometry3<MODELCLASS>* g = it.Self();
    for (int step_id = m_base_id + m_nsteps; step_id > m_curr_step; step_id--) {
      TpvTriangleSet2* tri = g->GetTriangles(step_id);
      int n = tri->GetSize();
      float* pos = tri->GetPositions()->GetArray();
      float* normal = tri->GetNormals()->GetArray();
      float* scalar = tri->GetField()->GetArray();
      DrawArray(n, 2, pos, normal, scalar); 
    }
  }
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::RenderTrianglesQ4()
{
  for (typename TriList::Iterator it = m_tri_geoms.Begin(); it.Valid(); ++it) {
    TpvFaceToShaderTriGeometry<MODELCLASS>* g = it.Self();
    for (int step_id = m_base_id + m_nsteps; step_id > m_curr_step; step_id--) {
      TpvTriangleSet2* tri = g->GetTrianglesQ4(step_id);
      int n = tri->GetSize();
      float* pos = tri->GetPositions()->GetArray();
      float* normal = tri->GetNormals()->GetArray();
      float* param = tri->GetParams()->GetArray();
      float* scalar = tri->GetField()->GetArray();
      DrawArrayQ(n, pos, normal, param, scalar);
    }
  }
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::RenderTrianglesQ8()
{
  for (typename TriList::Iterator it = m_tri_geoms.Begin(); it.Valid(); ++it) {
    TpvFaceToShaderTriGeometry<MODELCLASS>* g = it.Self();
    for (int step_id = m_base_id + m_nsteps; step_id > m_curr_step; step_id--) {
      TpvTriangleSet2* tri = g->GetTrianglesQ8(step_id);
      int n = tri->GetSize();
      float* pos = tri->GetPositions()->GetArray();
      float* normal = tri->GetNormals()->GetArray();
      float* param = tri->GetParams()->GetArray();
      float* scalar = tri->GetField()->GetArray();
      float* scalar2 = tri->GetField2()->GetArray();
      DrawArrayQ(n, pos, normal, param, scalar, scalar2);
    }
  }
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::RenderLines()
{
  for (typename LineList::Iterator it = m_line_geoms.Begin(); it.Valid(); ++it) {
    TpvLineGeometry3<MODELCLASS>* g = it.Self();
    for (int step_id = m_base_id + m_nsteps; step_id > m_curr_step; step_id--) {
      TpvLineSet2* lin = g->GetLines(step_id);
      int n = lin->GetSize();
      float* pos = lin->GetPositions()->GetArray();
      float* scalar = lin->GetField()->GetArray();
      DrawArray(n, 1, pos, NULL, scalar); 
    }
  }
}

MODEL_CLASS_TEMPLATE
bool TpvPolyContourShaderRenderer3<MODELCLASS>::Update()
{
  if (!Init())
    return false;
  if (m_update_geometry)
    m_update_property = true;
  if (m_update_property) {
//TODO: assumes that there is only one property beeing visualized.
    for (typename TriList::Iterator it = m_tri_geoms.Begin(); it.Valid(); ++it)
      it.Self()->SetField(m_property);
    for (typename LineList::Iterator it = m_line_geoms.Begin(); it.Valid(); ++it)
      it.Self()->SetField(m_property);
  }
  if (m_update_colorscale && m_colorscale) {
    if (m_property) {
      //float smin = m_property->GetMinValue();
      //float smax = m_property->GetMaxValue();
      //int ncolors = m_colorscale->GetNumberOfColors();
      int nvalues = m_colorscale->GetNumberOfValues();
      float smin = m_colorscale ? m_colorscale->GetValue(0) : 0.0f;
      float smax = m_colorscale ? m_colorscale->GetValue(nvalues - 1) : 1.0f;
      m_tex_matrix[0] = 1.0f/(smax-smin);
      m_tex_matrix[12] = -smin/(smax-smin);
    } else {
      m_tex_matrix[0] = m_tex_matrix[12] = 1.0f;
    }
    glPushAttrib(GL_TEXTURE_BIT | GL_ENABLE_BIT);
    glBindTexture(GL_TEXTURE_1D, m_tex_obj);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, m_tex_w,
                  0, GL_RGBA, GL_FLOAT, m_tex_data);
    glPopAttrib();
  }
  for (typename TriList::Iterator it = m_tri_geoms.Begin(); it.Valid(); ++it)
    it.Self()->Update();
  for (typename LineList::Iterator it = m_line_geoms.Begin(); it.Valid(); ++it)
    it.Self()->Update();
  
  m_update_geometry = false;
  m_update_property = false;
  m_update_colorscale = false;
  return true;
}

/************
  public
*************/
//constructor
MODEL_CLASS_TEMPLATE
TpvPolyContourShaderRenderer3<MODELCLASS>::TpvPolyContourShaderRenderer3()
  : m_model(NULL), m_property(NULL), m_colorscale(NULL),
  m_base_id(1), m_nsteps(1), m_curr_step(1),
  m_update_geometry(true), m_update_property(true), m_update_colorscale(true),
  m_initialized(false),
  m_clip_plane_enabled(false),
  m_poly_offset_factor(0), m_poly_offset_units(0), m_poly_offset_enabled(false),
  m_color_mask_enabled(true),
  m_render_triangles(true), m_render_lines(true), m_render_points(true),
  m_tex_obj(0), m_tex_w(1024), m_tex_data(NULL),
  m_cs_tex_obj(0),
  m_shader_q4(new CGShader()), m_shader_q8(new CGShader())
{
  InitExtensions();
  m_clip_plane[0] = 0.0f;
  m_clip_plane[1] = 0.0f;
  m_clip_plane[2] = 1.0f;
  m_clip_plane[3] = 0.0f;
  glGenTextures(1, &m_tex_obj);  
  // Identity matrix.
  for (int i = 0; i < 16; i++)
    m_tex_matrix[i] = 0.0f;
  m_tex_matrix[0] = m_tex_matrix[5] = m_tex_matrix[10] = m_tex_matrix[15] = 1.0f;
  strcpy(m_shader_path, ".");
}

MODEL_CLASS_TEMPLATE
TpvPolyContourShaderRenderer3<MODELCLASS>::~TpvPolyContourShaderRenderer3()
{
  glDeleteTextures(1, &m_tex_obj);  
  free(m_tex_data);
  delete m_shader_q4;
  delete m_shader_q8;
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::SetShaderPath(const char* path)
{
  strncpy(m_shader_path, path, 128);
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::AddTriGeometry(TpvFaceToShaderTriGeometry<MODELCLASS>* g)
{
  if (g) {
    m_tri_geoms.Append(g);
    m_update_geometry = true;
  }
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::ClearTriGeometries()
{
  m_tri_geoms.Clear();
  m_update_geometry = true;
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::AddLineGeometry(TpvLineGeometry3<MODELCLASS>* g)
{
  if (g) {
    m_line_geoms.Append(g);
    m_update_geometry = true;
  }
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::ClearLineGeometries()
{
  m_line_geoms.Clear();
  m_update_geometry = true;
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::SetProperty(TpvProperty* property)
{
  m_property = property;
  m_update_property = true;
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::SetColorScale(TpvColorScale* colorscale)
{
  m_colorscale = colorscale;
  free(m_tex_data);
  m_tex_data = NULL;
  if (colorscale) {
    colorscale->CreateTexArray(m_tex_w, &m_tex_data);
#ifdef VERBOSE
  printf("Texture: %d\n", m_tex_w);
  for (int i = 0; i < m_tex_w; i++)
    printf("[%5d]: %+6.3e %+6.3e %+6.3e %+6.3e\n", 
      i, m_tex_data[4*i], m_tex_data[4*i+1], m_tex_data[4*i+2], m_tex_data[4*i+3]);
#endif
  }
  m_update_colorscale = true;
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::SetColorScaleTexObj(int tex_obj)
{
  m_cs_tex_obj = tex_obj;
  m_shader_q4->TrackMatrix(Shader::VP, "TextureMatrix", 
                        Shader::TEXTURE,
                        Shader::IDENTITY);
  m_shader_q4->BindTexture("LUT", m_cs_tex_obj);
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::SetBaseId(int id)
{
  m_base_id = id;
}

MODEL_CLASS_TEMPLATE
int TpvPolyContourShaderRenderer3<MODELCLASS>::GetBaseId()
{
  return m_base_id;
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::SetNumOfSteps(int n)
{
  m_nsteps = n;
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::SetCurrentStep(int step_id)
{
  m_curr_step = step_id;
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::SetClippingPlane(float a, float b, float c, float d)
{
  m_clip_plane[0] = a; m_clip_plane[1] = b; m_clip_plane[2] = c; m_clip_plane[3] = d;
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::SetClippingPlaneEnabled(bool flag)
{
  m_clip_plane_enabled = flag;
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::SetPolygonOffset(float factor, float units)
{
  m_poly_offset_factor = factor;
  m_poly_offset_units = units;
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::SetPolygonOffsetEnabled(bool flag)
{
  m_poly_offset_enabled = flag;
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::SetColorMaskEnabled(bool flag)
{
  m_color_mask_enabled = flag;
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::SetRenderTriangles(bool flag)
{
  m_render_triangles = flag;
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::SetRenderLines(bool flag)
{
  m_render_lines = flag;
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::SetRenderPoints(bool flag)
{
  m_render_points = flag;
}

MODEL_CLASS_TEMPLATE
void TpvPolyContourShaderRenderer3<MODELCLASS>::Render()
{
  if (!Update())
    return;

  LoadTexture();
  glPushAttrib(GL_ENABLE_BIT | GL_TRANSFORM_BIT |  // clip plane
    GL_CURRENT_BIT | GL_LIGHTING_BIT | 
    GL_POLYGON_BIT | GL_LINE_BIT | GL_POINT_BIT);

  glEnable(GL_DEPTH_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  if (m_clip_plane_enabled) {
    double plane[4] = {m_clip_plane[0], m_clip_plane[1], m_clip_plane[2], m_clip_plane[3]};
    glClipPlane(GL_CLIP_PLANE0, plane);
    glEnable(GL_CLIP_PLANE0);
  }
  if (!m_color_mask_enabled) {
    glPushAttrib(GL_COLOR_BUFFER_BIT);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  }

  bool render_triangles = m_render_triangles && m_tri_geoms.Size() > 0;
  bool render_lines = m_render_lines && m_line_geoms.Size() > 0;
  if (render_triangles) {
    if (m_poly_offset_enabled) {
      if (render_lines)
        glPolygonOffset(m_poly_offset_factor, m_poly_offset_units+1);
      else
        glPolygonOffset(m_poly_offset_factor, m_poly_offset_units);
      glEnable(GL_POLYGON_OFFSET_FILL);
    } else if (render_lines) {
      glPolygonOffset(1, 1);
      glEnable(GL_POLYGON_OFFSET_FILL);
    }
    RenderTriangles();

    // Q4
    AlgVector eye = VglFrustum().GetEyePos();
    m_shader_q4->SetConstant(Shader::VP, "LightPosition", eye.x, eye.y, eye.z);
    m_shader_q4->SetConstant(Shader::VP, "LightingEnabled", 1);    
//TODO: conditional shader.
    if (m_clip_plane_enabled)
      m_shader_q4->SetConstant(Shader::FP, "ClipPlane", 
          m_clip_plane[0], m_clip_plane[1], m_clip_plane[2], m_clip_plane[3]);
    else
      m_shader_q4->SetConstant(Shader::FP, "ClipPlane", 
          0.0f, 0.0f, 1.0f, 1e23f);
    if (m_cs_tex_obj)
      m_shader_q4->BindTexture("LUT", m_cs_tex_obj);
    else
      m_shader_q4->BindTexture("LUT", m_tex_obj);
    m_shader_q4->Load();
    RenderTrianglesQ4();
    m_shader_q4->Unload();

    // Q8
    m_shader_q8->SetConstant(Shader::VP, "LightPosition", eye.x, eye.y, eye.z);
    m_shader_q8->SetConstant(Shader::VP, "LightingEnabled", 1);
    if (m_cs_tex_obj)
      m_shader_q8->BindTexture("LUT", m_cs_tex_obj);
    else
      m_shader_q8->BindTexture("LUT", m_tex_obj);
//TODO: conditional shader.
    if (m_clip_plane_enabled)
      m_shader_q8->SetConstant(Shader::FP, "ClipPlane", 
          m_clip_plane[0], m_clip_plane[1], m_clip_plane[2], m_clip_plane[3]);
    else
      m_shader_q8->SetConstant(Shader::FP, "ClipPlane", 
          0.0f, 0.0f, 1.0f, 1e23f);
    m_shader_q8->Load();
    RenderTrianglesQ8();
    m_shader_q8->Unload();
  }
  if (render_lines) {
    if (m_poly_offset_enabled) {
      glPolygonOffset(m_poly_offset_factor, m_poly_offset_units);
      glEnable(GL_POLYGON_OFFSET_LINE);
    }
    glDisable(GL_LIGHTING);
    glLineWidth(5);
    RenderLines();
  }

  if (!m_color_mask_enabled)
    glPopAttrib();

  glPopAttrib();
  UnloadTexture();
}

