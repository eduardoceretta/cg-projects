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

/****************************************************************************************/
/* Shader Controls.                                                                     */
/*  Each define specificates if a behaviour will affect the shader                      */
/****************************************************************************************/
//EYE_NEAREST
#define EYE_NEAREST          /**< Uses the information in the eyeNearest texture to get the nearest eye position of the fragment*/
//NORMAL_OFFSET 
#define NORMAL_OFFSET .01   /**< Modifies the center of each sphere so it does not cause same plane occlusion*/
//JITTER
//#define JITTER               /**< Enable pixel sampler Jitter*/

#define TEXTURE_SPHERE_SAMPLERS  /**< Uses texture to access the sphere samplers*/

/****************************************************************************************/
/* Shader Begin.                                                                        */
/****************************************************************************************/
uniform sampler1D gridInvFunc;        /**< Inverse Function of the grid cell structure*/

uniform sampler2D eyePos;             /**< Eye Position(rgb) Nearest Eye Fragment(a) Texture*/
uniform sampler2D normalDepth;        /**< Normal(rgb) Depth(a)*/
uniform usampler2D voxelGrid;         /**< Non Uniform Voxel Grid of the Scene*/
//uniform sampler2D voxelGridDEBUG;     /**< Non Uniform Voxel Grid of the Scene DEBUG*/

uniform int numCones;                 /**< Number of sampler cones*/
uniform int numSpheresByCone;         /**< Number of spheres used in each cones tracing*/
uniform int numSphereSamplers;        /**< Number of samplers in each sphere*/
uniform int numSamplersDistributions; /**< Number of samplers distributions*/

uniform int coneDirSamplersWidth;     /**< Sampler Textures Width*/
uniform sampler1D coneDirSamplers;    /**< Cone directions Samplers Texture*/

uniform sampler1D sphereInfo;          /**< Sphere Info Texture*/
uniform int sphereInfoWidth;           /**< Sphere Info Texture Width*/

uniform int bitCount16Height;         /**< BitCount Texture Height*/
uniform int bitCount16Width;          /**< BitCount Texture Width*/
uniform sampler2D bitCount16;         /**< Number of bits counter Texture(Max 16 bits)*/

//uniform mat4 projInv;


/**
 * Algorithm Parameters
 */
uniform float rfarPercent;
uniform float contrast; 
uniform int jitterEnabled;            /**< Enables texture sampler jittering*/

 
 
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



#define PROG_A0 4.0
#define PROG_STEP 2.0
#define ARITPROG_AN(a, d, n) int((a) + ((n-1.0)*(d)))
#define ARITPROG_SUM(a, d, n) int(((n)*((a)+ARITPROG_AN(a,d,n))/2.0))

//getNumSphereSamplers
#define getNumSphereSamplers(n) \
  (numSphereSamplers)
  //ARITPROG_AN(PROG_A0, PROG_STEP, (n)+1)

//getNumSphereSamplersAccumulated
#define getNumSphereSamplersAccumulated(n) \
  (numSphereSamplers*n)
  //(0)
    //ARITPROG_SUM(PROG_A0, PROG_STEP, (n))
  
//getTotalNumSphereSamplersAccumulated
#define getTotalNumSphereSamplersAccumulated(numSpheresByCone) \
  (getNumSphereSamplers(1)*numSpheresByCone)
  //(getNumSphereSamplers(1))
    //getNumSphereSamplersAccumulated(numSpheresByCone)
  
  
  
#ifdef TEXTURE_SPHERE_SAMPLERS 
  uniform sampler1D sphereSamplers;     /**< Sphere Sampler Texture*/
  uniform int sphereSamplersWidth;      /**< Sphere Sampler Texture Width*/
#else
  uniform vec3 sphereSamplersArray[getNumSphereSamplers(1)*3]; /**< Uniform Sphere samplers*/
#endif //TEXTURE_SPHERE_SAMPLERS 


#define getSphereCenter(eyePosition, coneDir, rfar, multiplier)  \
  ((eyePosition) + (coneDir)*(rfar)*(multiplier))

