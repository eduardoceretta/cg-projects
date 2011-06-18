// GLFont.h: interface for the GLFont class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include <GL/glut.h>
#include <GL/gl.h>

#include <string>
#include "GraphBasis\Color.h"

using namespace std;

class GLFont
{
public:
  typedef enum{Small, Medium, Big} FontSize;

  static int charHieght;
	static void print(int x, int y, string str,Color c=Color(1,1,0));
	static void endText();
	static void initText();
  static void setSize(FontSize s);
	static int length(string s);
	GLFont(){}
protected:
	static GLboolean color_material, texture_2d, fog, lighting, depth_test;
  static FontSize fontSize;
  static void* font;
  static void* fonts[];
  static int font_sizes[]; 
};


