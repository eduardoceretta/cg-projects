/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  OCt 2011
 *
 *  Perform the calculation of the Ambient Occlusion of the Scene.
 *  Uses a non regular grid as occlusion query.
 *  Verifies the number of full voxels inside n influence spheres to calculate the
 *  cone integral of each ray.
 */
#extension GL_EXT_gpu_shader4 : enable //Enable Integer Operations - Shader Model 4.0 GF 8 Series
#extension GL_ARB_gpu_shader5 : enable //Enable Some Bit Operations - Shader Model 5.0 

/****************************************************************************************/
/* Shader Controls.                                                                     */
/*  Each define specificates if a behaviour will affect the shader                      */
/****************************************************************************************/
//EYE_NEAREST_ENABLED
#define EYE_NEAREST_ENABLED  $REPLACE$     /**< Uses the information in the eyeNearest texture to get the nearest eye position of the fragment*/

//LIMITED_FAR_PERCENT 
/**< Uses a limited far disatance. The grid will not go to the far plane. Resulting in a greater resolution in the front voxels*/
#define LIMITED_FAR_ENABLED $REPLACE$
#define LIMITED_FAR_PERCENT $REPLACE$

//NORMAL_OFFSET 
#define NORMAL_OFFSET .01       /**< Modifies the center of each sphere so it does not cause same plane occlusion*/

/**< Uses texture to count the number of bits 1 in a integer*/
#define TEXTURE_BIT_COUNT_ENABLED  $REPLACE$   

#if !TEXTURE_BIT_COUNT_ENABLED
  #define BIT_EXTRACTION        /**< Uses glsl bitextract function instead of own function*/
#endif

#define SPHERE_SAMPLERS_DISTRIBUTIONS $REPLACE$
#define MAX_SPHERE_SAMPLERS           $REPLACE$

#define SPHERE_SAMPLER_DATA_SIZE      $REPLACE$
#define SAMPLING_DATA_SIZE            $REPLACE$

/****************************************************************************************/
/* Shader Begin.                                                                        */
/****************************************************************************************/
uniform sampler1D gridInvFunc;        /**< Inverse Function of the grid cell structure*/

uniform sampler2D eyePos;             /**< Eye Position(rgb) Nearest Eye Fragment(a) Texture*/
uniform sampler2D normalDepth;        /**< Normal(rgb) Depth(a)*/
uniform usampler2D voxelGrid;         /**< Non Uniform Voxel Grid of the Scene*/
//uniform sampler2D voxelGridDEBUG;     /**< Non Uniform Voxel Grid of the Scene DEBUG*/


/**< Index of the current position of the samplingData*/
int samplingIndex = 0;                

/**< Data cointaining sampling information for the spheresamplers. Size = MAX_SPHERE_SAMPLERS*SPHERE_SAMPLERS_DISTRIBUTIONS */
uniform vec2 sphereSamplerData[SPHERE_SAMPLER_DATA_SIZE];  

/**< Data cointaining all the sampling information*/
uniform float samplingData[SAMPLING_DATA_SIZE];      
// SamplingData Organization
//Size What
//[1]  Num Cones
//[3]    ConeDir
//[1]    NumSpheres
//[1]      SphereRadius
//[1]      SphereApexDistance
//[1]      NumSamplers

//[1]      SphereRadius
//[1]      SphereApexDistance
//[1]      NumSamplers
//[n]      ....
//[3]    ConeDir
//[1]    NumSpheres
//[n]       ....

#if TEXTURE_BIT_COUNT_ENABLED
uniform int bitCount16Height;         /**< BitCount Texture Height*/
uniform int bitCount16Width;          /**< BitCount Texture Width*/
uniform sampler2D bitCount16;         /**< Number of bits counter Texture(Max 16 bits)*/
#endif // TEXTURE_BIT_COUNT_ENABLED

/**
 * Algorithm Parameters
 */
uniform float rfarPercent;
uniform float contrast;
uniform int jitterEnabled;

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


#define getSphereCenter(eyePosition, coneDir, rfar, multiplier)  \
  ((eyePosition) + (coneDir)*(rfar)*(multiplier))


