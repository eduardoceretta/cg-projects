/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Aug 2011
 *
 *  Perform the calculation of the Voxelization of the scene
 *  Create a non regular grid and voxelizes the scene into it. Uses blending to make a bitmask that defines the grid.
 */
#extension GL_EXT_gpu_shader4 : enable //Enable Integer Operations - Shader Model 4.0 GF 8 Series

/****************************************************************************************/
/* Shader Controls.                                                                     */
/*  Each define specificates if a behaviour will affect the shader                      */
/****************************************************************************************/
//#define EYE_NEAREST          /**< Uses the information in the eyePos texture to get the nearest eye position of the fragment*/
 
/****************************************************************************************/
/* Shader Begin.                                                                        */
/****************************************************************************************/
//uniform sampler1D gridFunction;     /**< Grid Function Texture*/
uniform sampler1D gridInvFunction;    /**< Grid Inverse Function Texture*/
//uniform float gridFuncWidth;        /**< Grid Function Texture Dimension*/

uniform sampler2D gridBitMap;         /**< Grid BitMap Texture*/
//uniform float gridBitMapWidth;      /**< Grid BitMap Texture Dimensions*/
//uniform float gridBitMapHeight;     /**< Grid BitMap Texture Dimensions*/

uniform sampler2D eyePosTex;          /**< EyePosition Nearest Eye Fragment Texture*/

/**
 * Projection Parameters
 */
uniform float screenWidth;
uniform float screenHeight;
uniform float near;
uniform float far;
uniform float right;
uniform float top;

varying vec4 color;
varying vec3 eyePos;
varying vec4 worldPos;
varying vec3 normal;

#define PI 3.14159265

#define RED vec4(1,0,0,1)
#define GREEN vec4(0,1,0,1)
#define BLUE vec4(0,0,1,1)
#define CYAN vec4(0,1,1,1)
#define PINK vec4(1,0,1,1)
#define YELLOW vec4(1,1,0,1)
#define WHITE vec4(1,1,1,1)
#define BLACK vec4(0,0,0,1)
#define ORANGE vec4(1.,.5, 0., 1.)


void main()
{
  float dist = -eyePos.z;
  //float dist = length(eyePos);
  
#ifdef EYE_NEAREST
  float eyeNear = texture2D(eyePosTex, gl_FragCoord.xy/vec2(screenWidth, screenHeight)).a;
  if(eyeNear <= 0.0)
    discard;
  float fragNear = eyeNear;
#else  
  float fragNear = near;
#endif  
  
  float distNorm = (dist - fragNear)/far;
  float gridIndex = texture1D(gridInvFunction, distNorm).a;
  vec4 bitMask = texture2D(gridBitMap, vec2(gridIndex, .5));
  
  //OutPut
  gl_FragData[0] = vec4(0,pow(4.,.5),1,0);
  gl_FragData[1] = vec4(bitMask);
  gl_FragData[2] = vec4(texture2D(eyePosTex, gl_FragCoord.xy/vec2(screenWidth, screenHeight)).xyz, gl_FragCoord.z);
}







