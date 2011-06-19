/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2011
 *
 *  Combine two 2D Texture.
 *  Receives two 2D texture as input, blend the result
 */
uniform sampler2D colorTex; /**< First input texture */
uniform sampler2D ssaoTex;  /**< Second input texture */

void main()
{
  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_Position = ftransform();
}
