/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2010
 *
 *  OpenGL Material Attributes Container
 */
#include <iostream>
#include "assert.h"

#include "ScScene/ScMaterial.h"

using namespace std;

int ScMaterial :: s_materialNum = 0;


ScMaterial :: ScMaterial()
:m_calculed(false)
{
   m_myMaterialNum = s_materialNum++;
}


int ScMaterial :: getMyMaterialNumber() const
{
   return m_myMaterialNum;
}

void ScMaterial :: readFromStr(char buffer[])
{
   int r = sscanf( buffer, "%f %f %f %f %f %f %f %f %f %f %*s\n",
     &m_diffuse.r, &m_diffuse.g, &m_diffuse.b, &m_specular.r, &m_specular.g, &m_specular.b, &m_specularExp, &m_reflective, &m_refractive, &m_opacity);
   m_diffuse *= 1.0/255;
   m_specular *= 1.0/255;
   assert(r == 11 - 1);
}

void ScMaterial :: configure()
{
   if(!m_calculed)
   {
      m_gAmbient[0] = m_diffuse.r*.2;
      m_gAmbient[1] = m_diffuse.g*.2;
      m_gAmbient[2] = m_diffuse.b*.2;
      m_gAmbient[3] = 1.0;

      m_gDiffuse[0] = m_diffuse.r;
      m_gDiffuse[1] = m_diffuse.g;
      m_gDiffuse[2] = m_diffuse.b;
      m_gDiffuse[3] = 1.0;

      m_gSpecular[0] = m_specular.r;
      m_gSpecular[1] = m_specular.g;
      m_gSpecular[2] = m_specular.b;
      m_gSpecular[3] = 1.0;

      m_gShi = m_specularExp;
      m_calculed = true;
   }
}

void ScMaterial :: render()
{
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  m_gAmbient);
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  m_gDiffuse);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_gSpecular);
   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_gShi);
}
