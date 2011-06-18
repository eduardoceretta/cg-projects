/**
 *	Eduardo Ceretta Dalla Favera with modifications of Cesar Tadeu Pozzer
 *  eduardo.ceretta@gmail.com, pozzer3@gmail.com
 *  Mar 2010  
 *
 *  OpenGL Font utility class.
 */
#include <stdio.h>
#include <stdarg.h>

#include "GLFont.h"


void* GLFont :: fonts [] = {
  GLUT_BITMAP_HELVETICA_10
  ,GLUT_BITMAP_HELVETICA_12
  ,GLUT_BITMAP_HELVETICA_18
};

int GLFont :: font_sizes[] = {
  10
  ,12
  ,18
}; 

void* GLFont :: font = fonts[1];
int GLFont :: charHeight = font_sizes[1];

void GLFont::initText(int appWidth, int appHeight)
{
   glPushMatrix();
   glPushAttrib(GL_ENABLE_BIT);

   glDisable(GL_COLOR_MATERIAL);
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_FOG);
   glDisable(GL_LIGHTING);
   glDisable(GL_DEPTH_TEST);
   glDisable(GL_BLEND);

   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   gluOrtho2D(0, appWidth, appHeight, 0);

   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();
}

void GLFont::print(int x, int y, string str,Color color)
{
  glColor3f(color.r, color.g, color.b);
	for (const char *c= str.c_str(); *c != '\0'; c++)
   {
		glRasterPos2f(x,y);
		glutBitmapCharacter(font, *c);
		x = x + glutBitmapWidth(font,*c);
	}
}

int  GLFont :: length(string s)
{
   return glutBitmapLength(font,(const unsigned char*)s.c_str());
}

void GLFont::endText()
{
   glPopMatrix();

   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   
   glMatrixMode(GL_MODELVIEW);
   glPopAttrib();
   glPopMatrix();
}

void GLFont::setSize( FontSize s )
{
  switch(s)
  {
    case Small:
      font = fonts[0];
      charHeight = font_sizes[0];
      break;
    case Medium:
      font = fonts[1];
      charHeight = font_sizes[1];
      break;
    case Big:
      font = fonts[2];
      charHeight = font_sizes[2];
      break;
    }
}
