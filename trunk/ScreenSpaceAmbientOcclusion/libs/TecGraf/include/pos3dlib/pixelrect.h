//* pixelrect.h
// favera@tecgraf.puc-rio.br
// Tecgraf/PUC-Rio
// Mar 2010

#ifndef P3D_PIXEL_RECT_H
#define P3D_PIXEL_RECT_H

#include <assert.h>
#include <pos3dlib/defines.h>

/**
 * class P3DPixelRect
 * Rectangle of pixels from some visual area 
 */
class POS3DLIB_API P3DPixelRect
{
public:
  P3DPixelRect (int x, int y, int width, int height);

  ~P3DPixelRect ();

  int GetWidth () const;

  int GetHeight () const;

  int GetX () const;
  int GetY () const;


  unsigned int * GetPixelData () const;

   /************************************************************************
   * SetPixel
   *  Set the pixel at line l and column c with the value of pix_data
   ***********************************************************************/ 
  inline void SetPixel (unsigned int pix_data, int l, int c)
  {
    SetPixel(pix_data, l*m_width+c);
  }

  /************************************************************************
   * SetPixel
   *  Set the pixel at pos with the value of pix_data
   ***********************************************************************/ 
  inline void SetPixel (unsigned int pix_data, int pos)
  {
    assert(m_pixeldata);
    assert(pos < m_width*m_height);
    m_pixeldata[pos] = pix_data;
  }

 private:
  int m_width;
  int m_height;

  int m_x;
  int m_y;
  
  unsigned int *m_pixeldata;
};


#endif