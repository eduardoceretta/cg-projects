/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Aug 2011
 *
 *  Perform the calculation of the Voxelization of the scene
 *  Create a non regular grid and voxelizes the scene into it. Uses blending to make a bitmask that defines the grid.
 */
/****************************************************************************************/
/* Shader Begin.                                                                        */
/****************************************************************************************/
uniform sampler1D gridFunction;     /**< Grid Function Texture*/
uniform sampler1D gridInvFunction;  /**< Grid Inverse Function Texture*/
uniform float gridFuncWidth;        /**< Grid Function Texture Dimension*/

uniform sampler2D gridBitMap;       /**< Grid BitMap Texture*/
uniform float gridBitMapWidth;      /**< Grid BitMap Texture Dimensions*/
uniform float gridBitMapHeight;     /**< Grid BitMap Texture Dimensions*/


/**
 * Projection Parameters
 */
uniform float screenWidth;
uniform float screenHeight;
uniform float near;
uniform float far;
uniform float right;
uniform float top;
uniform float teste;

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

float rand(vec2 n)
{
  return 0.5 + 0.5 *
     fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

/**
 * Calculate the window position from the eye position
 */
vec3 eye2window(vec3 eye);

/**
 * Calculate the eye position from the window position
 */
vec3 window2eye(vec3 w);

/**
 * Calculate the ndc position from the window position
 */
vec3 window2ndc(vec3 w);

void main()
{
  float dist = length(eyePos);
  float distNorm = (dist - near)/far;
  //float gridIndex = pow(distNorm, 4);//texture1D(gridFunction, distNorm).a;
  //float gridIndex = pow(distNorm, .25);//texture1D(gridFunction, distNorm).a;
  //float gridIndex = distNorm;
  float gridIndex = texture1D(gridInvFunction, distNorm).a;
  //vec4 bitMask = texture2D(gridBitMap, vec2(distNorm, .5));
  vec4 bitMask = texture2D(gridBitMap, vec2(gridIndex, .5));
  
  
  //OutPut
  gl_FragData[0] = vec4(0,pow(4,.5),1,0);
  gl_FragData[1] = vec4(bitMask);
  gl_FragData[2] = vec4(pow(3,.5),pow(4,.5),pow(2,.5),1.5);//vec4(gl_ProjectionMatrixInverse*vec4(window2ndc(gl_FragCoord.xyz),1));
}

vec3 ndc2eye(vec3 ndc)
{	
//--assuming r == -l, t == -b
  float ze = 2. * far * near/(ndc.z * (far - near) - (far + near));
  float ye = -ze * ndc.y * (top - (-top))/(2.0*near);
  float xe = -ze * ndc.x * (right - (-right))/(2.0*near);
  
  return vec3(xe, ye, ze);
}

vec3 eye2ndc(vec3 eye)
{
//--assuming r == -l, t == -b
	float xn = 2. * near * eye.x/(right - (-right));
	float yn = 2. * near * eye.y/(top - (-top));
	float zn = -eye.z * (far + near)/(far - near) - 2. * far * near/(far - near);
	
	xn = xn/-eye.z;
	yn = yn/-eye.z;
	zn = zn/-eye.z;
	
	return vec3(xn, yn, zn);
}

vec3 window2ndc(vec3 w)
{
  //--assuming window (0,0), (screenWidth, screenHeight)
  //--assuming r == -l, t == -b
  float xn = 2. * w.x/screenWidth - 1.;
  float yn = 2. * w.y/screenHeight - 1.;
  float zn = (2. * w.z -1.);
  
  return vec3(xn, yn, zn);
}


vec3 ndc2window(vec3 ndc)
{
//--assuming window (0,0), (parms.w, parms.h)
  float xw = floor(screenWidth * ndc.x/2. + screenWidth/2.) + .5;
  float yw = floor(screenHeight * ndc.y/2. + screenHeight/2.) + .5;
  float zw = ndc.z/2. + .5;
  return vec3(xw, yw, zw);
}

vec3 window2eye(vec3 w)
{
   return ndc2eye(window2ndc(w));
}

vec3 eye2window(vec3 eye)
{
  return ndc2window(eye2ndc(eye));
}




