/**********************************************************\
            Nome:Eduardo Ceretta Dalla Favera
\**********************************************************/

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "main.h"
#include "PreProcess/PreProcessor.h"

using namespace std;

///////////////////
//~ PreProcessor
//////////////////
PreProcessor :: PreProcessor(VertexBufferObject* vbo)
:m_vbo(vbo)
{
}

PreProcessor :: ~PreProcessor()
{
}

