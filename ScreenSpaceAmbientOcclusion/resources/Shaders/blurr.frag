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
  //Read the current pixel 
  vec4 input = texture2D(inputTex,  gl_TexCoord[0].st);
  
  // Do nothing if the alpha value is negative
	if(input.a < 0.0)
	{
		gl_FragData[0] = vec4(.8, .8, 1.0, -1.0);
		return;
	}
	
	//Initialize the result
  vec4 sum = vec4(0,0,0,0);

  //Calculate the screen width/height rate  
  float dx = 1.0/screenWidth;
  float dy = 1.0/screenHeight;

  //Define the screen size area of the neighborhood to be visited
  int size = int(floor(pixelmask_size));
  float kernel_size = float(size)*2. + 1.;

  //Weight of each pixel added in the blur prosses
  float weight = intensity/(kernel_size*kernel_size);
  
  //Access the neighborhood
  int ssize = size*int(offsets_size);
  for(int i=-ssize; i < ssize + 1; i+=int(offsets_size))
    for(int j = -ssize; j < ssize + 1; j+=int(offsets_size))
    {
      vec2 inc = vec2(float(i)*dx, float(j)*dy);
      vec4 neigh = texture2D(inputTex,  gl_TexCoord[0].st + inc); 
      sum += neigh * weight;
    }
  
  //Output 
  gl_FragData[0] = sum;
}
