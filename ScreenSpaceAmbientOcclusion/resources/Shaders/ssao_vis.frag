/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Jun 2011
 *
 *  Perform the calculation of the Screen Space Ambient Occlusion.
 *  Receive projection matrix information and reprojects the pixels obtained from the depth
 *  input textures and does visibility tests with its surrounds
 */


/****************************************************************************************/
/* Shader Controls.                                                                     */
/*  Each define specificates if a behaviour will affect the shader                      */
/****************************************************************************************/
#define ONE_MINUS_AO          /**< Output is 1 - TotalAmbientOcclusion*/
//#define SIZE_OVER_DEPTH     /**< The Area of the neighborhood will change in function of depth*/
//#define DEPTH_PEELING       /**< Uses multiple peeling information in the calculation*/
#define MAX_OVER_PEELING      /**< Considers the Ambient occlusion as the max value of the peelings, else sums they*/
//#define INV_DIST_DIVIDE 2.    /**< Divide the ambient occlusion by the power distance of the pixels*/
#define SPHERE_CENTER_MINUS_NORMAL .01 /**< Modifies the center of each sphere so it does not cause same plane occlusion*/
//#define Z_MINUS_R -.5      /**< Modifies the center of each sphere z position using the radius as factor*/
//#define INVERT_NORMAL      /**< Invert model normals*/



//#define COS_SQUARE
#define NOMRAL_TRANSLATE .0005
//#define RAND_LEN .5
//#define RAND_DIR .5
//


/****************************************************************************************/
/* Shader Begin.                                                                        */
/****************************************************************************************/
uniform sampler2D depth0_normalTex;  /**< Fragment Peeling level 0. Normal(rbg) Depth(a)*/


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

