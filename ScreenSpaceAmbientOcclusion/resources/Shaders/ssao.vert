/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2011
 *
 *  Perform the calculation of the Screen Space Ambient Occlusion.
 *  Receive projection matrix information and reprojects the pixels obtained from the depth
 *  input textures and does the calculation of the occlusion that the neighborhood of a pixel 
 *  causes using spheres as occludes approximation.
 */

void main()
{
  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_Position = ftransform();
}
