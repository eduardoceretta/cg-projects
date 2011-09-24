/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Sep 2011
 *
 *  Perform the calculation of the Ambient Occlusion of the Scene.
 *  Uses a non regular grid as occlusion query.
 */


void main()
{
  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_Position = ftransform();
}
