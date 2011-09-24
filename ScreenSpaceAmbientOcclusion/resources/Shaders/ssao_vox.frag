/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Sep 2011
 *
 *  Perform the calculation of the Ambient Occlusion of the Scene.
 *  Uses a non regular grid as occlusion query.
 */
#extension GL_EXT_gpu_shader4 : enable //Enable Integer Operations - Shader Model 4.0 GF 8 Series

/****************************************************************************************/
/* Shader Controls.                                                                     */
/*  Each define specificates if a behaviour will affect the shader                      */
/****************************************************************************************/
#define EYE_NEAREST          /**< Uses the information in the eyeNearest texture to get the nearest eye position of the fragment*/
 
/****************************************************************************************/
/* Shader Begin.                                                                        */
/****************************************************************************************/
uniform sampler1D rayDirections;      /**< Ray Distributions Texture. ray:(RGB)*/
uniform int rayDirectionsWidth;       /**< Ray Distributions Texture size*/
uniform int numRayDistribution;       /**< Number of hemisphere ray distribution*/
uniform int numRayDirections;         /**< Number of ray directions(360)*/
uniform int numRayHemispherDivision;  /**< Number of hemisphere division(90)*/
uniform int numRaySteps;              /**< Number of ray steps*/ 

uniform sampler1D gridInvFunc;        /**< Inverse Function of the grid cell structure*/

uniform sampler2D eyePos;             /**< Eye Position(rgb) Nearest Eye Fragment(a) Texture*/
uniform sampler2D normalDepth;         /**< Normal(rgb) Depth(a)*/
uniform usampler2D voxelGrid;         /**< None Uniform Voxel Grid of the Scene*/

/**
 * Projection Parameters
 */
uniform float screenWidth;
uniform float screenHeight;
uniform float near;
uniform float far;
uniform float right;
uniform float top;
uniform int perspective;

#define PI 3.14159265

#define RED vec4(.8,0,0,1)
#define GREEN vec4(0,.8,0,1)
#define BLUE vec4(0,0,.8,1)
#define CYAN vec4(0,.8,.8,1)
#define PINK vec4(.8,0,.8,1)
#define YELLOW vec4(.8,.8,0,1)
#define WHITE vec4(1,1,1,1)
#define BLACK vec4(0,0,0,1)
#define ORANGE vec4(1.,.5, 0., 1.)


/**
 * Generate PseudoNumbers from[0, ~1)
 *  Do not generate one!
 */
