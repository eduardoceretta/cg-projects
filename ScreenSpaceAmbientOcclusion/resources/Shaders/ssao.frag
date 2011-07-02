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
#define ONE_MINUS_AO          /**< Output is 1 - TotalAmbientOcclusion*/
//#define SIZE_OVER_DEPTH     /**< The Area of the neighborhood will change in function of depth*/
//#define DEPTH_PEELING       /**< Uses multiple peeling information in the calculation*/
#define MAX_OVER_PEELING      /**< Considers the Ambient occlusion as the max value of the peelings, else sums they*/
#define SPHERE_POSITION       /**< Calculate pixel position, else uses positionTex*/
//#define INV_DIST_DIVIDE 2.    /**< Divide the ambient occlusion by the power distance of the pixels*/
#define SPHERE_CENTER_MINUS_NORMAL .01 /**< Modifies the center of each sphere so it does not cause same plane occlusion*/
//#define Z_MINUS_R -.5      /**< Modifies the center of each sphere z position using the radius as factor*/
//#define INVERT_NORMAL      /**< Invert model normals*/

/**
 * Neighborhood mode. Access the 2D texture containing binary information, if the pixel must or not be accessed.
 *  Must be the same of KernelSSAO.cpp
 */
//#define SAMPLER_QUAD     

/**
 * Neighborhood mode. Access the 1D texture containing the respective index of the neighborhood access.
 *  Must be the same of KernelSSAO.cpp
 */
#define SAMPLER_VECTOR


/****************************************************************************************/
/* Shader Begin.                                                                        */
/****************************************************************************************/
uniform sampler2D positionTex;       /**< Fragment eye space position*/
uniform sampler2D depth0_normalTex;  /**< Fragment Peeling level 0. Normal(rbg) Depth(a)*/
uniform sampler2D depth1_normalTex;  /**< Fragment Peeling level 1. Normal(rbg) Depth(a)*/
uniform sampler2D depth2_normalTex;  /**< Fragment Peeling level 2. Normal(rbg) Depth(a)*/

#ifdef SAMPLER_QUAD
  uniform sampler2D sampleTex;  /**< 2D Sampler Texture. If 0 do not acess else acess neighbor*/
#else
  uniform sampler1D sampleTex;  /**< 1D Sampler Texture. Neighborhood indexes*/
#endif
uniform float samplerSize;      /**< 2D: Quad half side; 1D: Vector Length*/

uniform float pixelmask;        /**< Depth parameter in case of SIZE_OVER_DEPTH is defined*/

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
float calcLocalAO(float i, float j, float depth, vec4 depth0_normal, float dx, float dy, vec4 position, inout int n);

