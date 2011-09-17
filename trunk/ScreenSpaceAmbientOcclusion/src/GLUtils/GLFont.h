/**
 *	Eduardo Ceretta Dalla Favera with modifications of Cesar Tadeu Pozzer
 *  eduardo.ceretta@gmail.com, pozzer3@gmail.com
 *  Mar 2010  
 *
 *  OpenGL Font utility class.
 */

#pragma once
#include <GL/glut.h>
#include <GL/gl.h>

#include "MathUtils/Color.h"

#include <string>

using namespace std;

class GLFont
{
public:
  /**
   * Enumerate the possible sizes for the font
   */
  typedef enum{Small, Medium, Big} FontSize;


  /**
   * Print the string str with the color c in the position x, y of the screen.
   *  Must came between initText and endText
   */
  static void print(int x, int y, string str,Color c=Color(1,1,0));

  /**
   * Setup the initial OpenGL context to draw the font
   */
  static void initText(int appWidth, int appHeight);
  
  /**
   * Restores the OpenGL context after the font was drew 
   */
  static void endText();

  /**
   * Set the size of the font
   */
  static void setSize(FontSize s);

  /**
   * Get the string size in pixels
   */
  static int length(string s);

  GLFont();

protected:
  /**
   * OpenGL context variables
   */
	static GLboolean s_color_material, s_texture_2d, s_fog, s_lighting, s_depth_test;
  
  /**
   * Character Height
   */
  static int s_charHeight;
  
  /**
   * Current font size
   */
  static FontSize fontSize;

  /**
   * Current Glut Bitmap data
   */
  static void* s_font;

  /**
   * Defined Glut Bitmap data
   */
  static void* s_fonts[3];
  
  /**
   * Defined Glut Bitmap font sizes
   */
  static int s_font_sizes[3]; 
};