float rand(vec2 n)
{
  return fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

/**
 * Calculate the window position from the eye position.
 *  perspective indicates if the projection matrix is perspective(true) or orthographic(false)
 */
vec3 eye2window(vec3 eye, bool perspective = true);

/**
 * Calculate the eye position from the window position.
 *  perspective indicates if the projection matrix is perspective(true) or orthographic(false)
 */
vec3 window2eye(vec3 w, bool perspective = true);

/**
 * Calculate the ndc position from the window position
 */
vec3 window2ndc(vec3 w);

/**
 * Get the Rotation matrix of rad_angle around the axis u.
 */ 
mat3 getRotationMatrix(float rad_ang, vec3 u);

/**
 * Return True if the bit in the position pos of the unsigned int v is One
 */
bool isBitOne(unsigned int v, unsigned int pos);

/**
 * Write in the parameters the input values read from the textures.
 */
void readInputData(out vec3 normal, out vec3 eyePosition, out float depth);

/**
 * Get the Rotation Matrix that takes the ray to the normal space
 */
mat3 getHemisphereRotationMatrix(vec3 normal);

/**
 * Get the ray vector based on the fragment index and rotates it towards
 *  the normal of the fragment.
 */
vec3 getRay(int rayDistIndex, int i, mat3 rotMat);

/**
 * Get the grid cell index that the ray touches.
 */
vec3 getGridIndex(vec3 ray, vec3 eyePosition);

/**
 * Verify if the given grid voxel is empty.
 */
bool isVoxelEmpty(vec3 gridIndex);

/**
 * Access 4 texels and interpolate the information.
 */
float getVoxelDensity(vec3 gridIndex);

/**
 * Normalize the Ambient Occlusion based on the number of rays
 */
float normalizeAo(float ao, int numRays);


void main()
{
  int numRays = numRayDirections*numRayHemispherDivision;
  //int halfDist = numRayDistribution/2 + 1;
  //int rayDistributionIndex = clamp(int(floor(gl_FragCoord.x))%halfDist + halfDist - int(floor(gl_FragCoord.y))%halfDist, 0, numRayDistribution);
  int rayDistributionIndex = int(floor(rand(gl_FragCoord.xy)*float(numRayDistribution)))*numRays;
  
  vec3 normal;
  vec3 eyePosition;
  float depth;
  
  readInputData(normal, eyePosition, depth);
  if(depth < 0.0)
    discard;
  
  mat3 rotMat = getHemisphereRotationMatrix(normal);
  
  vec3 fragGridIndex = getGridIndex(vec3(0,0,0), eyePosition);
 
  /**
	 * TEST NORMAL
	 *  Print the Normal's Color.
	 *  Result must be Blue for normals facing the screen
   * /
    //vec3 nn = (normal.xyz)*.5 + .5;
    vec3 nn = (normal.xyz);
    gl_FragData[0] = vec4(nn, 1.);
    return;
  /**/
  
  float rfar = .5*far;
  float rayStep = rfar/float(numRaySteps);
  float ao = 0.0;
  for(int i = 0; i < numRays; i++)
  {
    vec3 ray = getRay(rayDistributionIndex, i, rotMat);
    //vec3 ray = getRay(0, i, rotMat);
    float dotRayNormal = dot(ray,normal);
    vec3 displacedEyePos =  eyePosition;// + normal*dotRayNormal*.0001*far;
    
    float sv = sqrt(
              pow(2.0*right/screenWidth, 2.0) + 
              pow(2.0*top/screenHeight, 2.0) + 
              pow((far)/128.0, 2.0) );
    
    float initRayDistance = sv/(dotRayNormal);
    float len = initRayDistance;

    bool hit = false;
    float flen = 0.0;
    
    while(len <= rfar)
    {
      vec3 gridIndex = getGridIndex(ray*len, displacedEyePos);
      len += rayStep;
      if(gridIndex.z < .0)
        continue;
      
      bool voxelEmpty = isVoxelEmpty(gridIndex);
      if(!voxelEmpty && !hit)
      {
        hit = true;
        flen = len;
      }

    }
    if(hit)
    {
      float distFunc = ((1.0-flen/rfar));
      //float distFunc = pow((1.0-flen/rfar),1.0);
      //float distFunc = sin((1.0-len/rfar)*PI/2.0);
      //float distFunc = (1.1518/(pow(2.7182818, pow((1.0-len/rfar) - 1.2, 4.0) )) - .15);
      //float distFunc = (1.1518/(pow(2.7182818, ((1.0-len/rfar)-1.2) )) - .15);
      //float distFunc = 1.0;
      float dotFunc = dotRayNormal;
      //float dotFunc = 1.0;
      ao += distFunc*dotFunc;
     
      //ao++;
    }
  }
  
  ao = normalizeAo(ao, numRays);
  ao = clamp(ao, 0.0, 1.0);

  gl_FragData[0] = WHITE*(1.0 - ao);
}


void readInputData(out vec3 normal, out vec3 eyePosition, out float depth)
{
  vec2 fragCoord = gl_FragCoord.xy/vec2(screenWidth, screenHeight);
  vec4 normalData = texture2D(normalDepth, fragCoord);
  normal = normalize(normalData.xyz);
  depth = normalData.a;
  
  vec4 eyePosData = texture2D(eyePos, fragCoord);
  eyePosition = eyePosData.xyz;
}

mat3 getHemisphereRotationMatrix(vec3 normal)
{
  vec3 hemisphereNormal = vec3(0.0, 1.0, 0.0);

  float angle = acos(dot(hemisphereNormal, normal));
  vec3 axis = normalize(cross(hemisphereNormal, normal));
  
  return getRotationMatrix(angle, axis);
}

vec3 getRay(int rayDistIndex, int i, mat3 rotMat)
{
  float rayIndex = (float(rayDistIndex + i) + .5)/float(rayDirectionsWidth);
  vec3 ray = texture1D(rayDirections, rayIndex).rgb;
  return normalize(rotMat*ray);
}

vec3 getGridIndex(vec3 ray, vec3 eyePosition)
{
  vec3 point = eyePosition + ray;
  float dist = -point.z;
  //float dist = length(point);
 
  vec3 win = eye2window(point, bool(perspective));
  
  float zzNear;  
#ifdef EYE_NEAREST
  zzNear = texture2D(eyePos, win.xy/vec2(screenWidth, screenHeight)).a;
#else  
  zzNear = near;
#endif  
  
  float zIndex = (abs(dist) - abs(zzNear))/far;
  if(zIndex < 0.0 || zIndex > 1.0 )
    return vec3(-1);
  float zGridIndex = texture1D(gridInvFunc, zIndex).a;
  return vec3(win.x/screenWidth, win.y/screenHeight, zGridIndex);
}

bool isVoxelEmpty(vec3 gridIndex)
{
  uvec4 cellContent = texture2D(voxelGrid, gridIndex.xy).rgba;
  unsigned int zIndex = unsigned int(min(floor(gridIndex.z*128.0 + 1.0), 128.0));

  return !isBitOne(cellContent[zIndex/32u], zIndex%32u);
}


bool isVoxelOne(uvec4 cell, unsigned int zIndex)
{
  int component = int(zIndex/32u);
  unsigned int position = zIndex%32u;
  return isBitOne(cell[component], position);
}

float getVoxelDensity(vec3 gridIndex)
{
  vec3 gridIndexCentralCell = gridIndex*vec3(screenWidth, screenHeight, 128) - vec3(0.5,0.5, -1.0);
  vec3 floorIndex = floor(gridIndexCentralCell);
  vec3 ceilIndex = ceil(gridIndexCentralCell);
  
  uvec4 cellContent1 = texture2D(voxelGrid, floorIndex.xy/vec2(screenWidth, screenHeight)).rgba;
  uvec4 cellContent2 = texture2D(voxelGrid, vec2(floorIndex.x, ceilIndex.y)/vec2(screenWidth, screenHeight)).rgba;
  uvec4 cellContent3 = texture2D(voxelGrid, vec2(ceilIndex.x, floorIndex.y)/vec2(screenWidth, screenHeight)).rgba;
  uvec4 cellContent4 = texture2D(voxelGrid, ceilIndex.xy/vec2(screenWidth, screenHeight)).rgba;
  
  unsigned int zIndex1 = unsigned int(min(floorIndex.z, 128.0));
  unsigned int zIndex2 = unsigned int(min(ceilIndex.z, 128.0));
  
  bool cell11 = isVoxelOne(cellContent1, zIndex1);
  bool cell12 = isVoxelOne(cellContent2, zIndex2);
  bool cell21 = isVoxelOne(cellContent3, zIndex1);
  bool cell22 = isVoxelOne(cellContent4, zIndex2);
  bool cell31 = isVoxelOne(cellContent1, zIndex1);
  bool cell32 = isVoxelOne(cellContent2, zIndex2);
  bool cell41 = isVoxelOne(cellContent3, zIndex1);
  bool cell42 = isVoxelOne(cellContent4, zIndex2);
  vec3 d = ceilIndex - gridIndexCentralCell;
  
  float i1 = (1.0-d.z)*float(cell11) + d.z*float(cell12);
  float i2 = (1.0-d.z)*float(cell21) + d.z*float(cell22);
  float j1 = (1.0-d.z)*float(cell31) + d.z*float(cell32);
  float j2 = (1.0-d.z)*float(cell41) + d.z*float(cell42);
  
  float w1 = i1*(1.0 - d.y) + i2*d.y;
  float w2 = j1*(1.0 - d.y) + j2*d.y;
  
  float c = w1*(1.0 - d.x) + w2*d.x;
  return c;
}

float normalizeAo(float ao, int numRays)
{
  return ao/float(numRays);
}



bool isBitOne(unsigned int v, unsigned int pos)
{
  unsigned int mask = 1u << (31u-pos);
  return ((v & mask) != 0u);
}

mat3 getRotationMatrix(float rad_ang, vec3 u)
{
  float c = cos(rad_ang);
  float s = sin(rad_ang);
  float o_c = 1. - c;
       
  //COLUMN MATRIX
  return mat3(c + u.x*u.x*o_c,      u.y*u.x*o_c + u.z*s,  u.z*u.x*o_c - u.y*s, 
              u.x*u.y*o_c - u.z*s,  c + u.y*u.y*o_c,      u.z*u.y*o_c + u.x*s, 
              u.x*u.z*o_c + u.y*s,  u.y*u.z*o_c - u.x*s,  c + u.z*u.z*o_c     );
}


vec3 eye2ndcByOrtho(vec3 eye)
{
	//--assuming r == -l, t == -b
	float xn = eye.x/right;
	float yn = eye.y/top;
	float zn = -2.0*eye.z/(far - near) - (far + near)/(far - near);
	
	return vec3(xn, yn, zn);
}

vec3 ndc2eyeByOrtho(vec3 ndc)
{
  //--assuming r == -l, t == -b
  float ze = -ndc.z*(far - near)/2.0 - (far + near)/2.0;
  float ye = ndc.y * top;
  float xe = ndc.x * right;
  
  return vec3(xe, ye, ze);
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

vec3 window2eye(vec3 w, bool perspective =  true)
{
   if(perspective)
    return ndc2eye(window2ndc(w));
   return ndc2eyeByOrtho(window2ndc(w));
}

vec3 eye2window(vec3 eye, bool perspective =  true)
{
  if(perspective)
    return ndc2window(eye2ndc(eye));
  return ndc2window(eye2ndcByOrtho(eye));
}




