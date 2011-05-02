/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#pragma once
#include <string>
#include "GraphBasis/Vector3.h"
using namespace std;

class P3DFemDisplay;
class P3DModel;
class P3DNfrModelReader;
class P3DElemSelection;
class P3DElemVis;

class P3bMeshFile
{
  string m_fileName;

  string m_fileType;
  Vector3 m_pos;
  Vector3 m_scale;

  P3DModel *m_p3dModel;
  P3DNfrModelReader *m_p3dModelReader;
  P3DFemDisplay* m_p3dRenderer;

  P3DElemVis *m_elemvis;
  P3DElemSelection *m_elemsel;
public:
  P3bMeshFile(void);
  ~P3bMeshFile(void);

  int getNumElements();
  void setVisibleElements(bool *v);

  void readFile(string fileName, Vector3 pos = Vector3(0,0,0), Vector3 scale = Vector3(1,1,1));

  void render();
  bool isValidFileType( string filetype );
protected:
};


