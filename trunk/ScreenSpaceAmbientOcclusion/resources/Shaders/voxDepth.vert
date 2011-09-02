/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Sep 2011
 *
 *  Get the eye nearest fragment
 */
varying vec4 color;
varying vec3 eyePos;
varying vec3 normal;


void main()
{
  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_Position = ftransform();
  
  color = gl_Color;
  vec4 eyeP = gl_ModelViewMatrix*gl_Vertex;
  eyePos = eyeP.xyz/eyeP.w;
  normal = gl_NormalMatrix*gl_Normal;
}