void main()
{
  //Output Color 
  vec4 color = vec4(1,1,1,1);

  //Current Fragment Information Normal(rbg) Depht(a)
  vec4 depth0_normal = texture2D(depth0_normalTex,  gl_TexCoord[0].st);
  float depth = depth0_normal.a;

  //Current Fragment Information Position (xyz) Screen Sphere Radius (w)
  #ifdef SPHERE_POSITION
    vec4 position = getSphere(gl_FragCoord.x , gl_FragCoord.y , depth); 
  #else
    vec4 position = texture2D(positionTex,  gl_TexCoord[0].st);
    position.w = getSphere(gl_FragCoord.x , gl_FragCoord.y , depth).w;
  #endif
  position = position/far;

  // Do nothing if the depth value is negative
	if(depth < 0.0)
	{
		gl_FragData[0] = vec4(.8, .8, 1.0, -1.0);
		return;
	}
	
	  //float f = length(position.xyz)/far;
	  //if(f < 0)
	    //gl_FragData[0] = BLUE;
	  //else if(f < 1)
	    //gl_FragData[0] = WHITE*f;
	  //else
	    //gl_FragData[0] = RED;
	  //
    //return;
	
  /**
	 * TEST NORMAL
	 *  Print the Normal's Color.
	 *  Result must be Blue for normals facing the screen
   * /
    vec3 nn = (depth0_normal.xyz)*.5 + .5;
    gl_FragData[0] = vec4(nn, 1.);
    return;
  /**/
  
  /**
	 * TEST POSITION
	 *  Print the Diference between position from positionTex and calculated by getSphere. 
	 *  Result must be RED so the diference is small
   * /
    vec4 pp = getSphere(gl_FragCoord.x, gl_FragCoord.y , depth);
    if(length(pp.xyz - texture2D(positionTex,  gl_TexCoord[0].st).xyz) < .00009)
      gl_FragData[0] = RED;
    else gl_FragData[0] = BLUE;
    return;
  /**/
  
  //Calculate the screen width/height rate  
  float dx = 1.0/screenWidth;
  float dy = 1.0/screenHeight;
  
  //TotalAmbient Occlusion
  float totalAO = 0.0;
  
  //Determines the size of the neighborhood
#ifdef SAMPLER_QUAD
  float samplerTotalSize = samplerSize*2. + 1.;
  #ifdef SIZE_OVER_DEPTH
    float v = pixelmask;
    float n_depth = max(depth - v,0.)*1./(1.-v);
    int size = int(max(floor((1.-n_depth)*samplerSize + .5), 1.));
  #else
    int size = int(samplerSize);
  #endif
#else
  #ifdef SIZE_OVER_DEPTH
    float v = pixelmask;
    float n_depth = max(depth - v,0.)*1./(1.-v);
    int size = int(max(floor((1.-n_depth)*samplerSize + .5), 9.));
  #else
    int size = int(samplerSize);
  #endif
#endif

  /**
	 * TEST SIZE_OVER_DEPTH
	 *  Render the depth levels 
	 *  Result must be different colors for the different level of depth.
   * /
  if (size == samplerSize)
    gl_FragData[0] = ORANGE;
  else if (size > samplerSize*.9)
    gl_FragData[0] = WHITE;
  else if (size > samplerSize*.75)
    gl_FragData[0] = RED;
  else if (size > samplerSize*.6)
    gl_FragData[0] = CYAN;
  else if (size > samplerSize*.45)
    gl_FragData[0] = BLUE;
  else if (size > samplerSize*.3)
    gl_FragData[0] = BLACK;
  else if (size > samplerSize*.15)
    gl_FragData[0] = GREEN;
  else if (size >= samplerSize*.05)
    gl_FragData[0] = YELLOW;
  else 
    gl_FragData[0] = PINK;
  return;
  /**/
  
  //Number of Samplers Counter
  int n = 0;
  
  //Neighborhood fetch
#ifdef SAMPLER_QUAD
  for(int i=-size; i < size + 1; ++i)
    for(int j = -size; j < size + 1; ++j)
      if(!(i==0 && j==0))
      {
        vec2 coord = vec2((float(i) + samplerSize + .5)/samplerTotalSize, (float(j) + samplerSize + .5)/samplerTotalSize);
        float sample = floor(texture2D(sampleTex, coord).a + .5);
        if(sample == 1.0)
        {
          totalAO += calcLocalAO(float(i),float(j), depth, depth0_normal, dx, dy, position, n);
        }
      }

#else
  for(int k = 0; k < size ; ++k)
  {
    float coord = (float(k) + .5) / samplerSize;
    vec2 sample = texture1D(sampleTex, coord).xy;
    float i = floor(sample.x + .5);
    float j = floor(sample.y + .5);
    {
      i *= offsets_size;
      j *= offsets_size;
    }
   
    totalAO += calcLocalAO(i,j, depth, depth0_normal, dx, dy, position, n);
  }
#endif

  //Total Ambient Occlusion Normalization
  totalAO = intensity *  totalAO/(2*PI*pow(position.w,2));//(float(n));
  totalAO = clamp(totalAO,0.0,1.0);

  //OutPut
  #ifdef ONE_MINUS_AO
    gl_FragData[0] = color*(1.0 - totalAO);
  #else
    gl_FragData[0] = color*(totalAO);
  #endif
  
  gl_FragData[1] = RED*pixelmask;
}

float calcLocalAO(float i, float j, float depth, vec4 depth0_normal, float dx, float dy, vec4 position, inout int n)
{
  float localAO = 0.0;
  int nn = 0;
#ifdef DEPTH_PEELING
  for(int k = 0; k < 3; ++k)
#else
  int k = 0;
#endif
  {
    vec2 inc = vec2((i)*dx, (j)*dy);
    vec4 depth_normal;
    switch(k)
    {
      case 0:
        depth_normal = texture2D(depth0_normalTex,  gl_TexCoord[0].st + inc);  
      break;
      case 1:
        depth_normal = texture2D(depth1_normalTex,  gl_TexCoord[0].st + inc);  
      break;
      case 2:
        depth_normal = texture2D(depth2_normalTex,  gl_TexCoord[0].st + inc);  
      break;
      default:
        depth_normal = vec4(-1.0, -1.0, -1.0, -1.0);
      break;
    }
    
    
    if(depth_normal.a < 0.0)
    #ifdef DEPTH_PEELING
      continue;
    #else
      return localAO;
    #endif


    vec4 sphere = getSphere(gl_FragCoord.x + i , gl_FragCoord.y + j, depth_normal.a)/far;
    #ifdef SPHERE_CENTER_MINUS_NORMAL
      sphere.xyz = sphere.xyz - normalize(depth_normal.xyz)*SPHERE_CENTER_MINUS_NORMAL/far;
    #endif
    
    float eye_dist = length(sphere.xyz - position.xyz);

    if(eye_dist > rfar)
    #ifdef DEPTH_PEELING
      continue;
    #else
      return localAO;
    #endif
    
    #ifdef INV_DIST_DIVIDE
      float dist_divide = pow(eye_dist, INV_DIST_DIVIDE);
    #else  
      float dist_divide = 1.;
    #endif  
    
    #ifdef MAX_OVER_PEELING
      localAO = max(localAO, getAproxAO(sphere, position.xyz, depth0_normal.xyz, position.w)/dist_divide);
      nn = 1;
    #else
      localAO += (getAproxAO(sphere, position.xyz, depth0_normal.xyz, position.w)/dist_divide);
      nn++;
    #endif
  }
  n += nn;
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
