/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/
#pragma once

#include <vector>
#include <string>

#include "GraphBasis/Vector3.h"
#include "GraphBasis/VertexBufferObject.h"

using namespace std;


class PreProcessor
{
  protected:
      VertexBufferObject * m_vbo;
   public:
	    PreProcessor(VertexBufferObject * vbo);
      ~PreProcessor();

protected:
      void calcTextures();
};

