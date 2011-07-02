/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2011
 *
 *  Does a deffered shading and a depth peeling of the scene.
 *  Peels the geometry and writes the render targets.
 */

/**
 * Depth of the scene.
 * Is compared with the current depth in the peeling process
 */
uniform sampler2D dephtTex;   

varying vec4 pos;             /**< Eye space point position */
varying vec3 normal;          /**< Eye space point normal */

uniform float screenWidth; 
uniform float screenHeight;

void main()
{
  //Determine the normalized coordened of the scren fragment position
  vec2 texCoord = (gl_FragCoord.xy)/vec2(screenWidth, screenHeight);
  
  //Read the depth texture 
  float depth = texture2D(dephtTex, texCoord).a;

	//Discard if the depth is less then the current depth
	if(depth < 0. || gl_FragCoord.z <= depth)
	  discard;

  //Write the render targets
  //Eye position
	gl_FragData[0] = pos;
	 
	//Normal And Depth
	gl_FragData[1] = vec4(normalize(normal), gl_FragCoord.z);
	
	//Diffuse Color
	gl_FragData[2] = gl_FrontMaterial.diffuse;
	
	/**
	 * TEST PEEL
	 *  Print the Iluminated Peel.
	 *  Result must the model withou the first layer
   * /
    gl_FragData[0] = vec4(vec3(max( dot(normalize(normal), -normalize(pos)) , 0.)),1);
    return;
  /**/
}