float rand(vec2 n)
{
  return 0.5 + 0.5 *
     fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

/**
 * Calculate the corresponding sphere for the given window cordinates.
 */
vec4 getSphere(float xw, float yw, float zw);

/**
 * Calculate a rotation matrix around the axis u with the radian angle rad_ang
 */
mat3 getRotationMatrix(vec3 u, float rad_ang);

/**
 * Calculate the corresponding tangent vector for the given point.
 */
vec3 getTangent(vec2 fragCoord, vec3 position, vec3 normal,  float dx, float dy);

/**
 * Calculate the ambient occlusion caused by the sphereQ a the point P.
 */
float getAproxAO(vec4 sphereQ, vec3 posP, vec3 normalP,float radiusP);

/**
 * Calculate the Local ambient occlusion that the neighborhood at coord+(i,j) causes in P.
 */
float calcLocalAO(float i, float j, float depth, vec4 depth0_normal, float dx, float dy, vec4 position, inout int n);


float calcHorizontAngle(int num_steps, float rfar, vec3 normal, vec3 tangent, vec3 position, float dx, float dy);

float calcNormalRays(int num_rays, int num_steps, float rfar, vec3 normal, vec3 tangent, vec3 position, float init_angle, float dx, float dy);
/**
 * Calculate the window position from the eye position
 */
vec3 eye2window(vec3 eye);

/**
 * Calculate the eye position from the window position
 */
vec3 window2eye(vec3 w);

void main()
{
  //Output Color 
  vec4 color = vec4(1,1,1,1);

  //Current Fragment Information Normal(rbg) Depht(a)
  vec4 depth0_normal = texture2D(depth0_normalTex,  gl_TexCoord[0].st);
  float depth = depth0_normal.a;

  //Current Fragment Information Position (xyz) Screen Sphere Radius (w)
  vec3 position = window2eye(vec3(gl_FragCoord.x , gl_FragCoord.y , depth)); 
  
  /**
	 * TEST SPACE TRANSFORMATIONS
	 *  Verifies if the a window coordinate can be transformed to eye and back.
	 *  Result must be RED for correct Transformation.
   * /
  vec3 pe = window2eye(vec3(gl_FragCoord.x , gl_FragCoord.y , depth)); 
  vec3 pew = eye2window(pe);
  if(abs(depth - pew.z) < .0000001 && gl_FragCoord.x == pew.x && gl_FragCoord.y == pew.y)
    gl_FragData[0] = RED;
  else gl_FragData[0] = BLUE;
  return;
  /**/
  
  
  
  //vec4 sphere_l = getSphere(gl_FragCoord.x + 1, gl_FragCoord.y, texture2D(depth0_normalTex,  gl_TexCoord[0].st + vec2(1./screenWidth,0)).a);
  //vec4 sphere_d = getSphere(gl_FragCoord.x, gl_FragCoord.y + 1, texture2D(depth0_normalTex,  gl_TexCoord[0].st + vec2(0, 1./screenHeight)).a);
  //depth0_normal.xyz = normalize(cross((sphere_l.xyz - position.xyz), (sphere_d.xyz - position.xyz))); 
  
  vec3 normal = normalize(depth0_normal.xyz);
  position = position/far;

  // Do nothing if the depth value is negative
	if(depth < 0.0)
	{
		gl_FragData[0] = vec4(.8, .8, 1.0, -1.0);
		return;
	}

  /**
	 * TEST NORMAL
	 *  Print the Normal's Color.
	 *  Result must be Blue for normals facing the screen
   * /
    vec3 nn = (depth0_normal.xyz)*.5 + .5;
    gl_FragData[0] = vec4(nn, 1.);
    return;
  /**/
  
  //Calculate the screen width/height rate  
  float dx = 1.0/screenWidth;
  float dy = 1.0/screenHeight;
  
  //TotalAmbient Occlusion
  float totalAO = 0.0;
  
  //Fragment eye space tangent
  vec3 tangent = normalize(getTangent(gl_FragCoord.xy, position.xyz, normal,  dx, dy));
  //vec3 tangent = normalize(cross(vec3(0,1,0),normal));
  //tangent = normalize(vec3(1,0,0));
  /**
	 * TEST TANGENT
	 *  Print the Tangent's Color.
   * /
    //t = normalize(cross(t, normal));
      vec3 tt = vec3(t*.5) + .5;
      gl_FragData[0] = vec4(tt, 1.);
    return;
  /**/
  
  /**
	 * TEST ROTATION MATRIX
	 *  Does a rotation of step_angle and verifies if the rotated vector has the same angle with t.
	 *  Result must be RED
   * /
    
    float Tstep_angle = radians(45.);
    mat3 Trot = getRotationMatrix(normal, Tstep_angle);

    vec3 tt = normalize(Trot*t);
    tt = normalize(Trot*tt);
    if(degrees(acos(dot(t,tt))) >= 89.99 && degrees(acos(dot(t,tt))) <=90.01)
      gl_FragData[0] = RED;
    else gl_FragData[0] = YELLOW;
    return;
  /**/
 //mat3 rot2 = getRotationMatrix(normal, radians(120.));
   //tangent = normalize(rot2*tangent);
  int nd = 8;
  int ns = 5;
  int nn = 8;
  
  float step_angle = (2. * PI/float(nd))/2.;
#ifndef RAND_DIR      
  mat3 rot = getRotationMatrix(normal, step_angle);
#endif 
  float rfare = .01;
  for(int j = 0; j < nd; ++j)
  {
#ifdef RAND_DIR      
    mat3 rot = getRotationMatrix(normal, step_angle *(1. + (rand(gl_FragCoord.xy) - .5)*RAND_DIR));
#endif 
    tangent = normalize(rot*tangent);
    //float cos_theta = calcHorizontAngle(ns, rfare, normal, tangent, position, dx, dy);
    
    //float cos_alpha = calcNormalRays(nn, ns, rfare, normal, tangent, position, clamp(acos(cos_theta),0. , PI/2.), dx, dy);
    float cos_alpha = calcNormalRays(nn, ns, rfare, normal, tangent, position, 0., dx, dy);

    float ao = 0.;
#ifdef COS_SQUARE      
    //float sin_theta = 1. - cos_theta * cos_theta;
    //ao += sin_theta*sin_theta;
#else        
    //ao += 1. - cos_theta;
#endif    
//
#ifdef COS_SQUARE      
    float sin_alpha = 1. - cos_alpha * cos_alpha;
    ao += sin_alpha*sin_alpha;
#else        
    ao += 1. - cos_alpha;
#endif     
    totalAO += min(ao, 1.);
    if(ao > 1.0)
    {
      gl_FragData[0] = RED;
      return;
    }
      
  }
  /**/
  
  //Total Ambient Occlusion Normalization
  totalAO = intensity *  totalAO/(nd /* *PI*/);
  totalAO = clamp(totalAO, 0.0, 1.0);

  //OutPut
  #ifdef ONE_MINUS_AO
    gl_FragData[0] = color*(1.0 - totalAO);
  #else
    gl_FragData[0] = color*(totalAO);
  #endif
  
  gl_FragData[1] = RED*pixelmask;
}

float calcHorizontAngle(int num_steps, float rfar, vec3 normal, vec3 tangent, vec3 position, float dx, float dy)
{
  vec3 t = tangent;
  float step_len = rfar/num_steps;
#ifdef RAND_LEN        
  float len = step_len*(1. + (rand(gl_FragCoord.xy) - .5)*RAND_LEN);
#else
  float len = step_len;
#endif   
  
  vec3 init_t = t;
  int i = 0;
  while(len <= rfar && i < 5. * num_steps)
  {
#ifdef NOMRAL_TRANSLATE
    vec3 qe = position.xyz + normal*NOMRAL_TRANSLATE + t*len;
#else
    vec3 qe = position.xyz + t*len;
#endif         
    vec3 qw = eye2window(qe*far);
    vec2 coord = qw.xy * vec2(dx, dy);
    float texZ = texture2D(depth0_normalTex,  coord).a;
    if(texZ >= 0. && qw.z >= 0.)
    {
      if(texZ <= qw.z) // DepthBuffer's depth is above Q
      {
        qw.z = texZ;
        qe = window2eye(qw)/far;
        vec3 new_t = qe - position.xyz;

        float new_len = length(new_t);

        new_t = normalize(new_t);

        if(!(
          false 
          || dot(new_t, init_t)  < 0.
          || new_len > rfar 
          )) 
        { 
          len = new_len;
          t = new_t;
        }
      }
    }
#ifdef RAND_LEN        
    len += step_len*(1. + (rand(gl_FragCoord.xy) - .5)*RAND_LEN);
#else
    len += step_len;
#endif         
    i++;
  }
  
  float cos_theta = dot(normalize(t), normalize(init_t));
  return cos_theta;
}


float calcNormalRays(int num_rays, int num_steps, float rfar, vec3 normal, vec3 tangent, vec3 position, float init_angle, float dx, float dy)
{
  vec3 right = normalize(cross(tangent, normal));
  mat3 rot = getRotationMatrix(right, init_angle);
  vec3 t = normalize(rot*tangent);
 
  float step_angle = ((PI/2. - init_angle)/num_rays)/2.;
  rot = getRotationMatrix(right, step_angle);
  
  float step_len = rfar/num_steps;
  
  float num_hitted = 0.;
  for(int k = 0; k < num_rays; ++k)
  {
    t = normalize(rot*t);
    bool hit = false;
    for(int l = 0; l < num_steps && !hit; ++l)
    {
#ifdef RAND_LEN        
      float len = (l + 1)*step_len*(1. + (rand(gl_FragCoord.xy) - .5)*RAND_LEN);
#else
      float len = (l + 1)*step_len;
#endif  

#ifdef NOMRAL_TRANSLATE
    vec3 qe = position.xyz + normal*NOMRAL_TRANSLATE + t*len;
#else
    vec3 qe = position.xyz + t*len;
#endif         
    vec3 qw = eye2window(qe*far);
    vec2 coord = qw.xy * vec2(dx, dy);
    float texZ = texture2D(depth0_normalTex,  coord).a;
    if(texZ >= 0. && qw.z >= 0.)
      if(texZ < qw.z) // DepthBuffer's depth is above Q
      {
        qw.z = texZ;
        qe = window2eye(qw)/far;
        vec3 new_t = qe - position.xyz;

        float new_len = length(new_t);

        new_t = normalize(new_t);

        if(!(dot(new_t, tangent)  < 0. || new_len > rfar)) 
          hit = true;
      }
    }
    if(hit)
      num_hitted += 1.0;
  }
  
  return cos((PI/2. - init_angle)*num_hitted/num_rays);
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






vec3 getTangent(vec2 fragCoord, vec3 position, vec3 normal,  float dx, float dy)
{
  float d = texture2D(depth0_normalTex,  gl_TexCoord[0].st + vec2(dx,0)).a;
  if(d >= 0.)
    //return BLUE.rgb;
    return normalize(cross((window2eye(vec3(gl_FragCoord.x + 1., gl_FragCoord.y, d))/far - position), normal));

  d = texture2D(depth0_normalTex,  gl_TexCoord[0].st + vec2(-dx,0)).a;
  if(d >= 0.)
    //return RED.rgb;
    return normalize(cross((window2eye(vec3(gl_FragCoord.x - 1., gl_FragCoord.y, d))/far - position), normal));
    
  d = texture2D(depth0_normalTex,  gl_TexCoord[0].st + vec2(0,dy)).a;
  if(d >= 0.)
    //return YELLOW.rgb;
    return normalize(cross((window2eye(vec3(gl_FragCoord.x, gl_FragCoord.y + 1., d))/far - position), normal));
    
  d = texture2D(depth0_normalTex,  gl_TexCoord[0].st + vec2(0,-dy)).a;
  if(d >= 0.)
    //return GREEN.rgb;
    return normalize(cross((window2eye(vec3(gl_FragCoord.x, gl_FragCoord.y - 1., d))/far - position), normal));
    
  d = texture2D(depth0_normalTex,  gl_TexCoord[0].st + vec2(-dx,0)).a;
  if(d >= 0.)
    //return PINK.rgb;
    return normalize(cross((window2eye(vec3(gl_FragCoord.x - 1., gl_FragCoord.y, d))/far - position), normal));
  //return BLACK.rgb;
  return normalize(cross(vec3(0,1,0), normal));
}

mat3 getRotationMatrix(vec3 u, float rad_ang)
{
  float c = cos(rad_ang);
  float s = sin(rad_ang);
  float o_c = 1. - c;
       
  //COLUMN MATRIX
  return mat3(c + u.x*u.x*o_c, u.y*u.x*o_c + u.z*s, u.z*u.x*o_c - u.y*s,
               u.x*u.y*o_c - u.z*s, c + u.y*u.y*o_c, u.z*u.y*o_c + u.x*s,
               u.x*u.z*o_c + u.y*s, u.y*u.z*o_c - u.x*s, c + u.z*u.z*o_c);

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