#define getSphereRadius(rfar, multiplier) \
  ((multiplier)*(rfar))

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
  ONE(x)
 //SQRT2(x)
 //SIGMOID(x) 
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
 * Return True if the bit in the position pos of the unsigned int v is One
 */
bool isBitOne(unsigned int v, unsigned int pos);

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
 * Normalize the Ambient Occlusion based on the number of samplers
 */
float normalizeAo(float ao, int numSamplers);

mat3 getHemisphereRotationMatrix(vec3 normal);
vec3 getConeDir(int coneDirSamplerDistributionIndex, int i, mat3 rotMat);
float calcSphereOcclusion(int coneDirSamplerDistributionIndex, vec3 sphereCenter, float sphereRadius, int index);
float normalizeConeAo(float coneAo, int numSpheresByCone);
vec3 getSphereSampler(int samplerDistIndex, int sphereIndex, int samplerIndex, float radius);
float calcSphereAo(vec3 sphereSampler, vec3 sphereCenter, float sphereRadius, out float secant);
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

  int coneDirSamplerDistributionIndex = 0;
  int sphereSamplerDistributionIndex = 0;
  int sphereInfoDistributionIndex = 0;
    
  if(jitterEnabled == 1)
  {
    //vec3 fragGridIndex = getGridIndex(vec3(0,0,0), eyePosition);
    //int randNum = int(floor(rand(fragGridIndex.xy) * float(numSamplersDistributions)));
    int randNum = int(floor(rand(gl_FragCoord.xy/vec2(screenWidth, screenHeight)) * float(numSamplersDistributions)));
    
    coneDirSamplerDistributionIndex = randNum * numCones;
    sphereSamplerDistributionIndex = randNum * getTotalNumSphereSamplersAccumulated(numSpheresByCone);
    sphereInfoDistributionIndex = randNum * numSpheresByCone * numCones;
  }  
  

  /**
	 * TEST NORMAL
	 *  Print the Normal's Color.
	 *  Result must be Blue for normals facing the screen
   * /
    vec3 nn = (normal.xyz)*.5 + .5;
    //vec3 nn = (normal.xyz);
    gl_FragData[0] = vec4(nn, 1.);
    return;
  /**/
  
  
  
  
  
  vec3 voxelSize = getGridCellSize(near);
  float halfDiagonal = sqrt(voxelSize.x*voxelSize.x + voxelSize.y*voxelSize.y + voxelSize.z*voxelSize.z)/2.0;
  /**/
  float rfar = rfarPercent*far;
  float ao = 0.0;
  
  mat3 rotMat = getHemisphereRotationMatrix(normal);
  
  for(int i = 0; i < numCones; ++i)
  {
    vec3 coneDir = getConeDir(coneDirSamplerDistributionIndex, i, rotMat);
    float coneAo = 0.0;
    
    float sphereInfoIndex = (float(sphereInfoDistributionIndex + i*numSpheresByCone + 0) + .5)/sphereInfoWidth;
    float firstSphereRadius = texture1D(sphereInfo, sphereInfoIndex).a*rfar;
    
    for(int j = 0; j < numSpheresByCone; ++j)
    {
      float sphereInfoIndex = (float(sphereInfoDistributionIndex + i*numSpheresByCone + j) + .5)/sphereInfoWidth;
      vec2 sphereInfoData = texture1D(sphereInfo, sphereInfoIndex).ra;
      
      float d0 = abs(halfDiagonal/(dot(normal,coneDir))) + firstSphereRadius;
      float dist = sphereInfoData.x*(rfar - d0) + d0;
      
      float normalizedDist = sphereInfoData.x;
      //vec3 sphereCenter = getSphereCenter(eyePosition + normal*.06*rfar, coneDir, rfar, sphereInfoData.x);//.06
      vec3 sphereCenter = getSphereCenter(eyePosition + normal*NORMAL_OFFSET*rfar, coneDir, 1.0, dist);
            
      float sphereRadius = getSphereRadius(rfar, sphereInfoData.y);

      float sphereAo = calcSphereOcclusion(sphereSamplerDistributionIndex, sphereCenter, sphereRadius, j);
      
      float distAtt = DIST_ATT(normalizedDist);
      
      coneAo += sphereAo*distAtt*(1.0 - coneAo);
      if(coneAo>=1.0)
        break;
    }
    //coneAo = normalizeConeAo(coneAo, numSpheresByCone);

    ao += coneAo;
  }
  

  ao = normalizeAo(ao, numCones);
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
#ifdef EYE_NEAREST
  return texture2D(eyePos, winCoordinate/vec2(screenWidth, screenHeight)).a;
