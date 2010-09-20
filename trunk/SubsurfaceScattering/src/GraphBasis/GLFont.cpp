#include <stdio.h>
#include <stdarg.h>
#include "main.h"

#include "GraphBasis\GLFont.h"


int GLFont :: charHieght =  18;

void GLFont::initText()
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
   color.setColor();
	for (const char *c= str.c_str(); *c != '\0'; c++)
   {
		glRasterPos2f(x,y);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
		x = x + glutBitmapWidth(GLUT_BITMAP_HELVETICA_18,*c);
	}
}

int  GLFont :: length(string s)
{
   return glutBitmapLength(GLUT_BITMAP_HELVETICA_18,(const unsigned char*)s.c_str());
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