#define PI 3.14159265

#define ONE(x) \
  (1.0)

#define LINEAR(x) \
  (-(x)+1.0)
  
#define POLY(x) \
  (-(x)*(x) + 1.0)

#define SQRT(x) \
  (.3*(sqrt(2.0/((x) + .05)) - 1.0))

#define COS(x) \
  (cos((x)*PI/2.0))
  
#define LOG(x) \
  (.6*log(1.0/(x)))

#define EXP(x) \
  (pow(16.0, -(x)))
  
#define SIGMOID(x) \
  ((-1.0/(.5+pow(30.0, -((x)/.5 - .7)))+2.1)/2.0)
  
#define SQRT2(x) \
  (-pow((x),.05) + 1)
  


#define DIST_ATT(x) \
COS(x)
 //SIGMOID(x) 
 //SQRT2(x)
  //ONE(x)
 //SQRT(x)
 //LOG(x)
 //COS(x)
 //POLY(x)
 //LINEAR(x)
 //EXP(x)



#define RED vec4(.8,0,0,1)
#define GREEN vec4(0,.8,0,1)
#define BLUE vec4(0,0,.8,1)
#define CYAN vec4(0,.8,.8,1)
#define PINK vec4(.8,0,.8,1)
#define YELLOW vec4(.8,.8,0,1)
#define WHITE vec4(1,1,1,1)
#define BLACK vec4(0,0,0,1)
#define ORANGE vec4(1.,.5, 0., 1.)

#define OUT gl_FragData[0] 


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
 * Write in the parameters the input values read from the textures.
 */
void readInputData(out vec3 normal, out vec3 eyePosition, out float depth);

/**
 * Get the znear of the respective window coordinate.
 */
float getZnear(vec2 winCoordinate);

/**
 * Get the index z of the grid of the z eyecoordinate.
 *  Returns -1 if the index is less then zNear
 */
float getZGridIndex(float eyeZ, float zNear);

/**
 * Get the index z of the grid of the z eyecoordinate.
 *  If eyeZ is less then zNear return the zNear GridIndex
 *  If eyeZ is great then far return the far GridIndex
 */
float getClampedZGridIndex(float eyeZ, float zNear);

/**
 * Get the grid cell index that the ray touches in grid int coordinates(0-128).
 */
unsigned int getGridIndexNormalized(float gridIndex);

/**
 * Get the number of full voxels in the gridIndex between z top and botton index.
 */
unsigned int countFullVoxelsVolSphere(vec3 gridIndex, unsigned int gridIndexBottonN, unsigned int gridIndexTopN);

/**
 * Get the number of bits set in the range of indexStart and indexEnd.
 */
unsigned int countFullVoxelsInRange(unsigned int voxelContent, unsigned int indexStart, unsigned int indexEnd);

/**
 * Normalize the Ambient Occlusion based on the number of samplers
 */
float normalizeAo(float ao, int numSamplers);

/**
 * Gets the rotation matrix that rotates a direction into the hemisphere of the cone
 */
mat3 getHemisphereRotationMatrix(vec3 normal);

/**
 * Gets the data of the ith cone.
 *  out dir: cone direction in the position roationed by rotMat
 *  out num_spheres: number of spheres in this cone
 */
void getConeData(int coneDirSamplerDistributionIndex, int i, mat3 rotMat, out vec3 dir, out int num_spheres);


/**
 * Calculates the occlusion of the sphere at sphereCenter and with radius sphereRadius
 *  sphereCounter is the absolute (to all spheres in the hemisphere) index of the sphere 
 *  Returns how much of the volume of the sphere is occluded
 */
float calcSphereOcclusion(vec3 sphereCenter, float sphereRadius, int sphereCounter);

/**
 * Calculates the occlusion of prism defined by the sampler sphereSampler
 *  Writes the value of the secant. It is the total height of the prism.
 *  Returns how much of the volume of the prism is occluded
 */
float calcSphereAo(vec3 sphereSampler, vec3 sphereCenter, float sphereRadius, out float secant);

/**
 * Reads the Number of Cones from sampligData current position
 */
int readNumCones();

