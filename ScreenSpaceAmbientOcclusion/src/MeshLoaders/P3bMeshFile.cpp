/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2011
 *
 *  P3B Model Loader. 
 *  Imports a binary mesh file that load a FiniteElement Model used 
 *  in TecGraf's Pos3D.
 *  Permits the render of this model.
 *  Depends on diverse P3D libs.
 */
#include <limits>
#include "MeshLoaders/P3bMeshFile.h"

/**
 * TECGRAF LIBS
 */
#include "pos3dlib/model/model.h"
#include "pos3dlib/io/nfrmodelreader.h"
#include "pos3dlib/display/fem.h"
#include "pos3dlib/elemvis.h"
#include "pos3dlib/elemselection.h"

#include "defines.h"

/**
 * P3D File Types
 */
#define P3D_SUB_TYPE_LUA ".p3i"
string P3bMeshFile :: s_fileType = string(".p3b");

P3bMeshFile::P3bMeshFile(void)
:m_p3dModel(NULL)
,m_p3dModelReader(NULL)
,m_p3dRenderer(NULL)
,m_elemvis(NULL)
,m_elemsel(NULL)
,m_bb_size(Vector3(-1, -1, -1))
,m_bb_center(Vector3(0, 0, 0))
,m_bb_min(Vector3(numeric_limits<float>::infinity( ), numeric_limits<float>::infinity( ), numeric_limits<float>::infinity( )))
,m_bb_max(Vector3(-numeric_limits<float>::infinity( ),-numeric_limits<float>::infinity( ),-numeric_limits<float>::infinity( )))
{
}

P3bMeshFile::~P3bMeshFile(void)
{
  if(m_p3dModel)
    delete m_p3dModel;
  if(m_p3dModelReader)
    delete m_p3dModelReader;
  if(m_elemvis)
    delete m_elemvis;
  if(m_p3dRenderer)
    delete m_p3dRenderer;
  if(m_elemsel)
    delete m_elemsel;
}

void P3bMeshFile::readFile(string fileName, Vector3 pos /*= Vector3(0,0,0)*/, Vector3 scale /*= Vector3(1,1,1)*/ )
{
  m_pos = pos;
  m_scale = scale;
  m_fileName = fileName;
  string fileNoType = fileName.substr(0, fileName.rfind(s_fileType));

  if(m_p3dModel)
    delete m_p3dModel;
  m_p3dModel = new P3DModel();

  if(m_p3dModelReader)
    delete m_p3dModelReader;
  m_p3dModelReader = new P3DNfrModelReader(m_p3dModel);

  m_p3dModelReader->SetFilename((fileNoType + s_fileType).c_str(), (fileNoType + string(P3D_SUB_TYPE_LUA)).c_str());

  if(m_p3dModelReader->Open() == 0)
  {
    MyAssert("CAN'T OPEN FILE:" + string(fileNoType + s_fileType) + " " + string(fileNoType + P3D_SUB_TYPE_LUA), false);
    delete m_p3dModelReader;
    delete m_p3dModel;
  }

  if(m_p3dModelReader->Read() == 0)
  {
    MyAssert("CAN'T READ FILE:" + string() + " " + string() + "\n" + m_p3dModelReader->GetLastError(), true);
    delete m_p3dModelReader;
    delete m_p3dModel;
  }

  if(m_elemvis)
    delete m_elemvis;
  m_elemvis = new P3DElemVis();
  m_elemvis->SetModel(m_p3dModel);

  if(m_elemsel)
    delete m_elemsel;
  m_elemsel = new P3DElemSelection();
  m_elemsel->SetModel(m_p3dModel);
  m_elemsel->SetBaseGlobalId(1);


  if(m_p3dRenderer)
    delete m_p3dRenderer;
  m_p3dRenderer = new P3DFemDisplay();
  m_p3dRenderer->SetModel(m_p3dModel);


  m_p3dModel->GetBoundingBox(&m_bb_min.x, &m_bb_max.x,
                             &m_bb_min.y, &m_bb_max.y,
                             &m_bb_min.z, &m_bb_max.z);
  m_bb_size = m_bb_max - m_bb_min;
  m_bb_center = (m_bb_max + m_bb_min)*.5;
}



int P3bMeshFile::getNumElements()
{
  return  m_p3dModel->GetNumOfElements();
}

void P3bMeshFile::setVisibleElements( bool *v )
{
  m_elemvis->SetAllElements(false);
  m_elemsel->AddSelectElems(v);
  m_elemvis->SetSelection(m_elemsel);
  m_p3dRenderer->SetElemVis(m_elemvis);
}

void P3bMeshFile::render()
{
  glPushMatrix();
    glTranslatef(m_pos.x, m_pos.y, m_pos.z);
    glScalef(m_scale.x, m_scale.y, m_scale.z);
    glPushAttrib(GL_CURRENT_BIT|GL_ENABLE_BIT);
      glEnable(GL_NORMALIZE);
      glColor3f(1,1,1);
      m_p3dRenderer->Render();
    glPopAttrib();
  glPopMatrix();
}
bool P3bMeshFile::isValidFileType( string filetype )
{
  return strcmp(s_fileType.c_str(), strlwr((char*)filetype.c_str())) == 0;
}


Vector3 P3bMeshFile::getBoundingBoxSize() const
{
  return m_bb_size;
}

Vector3 P3bMeshFile::getBoundingBoxCenter() const
{
  return m_bb_center;
}

Vector3 P3bMeshFile::getBoundingBoxMin() const
{
  return m_bb_min;
}

Vector3 P3bMeshFile::getBoundingBoxMax() const
{
  return m_bb_max;
}

