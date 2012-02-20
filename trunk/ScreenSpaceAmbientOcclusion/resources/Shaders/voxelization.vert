/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Aug 2011
 *
 *  Perform the calculation of the Voxelization of the scene
 *  Create a non regular grid and voxelizes the scene into it. Uses blending to make a bitmask that defines the grid.
 */
//varying vec4 color;
//varying vec4 worldPos;
varying vec3 eyePos;
//varying vec3 normal;


void main()
{
  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_Position = ftransform();
  
  //color = gl_Color;
  vec4 eyeP = gl_ModelViewMatrix*gl_Vertex;
  eyePos = eyeP.xyz/eyeP.w;
  //worldPos = gl_Vertex;
  //normal = gl_NormalMatrix*gl_Normal;
}
