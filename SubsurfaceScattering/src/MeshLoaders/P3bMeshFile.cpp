/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#include "MeshLoaders/P3bMeshFile.h"

//TECGRAF LIBS
#include "pos3dlib/model/model.h"
#include "pos3dlib/io/nfrmodelreader.h"
#include "pos3dlib/display/fem.h"
#include "pos3dlib/elemvis.h"
#include "pos3dlib/elemselection.h"

#include "main.h"

#define P3D_SUB_TYPE_LUA ".p3i"

P3bMeshFile::P3bMeshFile(void)
:m_p3dModel(NULL)
,m_p3dModelReader(NULL)
,m_p3dRenderer(NULL)
,m_elemvis(NULL)
,m_elemsel(NULL)
,m_fileType(".p3b")
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
  string fileNoType = fileName.substr(0, fileName.rfind(m_fileType));

  if(m_p3dModel)
    delete m_p3dModel;
  m_p3dModel = new P3DModel();

  if(m_p3dModelReader)
    delete m_p3dModelReader;
  m_p3dModelReader = new P3DNfrModelReader(m_p3dModel);

  m_p3dModelReader->SetFilename((fileNoType + m_fileType).c_str(), (fileNoType + string(P3D_SUB_TYPE_LUA)).c_str());

  if(m_p3dModelReader->Open() == 0)
  {
    MyAssert("CAN'T OPEN FILE:" + string(fileNoType + m_fileType) + " " + string(fileNoType + P3D_SUB_TYPE_LUA), false);
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
  //m_p3dRenderer->SetColorScale(m_sci_colorscale);
  //m_p3dRenderer->SetBoundaryMeshColor(r, g, b, 1);
  //m_p3dRenderer->SetBoundaryMeshWidth((float) i);
  //m_p3dRenderer->SetBoundaryCohMeshColor(r, g, b, 1);
  //m_p3dRenderer->SetBoundaryCohMeshWidth((float) i);
  //m_p3dRenderer->SetBoundaryNodesEnabled(flag);
  //m_p3dRenderer->SetBoundaryNodesColor(r, g, b, 1);
  //m_p3dRenderer->SetBoundaryCohNodesColor(r, g, b, 1);
  //m_p3dRenderer->SetOutlineEnabled(flag);
  //m_p3dRenderer->SetMaterialOutlineEnabled(flag);
  //m_p3dRenderer->SetBoundaryElemNumbersEnabled(flag);
  //m_p3dRenderer->SetBoundaryNodeNumbersEnabled(flag);
  //m_p3dRenderer->SetDispFactor(factor);
  //m_p3dRenderer->SetUndeformedEnabled(flag);
  //m_p3dRenderer->SetMaterialColorScale(m_sci_matcolorscale);
  //m_p3dRenderer->SetBoundaryElemColor(r, g, b, 1.f);
  //m_p3dRenderer->SetBoundaryCohElemColor(r, g, b, 1.f);
  //m_p3dModel* p3d_model = m_model->GetModel();
  //TopModel* mesh = p3d_model->GetMesh();
  //m_p3dRenderer->SetBoundaryPerElemColors(mesh->GetNElem(), colors_rgba);
  //m_p3dRenderer->SetSelectionMode(P3DFemDisplay::SELECT_NODE);
  //m_p3dRenderer->SetSelectionMode(P3DFemDisplay::SELECT_ELEMENT);
  //m_p3dRenderer->SetShaderPath(path);

  //m_p3dRenderer->SetViewport(x0, y0, w, h);
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
    glPushAttrib(GL_CURRENT_BIT);
      glColor3f(1,1,1);
      m_p3dRenderer->Render();
    glPopAttrib();
  glPopMatrix();
}
bool P3bMeshFile::isValidFileType( string filetype )
{
  return strcmp(m_fileType.c_str(), strlwr((char*)filetype.c_str())) == 0;
}