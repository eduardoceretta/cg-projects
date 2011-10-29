/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Sep 2011
 *
 *  Perform the calculation of the Ambient Occlusion of the Scene.
 *  Uses a non regular grid as occlusion query.
 *  Verifies the number of full voxels inside the influence sphere.
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
uniform sampler1D gridInvFunc;        /**< Inverse Function of the grid cell structure*/

uniform sampler2D eyePos;             /**< Eye Position(rgb) Nearest Eye Fragment(a) Texture*/
uniform sampler2D normalDepth;        /**< Normal(rgb) Depth(a)*/
uniform usampler2D voxelGrid;         /**< Non Uniform Voxel Grid of the Scene*/
uniform sampler2D voxelGridDEBUG;     /**< Non Uniform Voxel Grid of the Scene DEBUG*/

uniform int numSamplers;              /**< Number of samplers*/
uniform int numSamplersDistributions; /**< Number of samplers distributions*/
uniform int samplersWidth;            /**< Sampler Textures Width*/
uniform sampler1D samplers;           /**< Samplers Texture*/

uniform int bitCount16Height;         /**< BitCount Texture Height*/
uniform int bitCount16Width;          /**< BitCount Texture Width*/
uniform sampler2D bitCount16;         /**< Number of bits counter Texture(Max 16 bits)*/

uniform mat4 projInv;

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
 * Get the sampler vector based on the fragment index and scales it to rfar size
 */
vec3 getSampler(int samplerDistIndex, int i, float rfar);

/**
 * Get the sampler point in grid space.
 *  Return the grid index of the z-perpendicular sampler and return the higher and lower z voxel index.
 */
vec3 getSamplerPointGridIndexVolSphere(vec3 pSampler, vec3 eyePosition, vec3 normal, float rfar, out unsigned int gridIndexBottonN, out unsigned int gridIndexTopN, out float secant);

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


