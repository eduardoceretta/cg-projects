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
  //Read textures
  vec4 color = texture2D(colorTex,  gl_TexCoord[0].st);
  vec4 ssao = texture2D(ssaoTex,  gl_TexCoord[0].st);
  
  // Do nothing if any alpha value is negative
  if(ssao.a < 0.0 || color.a < 0.0)
	{
		gl_FragData[0] = vec4(.8, .8, 1.0, -1.0);
		return;
	}
  
  //Output
  gl_FragData[0] = color*ssao;
}