#else  
  return near;
#endif
}

float getZGridIndex(float eyeZ, float zNear)
{
  float zIndex = (-eyeZ - zNear)/far;
  if(zIndex < 0.0 || zIndex > 1.0)
    return -1.;
  return texture1D(gridInvFunc, zIndex).a;
}

float getClampedZGridIndex(float eyeZ, float zNear)
{
  float zIndex = clamp((-eyeZ - zNear)/far, 0.0, 1.0);
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

vec3 getConeDir(int coneDirSamplerDistributionIndex, int i, mat3 rotMat)
{
  float coneIndex = (float(coneDirSamplerDistributionIndex + i) + .5)/float(coneDirSamplersWidth);
  vec3 coneDir = texture1D(coneDirSamplers, coneIndex).rgb;
  return normalize(rotMat*coneDir);
}
    
float calcSphereOcclusion(int sphereSamplerDistributionIndex, vec3 sphereCenter, float sphereRadius, int index)
{
  float ao = 0.0;
  float s = 0.0;

  int numSphereSamplers = getNumSphereSamplers(index);
  int sphereIndex = getNumSphereSamplersAccumulated((index));
    
  for(int i = 0; i < numSphereSamplers; ++i)
  {
    vec3 sphereSampler = getSphereSampler(sphereSamplerDistributionIndex, sphereIndex, i, sphereRadius*.99);
    
    float secant;
    ao += calcSphereAo(sphereSampler, sphereCenter, sphereRadius, secant);

    s += secant;
  }
  if(s != 0.0)
    ao = ao/s;
  
  return ao;
}

vec3 getSphereSampler(int samplerDistIndex, int sphereIndex, int samplerIndex, float radius)
{
#ifdef TEXTURE_SPHERE_SAMPLERS
  float sphereSamplerIndex = (float(samplerDistIndex + sphereIndex + samplerIndex) + .5)/float(sphereSamplersWidth);
  vec3 sphereSampler = texture1D(sphereSamplers, sphereSamplerIndex).rgb;
#else  
  vec3 sphereSampler = sphereSamplersArray[samplerDistIndex + sphereIndex + samplerIndex];
#endif  //TEXTURE_SPHERE_SAMPLERS
  return sphereSampler*radius;
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
  float height = min(float(numFullVoxels)*(far - zzNear)/128.0, secant);
  return height;
}

float normalizeConeAo(float coneAo, int numSpheresByCone)
{
  return coneAo/float(numSpheresByCone);
}
  
unsigned int countFullVoxelsInRange(unsigned int voxelContent, unsigned int indexStart, unsigned int indexEnd)
{
  voxelContent = voxelContent & (4294967295u >> indexStart);
  voxelContent = voxelContent >> (31u -  indexEnd);
  unsigned int i = 0u;
  unsigned int count = 0u;
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

  unsigned int bottonContentColor = gridIndexBottonN/32u;
  unsigned int topContentColor = gridIndexTopN/32u;
  
  unsigned int numFullVoxels = 0u;
  
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
  return numFullVoxels;
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
  
  float zIndex = (dist - zzNear)/far;
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

float normalizeAo(float ao, int numSamplers)
{
  return ao/float(numSamplers);
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

#ifdef EYE_NEAREST
  float zm = (far - zNear)/128.0;
#else
  float zm = (far - near)/128.0;
#endif //EYE_NEAREST

  return vec3(xm, ym, zm);
}

/**/