void main()
{
  int samplerDistributionIndex = int(floor(rand(gl_FragCoord.xy)*float(numSamplersDistributions)))*numSamplers;
  
  vec3 normal;
  vec3 eyePosition;
  float depth;
  
  readInputData(normal, eyePosition, depth);
  if(depth < 0.0)
    discard;
  
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
  
  
  //float rfar = .5*far;
  //float ao = 0.0;
  //for(int i = 0; i < numSamplers; ++i)
  //{
    //vec3 sampler = getSampler(samplerDistributionIndex, i, rfar);
//
    //unsigned int gridIndexTopN, gridIndexBottonN;
    //vec3 gridIndex = getSamplerPointGridIndex(sampler, eyePosition, normal, rfar, gridIndexBottonN, gridIndexTopN);
//
    //if(gridIndex.z < 0.0)
      //continue;
//
    //float percentFullVoxels = countFullVoxels(gridIndex, gridIndexBottonN, gridIndexTopN);
//
    //ao += percentFullVoxels;
  //}
  //
    //
  //ao = normalizeAo(ao, numSamplers);
  //ao = clamp(ao, 0.0, 1.0);
//
  //gl_FragData[0] = WHITE*(1.0 - ao);
  /**/


  /*************************\
         Debug Area 
  \*************************/
  float ao = 0.0;
  float s = 0.0;
  float rfar = .05*far/2.0;
  for(int i = 0; i < numSamplers; ++i)
  {
    vec3 sampler = getSampler(0, i, rfar*.99);
    //vec3 sampler = getSampler(samplerDistributionIndex, i, rfar*.99);
    //int num = 5;
    //vec3 sampler = vec3(cos(i%num*2*PI/(num)),sin(i%num*2*PI/(num)),0)*pow(((i/num) + 0.99)*(1./num),1.0)*rfar;
    //vec3 sampler = vec3(cos(i%8*2*PI/(8)),sin(i%8*2*PI/(8)),0)*pow(((i/5) + 0.99)*.2,12.0)*rfar;
    
    
    
    //vec3 sampler;
    ////sampler = vec3(0,1,0)*((3) + 0.99)*rfar*.2;
    //if(i%4==0)
      //sampler = vec3(0,1,0)*((i/5) + 0.99)*rfar*.2;
    //if(i%4==1)
      //////sampler = vec3(0,-1,0)*((i/5) + 0.99)*rfar*.2;
      //sampler = vec3(0,-1,0)*((i/5) + 0.99)*rfar*.2;
    //if(i%4==2)
      ////sampler = vec3(1,0,0)*((1) + 0.99)*rfar*.2;
      //sampler = vec3(1,0,0)*((i/5) + 0.99)*rfar*.2;
    //if(i%4==3)
      //sampler = vec3(-1,0,0)*((i/5) +0.99)*rfar*.2;
    ////else continue;
      //

    unsigned int gridIndexTopN, gridIndexBottonN;
    float secant;
    vec3 gridIndex = getSamplerPointGridIndexVolSphere(sampler, eyePosition/* + normal*(0.02)*far*/, normal, rfar, gridIndexBottonN, gridIndexTopN, secant);
    s += secant;
    
    
    
   float d = length(sampler);
  float zi = sqrt(rfar*rfar - d*d) ;
  float zo = -zi;
  vec3 point = eyePosition + normal*rfar + sampler;
  vec3 win = eye2window(point, bool(perspective));
  float zzNear = getZnear(win.xy);
  

    

    /******************************************************/
    if(all(equal(gl_FragCoord.xy, vec2(256.5, 256.5))) && i%4 == 2
         //||all(equal(gl_FragCoord.xy, vec2(349.5, 256.5)))
     //||all(equal(gl_FragCoord.xy, vec2(350.5, 256.5)))
     )
    {
    
      vec3 point = eyePosition + normal*rfar + sampler;
      vec3 win = eye2window(point, bool(perspective));
      float zzNear = getZnear(win.xy);
      //if(zzNear < 0.0)
        //return vec3(-3.0);
         
      float zGridIndex = getZGridIndex(point.z, zzNear);
      //if(zGridIndex < 0.0)
        //return vec3(-1.0);
        
      vec3 samplerGridIndex = vec3(win.x/screenWidth, win.y/screenHeight, zGridIndex);
      
      float d = length(sampler);
      float zi = sqrt(rfar*rfar - d*d) ;
      float zo = -zi;
      
      float zInGridIndex  = getClampedZGridIndex(point.z + zi, zzNear); //Closer to The Eye
      float zOutGridIndex = getClampedZGridIndex(point.z + zo, zzNear);
      
      float gridIndexTop = zInGridIndex;
      float gridIndexBotton = zOutGridIndex;
      
      //uvec4 cellContent = texture2D(voxelGrid, gridIndex.xy).rgba;
    
      gl_FragData[0] = vec4(zGridIndex, win.x, gridIndexBottonN, gridIndexTopN);
      //gl_FragData[0] = RED;
      gl_FragData[1] = texture2D(voxelGridDEBUG, gridIndex.xy).rgba;
      return;
    }
    /******************************************************/
   

    if(gridIndex.x < 0.0)
      continue;
      
    if(gridIndexBottonN == gridIndexTopN)
      continue;
    
    if(gridIndexBottonN < gridIndexTopN)
    {
      //gl_FragData[0] = vec4(point.z + zi, point.z + zn, point.z, zzNear);
      gl_FragData[0] = RED;
      return;
      //continue;
    }

    unsigned int numFullVoxels = countFullVoxelsVolSphere(gridIndex, gridIndexBottonN, gridIndexTopN);
    float zznear = getZnear(gridIndex.xy*vec2(screenWidth, screenHeight));
    float height = min(float(numFullVoxels)*(far - zznear)/128.0, secant);
    //if(height > secant && numFullVoxels == gridIndexBottonN - gridIndexTopN + 1)
    //{
      //gl_FragData[0] = ORANGE;
      //return;
    //}
    ao += height;
    //s +=secant;
    
    //unsigned int numVoxels = gridIndexBottonN - gridIndexTopN + 1;
    //float sHeight = numVoxels*far/128.0;
    //s += sHeight;

    /**/
  }
  
  //ao = normalizeAo(ao, numSamplers);
  //ao = ao/(rfar * 2.0 * s);
  if(s != 0.0)
    ao = ao/(s);
  //ao = ao*(3.0/(2.0*rfar*2.0*numSamplers));
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


vec3 getSampler(int samplerDistIndex, int i, float rfar)
{
  float samplerIndex = (float(samplerDistIndex + i) + .5)/float(samplersWidth);
  vec3 sampler = texture1D(samplers, samplerIndex).rgb;
  return sampler*rfar;
}

unsigned int getGridIndexNormalized(float gridIndex)
{
  return  unsigned int(floor(gridIndex*128.0 + 0.5));
}


vec3 getSamplerPointGridIndexVolSphere(vec3 pSampler, vec3 eyePosition, vec3 normal, float rfar, out unsigned int gridIndexBottonN, out unsigned int gridIndexTopN, out float secant)
{
  gridIndexBottonN = 0u;
  gridIndexTopN = 0u;
  secant = 0.;
  
  float d = length(pSampler);
  float zi = sqrt(rfar*rfar - d*d) ;
  float zo = -zi;
  secant = 2.0*zi;
  
  vec3 point = eyePosition + normal*rfar + pSampler;
  vec3 win = eye2window(point, bool(perspective));
  float zzNear = getZnear(win.xy);
  if(zzNear < 0.0)
    return vec3(-3.0);
     
  float zGridIndex = getZGridIndex(point.z, zzNear);
  //if(zGridIndex < 0.0)
    //return vec3(-1.0);
    
  vec3 samplerGridIndex = vec3(win.x/screenWidth, win.y/screenHeight, zGridIndex);
  

  
  float zInGridIndex  = getClampedZGridIndex(point.z + zi, zzNear); //Closer to The Eye
  float zOutGridIndex = getClampedZGridIndex(point.z + zo, zzNear);
  
  float gridIndexTop = zInGridIndex;
  float gridIndexBotton = zOutGridIndex;
  
  gridIndexBottonN = getGridIndexNormalized(gridIndexBotton);
  gridIndexTopN = getGridIndexNormalized(gridIndexTop);
  return samplerGridIndex;
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
    count += unsigned int (numBits*255.0);
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
    }
    else if(i == bottonContentColor)
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