/**
 * Reads the direction of the cone from sampligData current position. Rotates it using the rotMat
 */
vec3 readCurConeDir(mat3 rotMat);

/**
 * Reads the Number of Spheres from sampligData current position
 */
int readCurConeNumSpheres();

/**
 * Reads the Sphere radius (normalized[0,1]) from sampligData current position.
 *   Must be called before readCurSphereDist
 */
float readCurSphereRadius();

/**
 * Reads the Sphere distance to the cone's apex (normalized[0,1]) from sampligData current position.
 */
float readCurSphereDist();

/**
 * Reads the Number of Spheres Samplres from sampligData current position
 */
int readCurSphereNumSamplers();

/**
 * Reads the Sphere Sampler from sphereSamplerData. 
 *   distribIndex and i are used to access the sphereSamplerData.
 *   Multiplies by the radius of sphere.
 */
vec3 readSphereSampler(int distribIndex, int i, float radius);

/**
 * Generate a random distribution index based on the center of the sphere
 */
int getSphereSamplerDistributionIndex(vec3 sphereCenter);

vec3 getGridCellSize(float zNear);

/************************************************************************/
/************************************************************************/
/************************************************************************/
/*******************************MAIN*************************************/
/*******************************MAIN*************************************/
/*******************************MAIN*************************************/
/*******************************MAIN*************************************/
/************************************************************************/
/************************************************************************/
/************************************************************************/
/************************************************************************/

