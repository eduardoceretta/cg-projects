/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Jun 2011
 *
 *  Perform the calculation of the Screen Space Ambient Occlusion.
 *  Receive projection matrix information and reprojects the pixels obtained from the depth
 *  input textures and does visibility tests with its surrounds
 */

void main()
{
  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_Position = ftransform();
}
