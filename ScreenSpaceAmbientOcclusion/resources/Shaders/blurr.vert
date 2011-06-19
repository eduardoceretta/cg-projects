/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2011
 *
 *  Blurs a 2D Texture.
 *  Receives a 2D texture as input, applies a blur around each pixel 
 *  and writes the ouput in the frame buffer.
 */
uniform float screenWidth;    
uniform float screenHeight; 

uniform sampler2D inputTex;   /**< Input 2D Texture */
uniform float pixelmask_size; /**< Defines the size of the blurred area */

uniform float offsets_size;   /**< Adds an offset between each pixel read in the pixel texture */
uniform float intensity;      /**< Is multiplied by the final color of the pixel */

void main()
{
  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_Position = ftransform();
}