void main()
{
//if(gl_FragCoord.x == 320.5 && gl_FragCoord.y == 240.5)
//{
  //OUT = BLACK;
  //return;
//}
  
  vec3 normal; vec3 eyePosition; float depth;
  readInputData(normal, eyePosition, depth);
  
  if(depth < 0.0)
    discard;

  /**
	 * TEST NORMAL
	 *  Print the Normal's Color.
	 *  Result must be Blue for normals facing the screen
   * /
    vec3 nn = (normal.xyz)*.5 + .5;
    //vec3 nn = (normal.xyz);
    gl_FragData[0] = vec4(nn, 1.);
    //gl_FragData[0] = RED;
        return;
  /**/
  
  vec3 voxelSize = getGridCellSize(near);
  float halfDiagonal = sqrt(voxelSize.x*voxelSize.x + voxelSize.y*voxelSize.y + voxelSize.z*voxelSize.z)/2.0;
  /**/
  float rfar = rfarPercent*far;
  float ao = 0.0;
  float sumdot = 0.0;
    
  mat3 rotMat = getHemisphereRotationMatrix(normal);
  
  int numCones = readNumCones();
  //vec3 coneDir = readCurConeDir(rotMat);
  //
  //OUT = vec4(coneDir, 1);
  //return;
  
  int sphereCounter = 0;
  
  for(int i = 0; i < numCones; ++i)
  {
    vec3 coneDir = readCurConeDir(rotMat);
    int numSpheres = readCurConeNumSpheres();
    
    float coneAo = 0.0;
    
    float firstSphereRadius = readCurSphereRadius()*rfar;
    float d0 = abs(halfDiagonal/(dot(normal,coneDir))) + firstSphereRadius;
    //Need to return the index one position
    samplingIndex = samplingIndex - 1;
    
    for(int j = 0; j < numSpheres; ++j)
    {
      float sphereRadius = readCurSphereRadius()*rfar;
      float normalizedSphereDist = readCurSphereDist();

      float dist = normalizedSphereDist*(rfar - d0) + d0;
      
      //vec3 sphereCenter = getSphereCenter(eyePosition + normal*NORMAL_OFFSET*rfar, coneDir, rfar, sphereInfoData.x);//.06
      vec3 sphereCenter = getSphereCenter(eyePosition + normal*NORMAL_OFFSET*rfar, coneDir, 1.0, dist);

      float sphereAo = calcSphereOcclusion(sphereCenter, sphereRadius, sphereCounter++);
      
      float distAtt = DIST_ATT(normalizedSphereDist);
      //float distAtt = 1.0;
            
      coneAo += sphereAo*distAtt*(1.0 - coneAo);
      coneAo = min(coneAo, 1.0);
      //if(coneAo>=1.0)
        //break;
    }
    float dt = dot(normalize(normal),normalize(coneDir));
    sumdot += dt;
    ao += coneAo*dt;
    //ao += coneAo;
  }

  ao = ao/sumdot;
  //ao = normalizeAo(ao, numCones);
  ao = clamp(ao*contrast, 0.0, 1.0); 

  gl_FragData[0] = WHITE*(1.0 - ao);
  
  /*************************\
         Debug Area 
  \*************************/
  /**/
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/*******************************END MAIN*************************************/
/*******************************END MAIN*************************************/
/*******************************END MAIN*************************************/
/*******************************END MAIN*************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/


void readInputData(out vec3 normal, out vec3 eyePosition, out float depth)
{
  vec2 fragCoord = gl_FragCoord.xy/vec2(screenWidth, screenHeight);
  vec4 normalData = texture2D(normalDepth, fragCoord);
  normal = normalize(normalData.xyz);
  depth = normalData.a;
  
  vec4 eyePosData = texture2D(eyePos, fragCoord);
  eyePosition = eyePosData.xyz;
}

float getZnear(vec2 winCoordinate)
{
#if EYE_NEAREST_ENABLED
  return texture2D(eyePos, winCoordinate/vec2(screenWidth, screenHeight)).a;
#else  
  return near;
#endif
}

float getZGridIndex(float eyeZ, float zNear)
{
#if LIMITED_FAR_ENABLED
  float zIndex = (-eyeZ - zNear)/(LIMITED_FAR_PERCENT*(far - zNear));
#else  
  float zIndex = (-eyeZ - zNear)/(far - zNear);
#endif  
    
  if(zIndex < 0.0 || zIndex > 1.0)
    return -1.;
  return texture1D(gridInvFunc, zIndex).a;
}

float getClampedZGridIndex(float eyeZ, float zNear)
{
#if LIMITED_FAR_ENABLED
  float zIndex = clamp((-eyeZ - zNear)/(LIMITED_FAR_PERCENT*(far - zNear)), 0.0, 1.0);
#else  
  float zIndex = clamp((-eyeZ - zNear)/(far - zNear), 0.0, 1.0);
#endif  
    
  return texture1D(gridInvFunc, zIndex).a;
}

unsigned int getGridIndexNormalized(float gridIndex)
{
  return  unsigned int(floor(gridIndex*128.0 + 0.5));
}

mat3 getHemisphereRotationMatrix(vec3 normal)
{
  vec3 hemisphereNormal = vec3(0.0, 1.0, 0.0);

  float angle = acos(dot(hemisphereNormal, normal));
  vec3 axis = normalize(cross(hemisphereNormal, normal));
  
  return getRotationMatrix(angle, axis);
}  

int readNumCones()
{
  return int(floor(samplingData[samplingIndex++] + .5));
}

vec3 readCurConeDir(mat3 rotMat)
{
  vec3 coneDir = vec3(samplingData[samplingIndex++], samplingData[samplingIndex++], samplingData[samplingIndex++]);
  return normalize(rotMat*coneDir);
}

int readCurConeNumSpheres()
{
  return int(floor(samplingData[samplingIndex++] + .5));
}

float readCurSphereRadius()
{
  return samplingData[samplingIndex++];
}

float readCurSphereDist()
{
  return samplingData[samplingIndex++];
}

int readCurSphereNumSamplers()
{
  return int(floor(samplingData[samplingIndex++] + .5));
}

vec3 readSphereSampler(int distribIndex, int i, float radius)
{
  vec3 sphereSampler = vec3(sphereSamplerData[distribIndex + i], 0.0);
  return sphereSampler*radius;
}

int getSphereSamplerDistributionIndex(vec3 sphereCenter)
{
  int randNum = int(floor(rand(sphereCenter.xy) * float(SPHERE_SAMPLERS_DISTRIBUTIONS)));
  return randNum*MAX_SPHERE_SAMPLERS;
}

float calcSphereOcclusion(vec3 sphereCenter, float sphereRadius, int sphereCounter)
{
  float ao = 0.0;
  float s = 0.0;

  int numSphereSamplers = readCurSphereNumSamplers();
  
  int distribIndex;
  if(jitterEnabled)
    distribIndex = getSphereSamplerDistributionIndex(sphereCenter);
  else 
    distribIndex = (sphereCounter%SPHERE_SAMPLERS_DISTRIBUTIONS)*MAX_SPHERE_SAMPLERS;
        
  for(int i = 0; i < numSphereSamplers; ++i)
  {
    vec3 sphereSampler = readSphereSampler(distribIndex, i, sphereRadius*.99);
    
    float secant;
    ao += calcSphereAo(sphereSampler, sphereCenter, sphereRadius, secant);

    s += secant;
  }
  if(s != 0.0)
    ao = ao/s;
  
  return ao;
}

float calcSphereAo(vec3 sphereSampler, vec3 sphereCenter, float sphereRadius, out float secant)
{
  unsigned int gridIndexTopN  = 0u;
  unsigned int gridIndexBottonN = 0u;
  secant = 0.0;

  float d = length(sphereSampler);
  float zi = sqrt(sphereRadius*sphereRadius - d*d) ;
  float zo = -zi;
  secant = 2.0*zi;

  vec3 point = sphereCenter + sphereSampler;
  vec3 win = eye2window(point, bool(perspective));
  float zzNear = getZnear(win.xy);
  
  if(zzNear < 0.0)
    return 0.0;

  float zGridIndex = getZGridIndex(point.z, zzNear);
  vec3 samplerGridIndex = vec3(win.x/screenWidth, win.y/screenHeight, zGridIndex);

  float gridIndexTop  = getClampedZGridIndex(point.z + zi, zzNear); //Closer to The Eye
  float gridIndexBotton = getClampedZGridIndex(point.z + zo, zzNear);

  gridIndexBottonN = getGridIndexNormalized(gridIndexBotton);
  gridIndexTopN = getGridIndexNormalized(gridIndexTop);
  
  if(samplerGridIndex.x < 0.0)
    return 0.0;
    
  if(gridIndexBottonN == gridIndexTopN)
    return 0.0;

  unsigned int numFullVoxels = countFullVoxelsVolSphere(samplerGridIndex, gridIndexBottonN, gridIndexTopN);
#if LIMITED_FAR_ENABLED
  float height = min(float(numFullVoxels)*(LIMITED_FAR_PERCENT*(far - zzNear))/128.0, secant);
#else  
  float height = min(float(numFullVoxels)*(far - zzNear)/128.0, secant);
#endif  
  return height;
}

  
unsigned int countFullVoxelsInRange(unsigned int voxelContent, unsigned int indexStart, unsigned int indexEnd)
{
  voxelContent = voxelContent & (4294967295u >> indexStart);
  voxelContent = voxelContent >> (31u -  indexEnd);
  unsigned int count = 0u;
#if TEXTURE_BIT_COUNT_ENABLED  
  unsigned int i = 0u;
  vec2 bitCountSize = vec2(bitCount16Width, bitCount16Height);
  while(i <= indexEnd - indexStart)
  {
    unsigned int t = voxelContent & (4294967295u >> 16);
    vec2 coord = vec2(float(t%unsigned int(bitCount16Width)) + .5, float(t/unsigned int(bitCount16Width)) + .5);
    float numBits = texture2D(bitCount16, coord/bitCountSize).a;
    count += unsigned int(numBits*255.0);
    voxelContent = voxelContent >> 16u;
    i+=16u;
  }
#else
  count = bitCount(voxelContent);
#endif  
  return count;
}

unsigned int countFullVoxelsVolSphere(vec3 gridIndex, unsigned int gridIndexBottonN, unsigned int gridIndexTopN)
{
  //BIT SCHEME:
  //R - 0 ... 31, G - 32 ... 63, B - 64 ... 95, A - 92 ... 127
  //   TOP                                               BOTTON
  //cellContent = uvec4(2565681391u, 383192u, 36815332u, 494537323u);
  /*                                                                                                     1  1 1  1 1  1 1  1 1  1 1 1
  0  0 0  0 1  1 1  1 2  2 2  2 33%33 3  3 4  4 4  4 5  5 5  5 6  6%66  6 7  7 7  7 8  8 8  8 9  9 9%9 9 0  0 0  0 1  1 1  1 2  2 2 2   
  0..3.5..8.0..3.5..8.0..3.5..8.01%23.5..8.0..3.5..8.0..3.5..8.0..3%45..8.0..3.5..8.0..3.5..8.0..3.5%6.8.0..3.5..8.0..3.5..8.0..3.5.7
  10011000111011010011000011101111%00000000000001011101100011011000%00000010001100011100000111100100%00011101011110100000101001101011
               18                               10                                    11                              16
  /**/
  //////////////////////////////////////////////////////////////////////
 
  uvec4 cellContent = texture2D(voxelGrid, gridIndex.xy).rgba;
  unsigned int numFullVoxels = 0u;
/**/
#ifndef BIT_EXTRACTION
  unsigned int bottonContentColor = gridIndexBottonN/32u;
  unsigned int topContentColor = gridIndexTopN/32u;
   
  for(unsigned int i = topContentColor; i <= bottonContentColor; ++i)
  {
    if(i == topContentColor && i == bottonContentColor)
    {
      numFullVoxels += countFullVoxelsInRange(cellContent[i], gridIndexTopN%32u, gridIndexBottonN%32u);
    }else if(i == topContentColor)
    {
      numFullVoxels += countFullVoxelsInRange(cellContent[i], gridIndexTopN%32u, 31u);
    }else if(i == bottonContentColor)
    {
      numFullVoxels += countFullVoxelsInRange(cellContent[i], 0u, gridIndexBottonN%32u);
    }else
    {
      numFullVoxels += countFullVoxelsInRange(cellContent[i], 0u, 31u);
    }
  }
#else  
  int tmax32 = max(0, gridIndexTopN-32);
  int tmax64 = max(0, gridIndexTopN-64);
  int tmax96 = max(0, gridIndexTopN-96);
  
  int top[4];
  top[0] = min(31, gridIndexTopN);
  top[1] = min(31, tmax32);
  top[2] = min(31, tmax64);
  top[3] = min(31, tmax96);
  
  int bot[4];
  bot[0] = max(0, min(31, gridIndexBottonN) - gridIndexTopN + 1);
  bot[1] = max(0, min(31, gridIndexBottonN-32) - tmax32 + 1);
  bot[2] = max(0, min(31, gridIndexBottonN-64) - tmax64 + 1);
  bot[3] = max(0, min(31, gridIndexBottonN-96) - tmax96 + 1);

  numFullVoxels += bitCount(bitfieldExtract(cellContent.r, 32 - top[0] - bot[0], bot[0]));
  numFullVoxels += bitCount(bitfieldExtract(cellContent.g, 32 - top[1] - bot[1], bot[1]));
  numFullVoxels += bitCount(bitfieldExtract(cellContent.b, 32 - top[2] - bot[2], bot[2]));
  numFullVoxels += bitCount(bitfieldExtract(cellContent.a, 32 - top[3] - bot[3], bot[3]));
#endif
  return numFullVoxels;
}  

float normalizeAo(float ao, int numSamplers)
{
  return ao/float(numSamplers);
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


vec3 getGridCellSize(float zNear)
{
  float xm; 
  float ym;

  if(!perspective)
  {
    xm = (2.0*right)/screenWidth; 
    ym = (2.0*top)/screenHeight;
  }else
  {
    xm = (right*(far - near)/(2.0*near))/screenWidth; //Half Frustum Width
    ym = (top*(far - near)/(2.0*near))/screenHeight;//Half Frustum Height 
  }

#if LIMITED_FAR_ENABLED
  #if EYE_NEAREST_ENABLED
    float zfar = LIMITED_FAR_PERCENT*far + zNear;
  #else
    float zfar = LIMITED_FAR_PERCENT*far + near;
  #endif
#else  
  float zfar = far;
#endif

#if EYE_NEAREST_ENABLED
  float zm = (zfar - zNear)/128.0;
#else
  float zm = (zfar - near)/128.0;
#endif //EYE_NEAREST_ENABLED

  return vec3(xm, ym, zm);
}

/**/