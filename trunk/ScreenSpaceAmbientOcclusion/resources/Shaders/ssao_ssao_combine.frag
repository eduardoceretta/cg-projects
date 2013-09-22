/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2011
 *
 *  Perform the calculation of the Screen Space Ambient Occlusion.
 *  Receive projection matrix information and reprojects the pixels obtained from the depth
 *  input textures and does the calculation of the occlusion that the neighborhood of a pixel 
 *  causes using spheres as occludes approximation.
 */


/****************************************************************************************/
/* Shader Controls.                                                                     */
/*  Each define specificates if a behaviour will affect the shader                      */
/****************************************************************************************/
 //#define INV_DIST_DIVIDE 2.    /**< Divide the ambient occlusion by the power distance of the pixels*/
#define SPHERE_CENTER_MINUS_NORMAL .01 /**< Modifies the center of each sphere so it does not cause same plane occlusion*/
//#define Z_MINUS_R -.5      /**< Modifies the center of each sphere z position using the radius as factor*/
//#define INVERT_NORMAL      /**< Invert model normals*/



/****************************************************************************************/
/* Shader Begin.                                                                        */
/****************************************************************************************/
uniform sampler2D aoTex;            /**< Ambient Occlusion to Combine*/
uniform sampler2D depth_normalTex;  /**< Fragment Peeling level 0. Normal(rbg) Depth(a)*/

uniform float samplerSize;      /**< 2D: Quad half side; 1D: Vector Length*/

 #define SAMPLER_VECTOR_SIZE 254
uniform float sampleArray [SAMPLER_VECTOR_SIZE];

uniform float rfar;             /**< Neighborhood access distance*/
uniform float offsets_size;     /**< Controls the search area of the shader in screen space*/ 
uniform float intensity;        /**< Affects directly the final result*/

/**
 * Projection Parameters
 */
uniform float screenWidth;
uniform float screenHeight;
uniform float near;
uniform float far;
uniform float right;
uniform float top;

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

/**
 * Calculate the corresponding sphere for the given window cordinates.
 */
vec4 getSphere(float xw, float yw, float zw);

/**
 * Calculate the ambient occlusion caused by the sphereQ a the point P.
 */
float getAproxAO(vec4 sphereQ, vec3 posP, vec3 normalP,float radiusP);

/**
 * Calculate the Local ambient occlusion that the neighborhood at coord+(i,j) causes in P.
 */
float calcLocalAO(float i, float j, float depth, vec4 depth0_normal, float dx, float dy, vec4 position);

void main()
{
  //Output Color 
  vec4 color = vec4(1,1,1,1);

  //Current Fragment Information Normal(rbg) Depht(a)
  vec4 depth0_normal = texture2D(depth_normalTex,  gl_TexCoord[0].st);
  float depth = depth0_normal.a;

  //Current Fragment Information Position (xyz) Screen Sphere Radius (w)
  vec4 position = getSphere(gl_FragCoord.x , gl_FragCoord.y , depth); 
  position = position/far;
  
  // Do nothing if the depth value is negative
	if(depth < 0.0)
		discard;
  
  //Calculate the screen width/height rate  
  float dx = 1.0/screenWidth;
  float dy = 1.0/screenHeight;
  
  //TotalAmbient Occlusion
  float totalAO = 0.0;
  
  //Determines the size of the neighborhood
  int size = int(samplerSize);
  
  for(int k = 0; k < size ; k+=2)
  {
    float i = floor(sampleArray[k*2] + .5);
    float j = floor(sampleArray[k*2 + 1] + .5);
    {
      i *= offsets_size;
      j *= offsets_size;
    }
   
    totalAO += calcLocalAO(i,j, depth, depth0_normal, dx, dy, position);
  }

  //Total Ambient Occlusion Normalization
  totalAO = intensity *  totalAO/(2.0*PI*pow(position.w,2.0));
  totalAO = clamp(totalAO,0.0,1.0);

  float otherAo = texture2D(aoTex, gl_TexCoord[0].st).r;
  float result = 1.0 - clamp(totalAO + (1.0 - otherAo), 0.0, 1.0);
  //OutPut
  gl_FragData[0] = vec4(vec3(result), 1.0);
  //gl_FragData[0] = color*(1.0 - (totalAO));
  //gl_FragData[0] = color*otherAo;
}

float calcLocalAO(float i, float j, float depth, vec4 depth0_normal, float dx, float dy, vec4 position)
{
  float localAO = 0.0;
  int k = 0;
  vec2 inc = vec2((i)*dx, (j)*dy);
  vec4 depth_normal = texture2D(depth_normalTex,  gl_TexCoord[0].st + inc);  
  
  if(depth_normal.a < 0.0)
    return localAO;

  vec4 sphere = getSphere(gl_FragCoord.x + i , gl_FragCoord.y + j, depth_normal.a)/far;
  #ifdef SPHERE_CENTER_MINUS_NORMAL
    sphere.xyz = sphere.xyz - normalize(depth_normal.xyz)*SPHERE_CENTER_MINUS_NORMAL/far;
  #endif
  
  float eye_dist = length(sphere.xyz - position.xyz);

  if(eye_dist > rfar)
    return localAO;
  
  #ifdef INV_DIST_DIVIDE
    float dist_divide = pow(eye_dist, INV_DIST_DIVIDE);
  #else  
    float dist_divide = 1.;
  #endif  
  
  localAO = (getAproxAO(sphere, position.xyz, depth0_normal.xyz, position.w)/dist_divide);
  return localAO;
}

vec3 ndc2eye(vec3 ndc)
{	
//--assuming r == -l, t == -b
  float ze = 2. * far * near/(ndc.z * (far - near) - (far + near));
  float ye = -ze * ndc.y * (top - (-top))/(2.0*near);
  float xe = -ze * ndc.x * (right - (-right))/(2.0*near);
  
  return vec3(xe, ye, ze);
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

vec3 window2eye(vec3 w)
{
   return ndc2eye(window2ndc(w));
}

vec4 getSphere(float xw, float yw, float zw)
{
  //assuming window (0,0), (w, h)
  //assuming r == -l, t == -b
  vec3 ex1 = window2eye(vec3(xw, yw, zw));
  vec3 ex2 = window2eye(vec3(xw + 1., yw, zw));
  float rr = length(ex2 - ex1)/2.;
  #ifdef Z_MINUS_R
    ex1.z -= rr * Z_MINUS_R;
  #endif
  return vec4(ex1,rr);
}

float getAproxAO(vec4 sphereQ, vec3 posP, vec3 normalP, float radiusP)
{
  vec3 PQ = (sphereQ.xyz - posP);
  float i = 1.;
  float S = 2.0 * PI  * (i*radiusP) * (i*radiusP) * (1.0 - cos(asin((sphereQ.a)/ length(PQ))));
  #ifdef INVERT_NORMAL
    normalP = -normalP;
  #endif
  float max_dot = max(dot(normalize(normalP), normalize(PQ)), 0.0);
  return S*max_dot;
}
