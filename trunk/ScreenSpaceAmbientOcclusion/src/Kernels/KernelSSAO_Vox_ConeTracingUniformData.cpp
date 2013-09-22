/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Oct 2012
 *
 *  Calculate the AmbientOcclusion of a Scene using the voxelization of it.
 *  Make Cone Tracing to calculate the occlusion. Each Cone Integral is achieved 
 *  by calculating n sphere integrals.
 *  Uses a uniform variable to set the information instead of textures
 */
#include "KernelSSAO_Vox_ConeTracingUniformData.h"
#include <cmath>
#include <limits>

#include "defines.h"
#include "MathUtils/Matrix4.h"
#include "MathUtils/Vector4.h"
#include "MathUtils/Bit.h"
#include "GLUtils/GLProjectionMatrix.h"
#include "GLUtils/GLTextureObject.h"
#include "GLUtils/GLError.h"
#include "MathUtils/UniformPoissonDiskSampler.h"
#include "Kernels/KernelSSAO_Vox_ConeTracing.h"
#include "Kernels/KernelVoxelization.h"

#define str(s) #s

#define STATIC_RAND 456

#define VAR_SPHERES
#define VAR_SAMPLERS
#define STATIC_SAMPLING 

static const int MAX_SPHERE_SAMPLERS = 20;
static const int SPHERE_SAMPLERS_DISTRIBUTIONS = 12;
static const int SPHERE_SAMPLER_DATA_SIZE = 250;
static const int SAMPLING_DATA_SIZE = 241;

//BITCOUNT TEXTURE//////////////////////////////
//BITCOUNT TEXTURE//////////////////////////////
//BITCOUNT TEXTURE//////////////////////////////
//#define TEXTURE_BIT_COUNT


//#define CONE_REVOLUTION_ANGLE 1.04719755   //60
//#define CONE_REVOLUTION_ANGLE 0.785398163    //45
//#define CONE_REVOLUTION_ANGLE 0.523598776  //30
#define CONE_REVOLUTION_ANGLE 0.261799388  //15

#define getSphereCenter(eyePosition, coneDir, rfar, multiplier)  \
  ((eyePosition) + (coneDir)*(rfar)*(multiplier))

#define getSphereRadius(rfar, multiplier) \
  ((multiplier)*(rfar))

////////KERNEL////////////////////////////////////////////////
////////KERNEL////////////////////////////////////////////////
////////KERNEL////////////////////////////////////////////////
////////KERNEL////////////////////////////////////////////////
KernelSSAO_Vox_ConeTracingUniformData::KernelSSAO_Vox_ConeTracingUniformData(char* path, int width, int height, 
                                                 GLuint texIdEyePos, GLuint texIdNormalDepth, 
                                                 GLuint texIdVoxelGrid, GLuint texIdGridInvFunction)
: KernelBase(NULL, NULL, width, height)
,m_width(width)
,m_height(height)
,m_coneDirSamplersWidth(0)
,m_bitCount16Height(0)
,m_bitCount16Width(0)
,m_rfarPercent(.1f)
,m_contrast(1.0f)
,m_numCones(6) //HINT. MIN 4
,m_numSpheresByCone(3)
,m_numSamplersDistributions(5)
,m_coneRevolutionAngle(CONE_REVOLUTION_ANGLE)
,m_numSphereSamplers(3)
,m_jitterEnabled(true)
,m_sphereInfo(new SphereInfo())
,m_texIdConeDirSamplers(0)
,m_texIdBitCount16(0)
,m_uniformData(NULL)
,m_uniformDataSize(0)
,m_uniformSphereSamplerData(NULL)
,m_uniformSphereSamplerDataSize(0)
{
  const GLubyte *shader_version_str = glGetString(GL_SHADING_LANGUAGE_VERSION);
  float shader_vertsion;
  sscanf((char*)shader_version_str, "%f", &shader_vertsion);

  m_shader = new GLShader();
  m_shader->addReplaceDefine(str(MAX_SPHERE_SAMPLERS), MAX_SPHERE_SAMPLERS);
  m_shader->addReplaceDefine(str(SPHERE_SAMPLERS_DISTRIBUTIONS), SPHERE_SAMPLERS_DISTRIBUTIONS);
  m_shader->addReplaceDefine(str(SPHERE_SAMPLER_DATA_SIZE), SPHERE_SAMPLER_DATA_SIZE);
  m_shader->addReplaceDefine(str(SAMPLING_DATA_SIZE), SAMPLING_DATA_SIZE);
  m_shader->addReplaceDefine(str(EYE_NEAREST_ENABLED), EYE_NEAREST_ENABLED);
  m_shader->addReplaceDefine(str(LIMITED_FAR_ENABLED), LIMITED_FAR_ENABLED);
  m_shader->addReplaceDefine(str(LIMITED_FAR_PERCENT), LIMITED_FAR_PERCENT);
  m_shader->addReplaceDefine(str(TEXTURE_BIT_COUNT_ENABLED), shader_vertsion < 4.0f);
  string v = string(path) + "ssao_vox_cone_tracing_uniform.vert";
  string f = string(path) + "ssao_vox_cone_tracing_uniform.frag";
  m_shader->setShaderFiles((char*)v.c_str(), (char*)f.c_str());

  m_fbo->attachToDepthBuffer(GL_FBOBufferType::RenderBufferObject);

#ifdef STATIC_RAND
  srand(STATIC_RAND);
#endif // STATIC_RAND
  generateConeSamplerTexture();
  generateUniformDataArray();

#ifdef TEXTURE_BIT_COUNT
  generateBitCount16Texture();
#endif // TEXTURE_BIT_COUNT

  GLTextureObject t;
  t.createTexture2D(width, height, GL_RGBA32UI_EXT, GL_RGBA_INTEGER_EXT, GL_UNSIGNED_INT);
  t.setFilters(GL_NEAREST, GL_NEAREST);
  t.setWraps(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
  //Output
  m_texIdSSAO = addOutput(KernelSSAO_Vox_ConeTracingUniformData::SSAO);
  m_texIdDebug = addOutput(KernelSSAO_Vox_ConeTracingUniformData::Debug, t.getId());
	//Input
	m_shader->setActive(true);
    addInputFloat("rfarPercent", m_rfarPercent);
    addInputFloat("contrast", m_contrast);
    addInputInt("jitterEnabled", int(m_jitterEnabled));

    addInputFloat("screenWidth", width);
    addInputFloat("screenHeight", height);

    addInputFloatArray("samplingData", m_uniformData, m_uniformDataSize);
    addInputFloat2Array("sphereSamplerData", m_uniformSphereSamplerData, m_uniformSphereSamplerDataSize);
#ifdef TEXTURE_BIT_COUNT
    addInputTexture(GL_TEXTURE_2D, "bitCount16", m_texIdBitCount16);
    addInputInt("bitCount16Width", m_bitCount16Width);
    addInputInt("bitCount16Height", m_bitCount16Height);
#endif // TEXTURE_BIT_COUNT

    addInputTexture(GL_TEXTURE_1D, "gridInvFunc", texIdGridInvFunction);
    addInputTexture(GL_TEXTURE_2D, "eyePos", texIdEyePos);
    addInputTexture(GL_TEXTURE_2D, "normalDepth", texIdNormalDepth);
    addInputTexture(GL_TEXTURE_2D, "voxelGrid", texIdVoxelGrid);
    //addInputTexture(GL_TEXTURE_2D, "voxelGridDEBUG", texIdVoxelGrid);
    
	m_shader->setActive(false);
  m_pointLight.setAmbientColor(Color(.2, .2, .2));
  m_pointLight.setDiffuseColor(Color(.5, .5, .5));
  m_pointLight.setSpecularColor(Color(1, 1, 1));
  m_pointLight.setPosition(Vector3(0,2,0));
  m_pointLight.setRenderSphereRadius(.2);
  m_pointLight.setRenderSphereEnabled(true);
}

KernelSSAO_Vox_ConeTracingUniformData::~KernelSSAO_Vox_ConeTracingUniformData()
{
  delete[] m_uniformData;
  delete[] m_uniformSphereSamplerData;
}


void KernelSSAO_Vox_ConeTracingUniformData::setActive( bool op, GLProjectionMatrix *projectionMatrix )
{
  if(op)
  {
    float znear = projectionMatrix->getNear();
    float zfar = projectionMatrix->getFar();
    float right = projectionMatrix->getRight();
    float top = projectionMatrix->getTop();
    int perspective = (int)!(projectionMatrix->isOrthographic());

    m_shader->setActive(true);
      addInputFloat("near", znear);
      addInputFloat("far", zfar);
      addInputFloat("right", right);
      addInputFloat("top", top);
      addInputInt("perspective", perspective);
    m_shader->setActive(false);
  }
  KernelBase::setActive(op);
}

void KernelSSAO_Vox_ConeTracingUniformData::setActiveShaderOnly( bool op, GLProjectionMatrix *projectionMatrix )
{
  if(op)
  {
    float znear = projectionMatrix->getNear();
    float zfar = projectionMatrix->getFar();
    float right = projectionMatrix->getRight();
    float top = projectionMatrix->getTop();
    int perspective = (int)!(projectionMatrix->isOrthographic());

    m_shader->setActive(true);
    addInputFloat("near", znear);
    addInputFloat("far", zfar);
    addInputFloat("right", right);
    addInputFloat("top", top);
    addInputInt("perspective", perspective);
    m_shader->setActive(false);
  }

  KernelBase::setActiveShaderOnly(op);
}


void KernelSSAO_Vox_ConeTracingUniformData::step( GLProjectionMatrix *projectionMatrix )
{
  float znear = projectionMatrix->getNear();
  float zfar = projectionMatrix->getFar();
  float right = projectionMatrix->getRight();
  float top = projectionMatrix->getTop();
  int perspective = (int)!(projectionMatrix->isOrthographic());
  //GLfloat invProjGL[16];
  //for(int i = 0; i < 4; ++i)
  //  for(int j = 0; j < 4; ++j)
  //    invProjGL[i*4+j] = projectionMatrix->getValue(i, j);

  //Matrix4 invProj = Matrix4(invProjGL);
  //invProj.Invert();
  //invProj.Transpose();
  //for(int i = 0; i < 4; ++i)
  //  for(int j = 0; j < 4; ++j)
  //    invProjGL[i*4+j] = invProj.getValue(i, j);
  m_fbo->setActive(true);
  if(m_shader)
  {
    m_shader->setActive(true);
    addInputFloat("near", znear);
    addInputFloat("far", zfar);
    addInputFloat("right", right);
    addInputFloat("top", top);
    addInputInt("perspective", perspective);
    //addInputMatrix4("projInv", invProjGL);
  }
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  renderScreenQuad();
  
  if(m_shader)
    m_shader->setActive(false);
  m_fbo->setActive(false);
}


GLuint KernelSSAO_Vox_ConeTracingUniformData::getTexIdSSAO() const
{
  return m_texIdSSAO;
}

int KernelSSAO_Vox_ConeTracingUniformData::getNumCones() const
{
  return m_numCones;
}

void KernelSSAO_Vox_ConeTracingUniformData::setNumCones( int val )
{
  m_numCones = val;
  reloadShaderInput();
}

int KernelSSAO_Vox_ConeTracingUniformData::getNumSpheresByCone() const
{
  return m_numSpheresByCone;
}

void KernelSSAO_Vox_ConeTracingUniformData::setNumSpheresByCone( int val )
{
  if(m_sphereInfo->currCalcMethod == SphereInfo::RadiusInitDistance)
    m_numSpheresByCone = min(val, m_sphereInfo->radDistParms.numMaxSpheres);
  else
    m_numSpheresByCone = val;
  reloadShaderInput();
}

SphereInfo* KernelSSAO_Vox_ConeTracingUniformData::getSphereInfo() 
{
  return m_sphereInfo;
}

bool KernelSSAO_Vox_ConeTracingUniformData::isJitterEnabled() const
{
  return m_jitterEnabled;
}

void KernelSSAO_Vox_ConeTracingUniformData::setJitterEnabled( bool val )
{
  m_jitterEnabled = val;
  m_shader->setActive(true);
  addInputInt("jitterEnabled", int(m_jitterEnabled));
  m_shader->setActive(false);

}

float KernelSSAO_Vox_ConeTracingUniformData::getContrast() const
{
  return m_contrast;
}

void KernelSSAO_Vox_ConeTracingUniformData::setContrast( float val )
{
  m_contrast = val;
  m_shader->setActive(true);
  addInputFloat("contrast", m_contrast);
  m_shader->setActive(false);
}

float KernelSSAO_Vox_ConeTracingUniformData::getRfarPercent() const
{
  return m_rfarPercent;
}

void KernelSSAO_Vox_ConeTracingUniformData::setRfarPercent( float val )
{
  m_rfarPercent = val;

  m_shader->setActive(true);
  addInputFloat("rfarPercent", m_rfarPercent);
  m_shader->setActive(false);

  //m_numSpheresByCone = CALC_NUM_SPHERES_BASED_ON_RFAR(m_rfarPercent);
  //reloadShaderInput();
}
float KernelSSAO_Vox_ConeTracingUniformData::getConeRevolutionAngle() const
{
  return m_coneRevolutionAngle;
}

void KernelSSAO_Vox_ConeTracingUniformData::setConeRevolutionAngle( float val )
{
  m_coneRevolutionAngle = val;
  reloadShaderInput();
}

int KernelSSAO_Vox_ConeTracingUniformData::getNumberSphereSamplers() const
{
  return m_numSphereSamplers;
}

void KernelSSAO_Vox_ConeTracingUniformData::setNumSphereSamplers( int val )
{
  m_numSphereSamplers = val;
  reloadShaderInput();
}
int KernelSSAO_Vox_ConeTracingUniformData::getNumSamplersDistributions() const
{
  return m_numSamplersDistributions;
}

void KernelSSAO_Vox_ConeTracingUniformData::setNumSamplersDistributions( int val )
{
  m_numSamplersDistributions = val;
  reloadShaderInput();
}

void KernelSSAO_Vox_ConeTracingUniformData::reloadShaderInput()
{
#ifdef STATIC_RAND
  srand(STATIC_RAND);
#endif // STATIC_RAND
  generateConeSamplerTexture();
  generateUniformDataArray();

  //Input
  m_shader->setActive(true);
  addInputFloat("rfarPercent", m_rfarPercent);

  addInputFloatArray("samplingData", m_uniformData, m_uniformDataSize);
  addInputFloat2Array("sphereSamplerData", m_uniformSphereSamplerData, m_uniformSphereSamplerDataSize);

  m_shader->setActive(false);
}

void KernelSSAO_Vox_ConeTracingUniformData::generateBitCount16Texture()
{
  int numOfValues = (int)pow(2.0f, 16);

  GLint max_tex_size;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);

  m_bitCount16Width =  numOfValues > max_tex_size ? max_tex_size : numOfValues;
  m_bitCount16Height = numOfValues/max_tex_size + 1;

  GLubyte* texData = new GLubyte[m_bitCount16Width*m_bitCount16Height];


  for(int i = 0; i < numOfValues; ++i)
    texData[i] = (GLubyte)(unsigned char)Bit::numberOfSetBits(i);

  GLTextureObject t;
  if(!m_texIdBitCount16)
    m_texIdBitCount16 = t.createTexture2D(m_bitCount16Width, m_bitCount16Height, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE, texData);
  else 
    t.setTexture2D(m_texIdBitCount16, m_bitCount16Width, m_bitCount16Height, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE, texData);

  t.setFilters(GL_NEAREST, GL_NEAREST);
  t.setWraps(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

  delete[] texData;
}

void KernelSSAO_Vox_ConeTracingUniformData::generateConeSamplerTexture()
{
  GLfloat* texData;
  if(m_numCones == 9)
  {
    m_coneDirSamplersWidth = m_numSamplersDistributions * 9;
    texData = new GLfloat[m_coneDirSamplersWidth*4];
    
    Vector3 dirs[9];
    dirs[0] = Vector3(0,1,0);
    for(int i = 0; i < 4; ++i)
    {
      float angleBeta = DEG_TO_RAD(50.0f);
      float angleAlpha = DEG_TO_RAD(i*360.0f/4.0f);
      dirs[i+1] = Vector3(sin(angleBeta)*sin(angleAlpha), cos(angleBeta), sin(angleBeta)*cos(angleAlpha));
    }

    for(int i = 0; i < 4; ++i)
    {
      float angleBeta = DEG_TO_RAD(75.0f);
      float angleAlpha = DEG_TO_RAD(45.0f + i*360.0f/4.0f);
      dirs[i+5] = Vector3(sin(angleBeta)*sin(angleAlpha), cos(angleBeta), sin(angleBeta)*cos(angleAlpha));
    }
    for(int i = 0; i < 9; ++i)
    {
      texData[i*4 + 0] = dirs[i].x;
      texData[i*4 + 1] = dirs[i].y;
      texData[i*4 + 2] = dirs[i].z;
      texData[i*4 + 3] = (float)m_numSpheresByCone;
    }
  }else {
    int numSamplersBeta = ceil(sqrt(float(m_numCones)));            //Longitude  (|||)
    int numSamplersAlpha = ceil(float(m_numCones)/numSamplersBeta); //Latitude   (===)

    m_numCones = numSamplersAlpha*numSamplersBeta;

    m_coneDirSamplersWidth = m_numSamplersDistributions * numSamplersAlpha * numSamplersBeta;
    texData = new GLfloat[m_coneDirSamplersWidth*4];

    for(int k = 0; k < m_numSamplersDistributions; ++k)
    {
      float soma = 0;
      GLfloat *d = &texData[k*numSamplersBeta*numSamplersAlpha*4];
      for(int i = numSamplersAlpha-1; i >= 0; i--)
      {
        float decPar = numSamplersAlpha%2==0 && (numSamplersAlpha/2) - i <=0;
        float step = 1.0f/(numSamplersAlpha);
        float value = 1 + ((numSamplersAlpha/2) - i - decPar)*step;
        //printf("%d %d %f %f\n", i,m_numSamplersAlpha,  value, (i > 0)? floor(m_numSamplersBeta*value): m_numSamplersBeta*m_numSamplersAlpha - soma );
        int betaSize = int((i > 0)? floor(numSamplersBeta*value): numSamplersBeta*numSamplersAlpha - soma );
        soma += float(betaSize);
        for(int j = 0; j < betaSize; ++j)
        {
          float r = 1.0f;

          if(k > 0)
            r = (1.0f + float(rand()%10)/100 - .05f);

          float coneAngle = atan(sqrt(2.0f/(m_numCones)));
          float angleBeta = (((PI/2 - coneAngle)*(numSamplersAlpha-i-1)/(numSamplersAlpha-1)))*r; //Latitude

          if(k > 0)
            r = (1.0f + float(rand()%10)/100 - .05f);

          float  angleAlpha = ((j)*(2*PI)/(betaSize) + ((i%2))*(2*PI)/(2*betaSize))*r ;
          if(i==1)
            angleAlpha+=1.05;

          Vector3 dir = Vector3(sin(angleBeta)*sin(angleAlpha), cos(angleBeta), sin(angleBeta)*cos(angleAlpha)); //Longitude
          d[int(soma-betaSize)*4 + j*4 + 0] = dir.x;
          d[int(soma-betaSize)*4 + j*4 + 1] = dir.y;
          d[int(soma-betaSize)*4 + j*4 + 2] = dir.z;
          d[int(soma-betaSize)*4 + j*4 + 3] = (float)m_numSpheresByCone;
        }
      }
      //printf("%f %d %f!!!!!!!!!!!\n\n", soma, m_numSamplersBeta*m_numSamplersAlpha, soma/(m_numSamplersBeta*m_numSamplersAlpha));
    }
  }
  GLTextureObject t;
  if(!m_texIdConeDirSamplers)
    m_texIdConeDirSamplers = t.createTexture1D(m_coneDirSamplersWidth, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT, texData);
  else 
    t.setTexture1D(m_texIdConeDirSamplers, m_coneDirSamplersWidth, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT, texData);

  t.setFilters(GL_NEAREST, GL_NEAREST);
  t.setWraps(GL_CLAMP_TO_EDGE);

  delete [] texData;

}



/**
 *	Auxiliary Functions
 */
static inline Vector3 getConeDir(int i, GLfloat* coneData)
{
  Vector3 dir;
  dir.x = coneData[i*4 + 0];
  dir.y = coneData[i*4 + 1];
  dir.z = coneData[i*4 + 2];
  return dir;
}

static inline int getNumberOfSpheres(int numMin, int numMax, Vector3 coneDir, int coneIndex, int numCones)
{
#ifdef VAR_SPHERES
  #ifdef STATIC_SAMPLING
    if(numCones == 9 && numMax == 7) // HIGH
    {
      int numSphereHighStatic[9] = {7,6,6,6,6,5,5,5,5}; 
      return numSphereHighStatic[coneIndex];
    }
    if(numCones == 9 && numMax == 5) // Medium
    {
      int numSphereMediumStatic[9] = {5,4,4,4,4,2,2,2,2};
      return numSphereMediumStatic[coneIndex];
    }
    if(numCones == 6 && numMax == 4) //Low
    {
      int numSphereLowStatic[6] = {4,3,3,3,3,3}; 
      return numSphereLowStatic[coneIndex];
    }

    int range = numMax - numMin;
    float dot = coneDir*Vector3(0,1,0);
    return numMin + (int) floorf(dot*range + 0.5f);
  #else
    int range = numMax - numMin;
    float dot = coneDir*Vector3(0,1,0);
    return numMin + (int) floorf(dot*range + 0.5f);
  #endif // STATIC_SAMPLING
#else
  return numMax;
#endif // VAR_SPHERES
}

static inline int getNumberOfSamplers(int numMin, int numMax, float dist, int coneIndex, int sphereIndex, int numCones) 
{ 
#ifdef VAR_SAMPLERS
  #ifdef STATIC_SAMPLING
  if(numCones == 9 && numMax == 12) // HIGH
  {
    int numSamplerHighStatic[9][7] = {
      {7,7,7,7,7,7,7},
      {4,4,4,4,4,4,4},
      {4,4,4,4,4,4,4},
      {4,4,4,4,4,4,4},
      {4,4,4,4,4,4,4},
      {2,2,2,2,2,2,2},
      {2,2,2,2,2,2,2},
      {2,2,2,2,2,2,2},
      {2,2,2,2,2,2,2},
    };
    return numSamplerHighStatic[coneIndex][sphereIndex];
  }
  if(numCones == 9 && numMax == 6) // Medium
  {
    int numSamplerMedimumStatic[9][7] = {
      {5,5,5,5,5,5,5},
      {4,4,4,4,4,4,4},
      {4,4,4,4,4,4,4},
      {4,4,4,4,4,4,4},
      {4,4,4,4,4,4,4},
      {2,2,2,2,2,2,2},
      {2,2,2,2,2,2,2},
      {2,2,2,2,2,2,2},
      {2,2,2,2,2,2,2},
    };
    return numSamplerMedimumStatic[coneIndex][sphereIndex];
  }
  if(numCones == 6 && numMax == 4) //Low
  {
    int numSamplerLowStatic[6][5] = {
      {3,3,3,3,3},
      {2,2,2,2,2},
      {2,2,2,2,2},
      {2,2,2,2,2},
      {2,2,2,2,2},
      {2,2,2,2,2},
    };
    return numSamplerLowStatic[coneIndex][sphereIndex];
  }
  int range = numMax - numMin;
  dist = clamp(dist, 0.0f, 1.0f);
  dist = (1.0f - dist); //Closer Spheres should have more samplers
  return numMin + (int) floorf(dist*range + 0.5f);
  #else
    int range = numMax - numMin;
    dist = clamp(dist, 0.0f, 1.0f);
    dist = (1.0f - dist); //Closer Spheres should have more samplers
    return numMin + (int) floorf(dist*range + 0.5f);
  #endif // STATIC_SAMPLING
#else
  return numMax;
#endif // VAR_SAMPLERS
}

static inline void getSphereInformation(SphereInfo* info, float coneRevolutionAngle, int numCones, int numSpheresByCone, int numSamplersBySphere, int i, float *out_center, float *out_radius)
{
  info->setParameters(coneRevolutionAngle, numCones, numSpheresByCone, numSamplersBySphere);
  info->getSphereInfo(i, out_center, out_radius);
}

static inline vector<Vector3> getSamplersOnPoissonDisc(UniformPoissonDiskSampler &u, int numSamplers)
{
  vector<Vector3> v;
  static const int MaxRuns = 10;
  // MaxRuns limits the maximum number of tries of the sample algorithm
  for(int m = 0; m < MaxRuns && v.size() < numSamplers; ++m)
    v = u.sampleCircle(Vector3(0,0,0), 1.0f, (1+m*0.1f)*sqrt(1.0f/numSamplers)*1.35f);
  while(v.size() > numSamplers)
  {
    int k = rand()%v.size();
    v.erase(v.begin()+k);
  }
  assert(v.size() == numSamplers);
  return v;
}

void KernelSSAO_Vox_ConeTracingUniformData::generateUniformDataArray ()
{
  int numCones = m_numCones;
  int numSpheres_min = (int) floorf(m_numSpheresByCone*0.2f + 0.5f);
  int numSpheres_max = m_numSpheresByCone;

  int numSamplers_min = (int) floorf(m_numSphereSamplers*.2f + 0.5f);
  int numSamplers_max = m_numSphereSamplers;

  //////////////////////////////
  // HEMISHPHERE SAMPLERS
  //////////////////////////////
  // Texture Object generated by the function generateConeSamplerTexture()
  GLTextureObject coneTexObj(m_texIdConeDirSamplers);
  GLfloat* coneDirData = coneTexObj.read1DTextureFloatData(GL_RGBA);

  int size = 0;

  // More than the real space needed because considers the max values
  int memorySize = 1 + numCones*(3+1+numSpheres_max*(1+1+1));

  delete[] m_uniformData;
  m_uniformData = new GLfloat[memorySize];

  m_uniformData[size++] = numCones;
  printf("\nNumCones: %d\n", numCones);
  for(int i = 0; i < numCones; ++i)
  {
    Vector3 coneDir = getConeDir(i, coneDirData);
    int numSpheres = getNumberOfSpheres(numSpheres_min, numSpheres_max, coneDir, i, numCones);
    m_uniformData[size++] = coneDir.x;
    m_uniformData[size++] = coneDir.y;
    m_uniformData[size++] = coneDir.z;
    m_uniformData[size++] = (GLfloat)numSpheres;
    printf("  NumSpheres: %d\n", numSpheres);
    for(int j = 0; j < numSpheres; ++j)
    {
      float r, d;
      getSphereInformation(m_sphereInfo, m_coneRevolutionAngle, m_numCones, numSpheres, m_numSphereSamplers, j, &d, &r);
      
      int numSamplers = getNumberOfSamplers(numSamplers_min, numSamplers_max, d, i, j, numCones);
      m_uniformData[size++] = r;
      m_uniformData[size++] = d;
      m_uniformData[size++] = (GLfloat)numSamplers;
      printf("    NumSamplers: %d\n", numSamplers);
    }
  }
  m_uniformDataSize = size;
  MyAssert("Max Uniform Sampling Array Size Reached", m_uniformDataSize <= SAMPLING_DATA_SIZE);

  //////////////////////////////
  // SPHERE SAMPLERS
  //////////////////////////////
  // Poisson Disk Sampler.
  UniformPoissonDiskSampler u;

  delete[] m_uniformSphereSamplerData;
  m_uniformSphereSamplerData = new GLfloat[SPHERE_SAMPLERS_DISTRIBUTIONS*MAX_SPHERE_SAMPLERS*2];

  for(int i = 0; i < SPHERE_SAMPLERS_DISTRIBUTIONS; ++i)
  {
    vector<Vector3> samplers = getSamplersOnPoissonDisc(u, MAX_SPHERE_SAMPLERS);
    for(int j = 0; j < MAX_SPHERE_SAMPLERS; ++j)
    {
      m_uniformSphereSamplerData[i*MAX_SPHERE_SAMPLERS*2 + j*2 + 0] = samplers[j].x;
      m_uniformSphereSamplerData[i*MAX_SPHERE_SAMPLERS*2 + j*2 + 1] = samplers[j].y;
    }
  }
  m_uniformSphereSamplerDataSize = SPHERE_SAMPLERS_DISTRIBUTIONS*MAX_SPHERE_SAMPLERS;
  MyAssert("Max Uniform Sphere Sampling Array Size Reached", m_uniformSphereSamplerDataSize <= SPHERE_SAMPLER_DATA_SIZE);
}


void KernelSSAO_Vox_ConeTracingUniformData::renderConeDistribution(int distribution)
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glPushMatrix();

  glPushMatrix();
  glScalef(2.0f,1.0f,2.0f);
  glTranslatef(0.0f, .5f, 0.0f);
  glutWireCube(1);
  glCullFace(GL_FRONT);
  glColor3f(.60f,0.2f,0.31f);
  glEnable(GL_CULL_FACE);
  glutSolidCube(1);
  glDisable(GL_CULL_FACE);
  glPopMatrix();

  m_pointLight.setPosition(Vector3(0,2,0));
  m_pointLight.configure();
  static float f = 0;
  glPushMatrix();
  //glRotated(f+=3, 0,1,0);
  glTranslated(2,0,0);
  m_pointLight.render();
  glPopMatrix();


  glEnable(GL_NORMALIZE);

  //int numSpheres_min = (int) floorf(m_numSpheresByCone*0.2f + 0.5f);
  //int numSpheres_max = m_numSpheresByCone;

  //int numSamplers_min = (int) floorf(m_numSphereSamplers*.2f + 0.5f);
  //int numSamplers_max = m_numSphereSamplers;

  int pos = 0;
  int numCones = m_uniformData[pos++];
  for(int i = 0; i < numCones; ++i)
  {
    Vector3 coneDir(m_uniformData[pos++], m_uniformData[pos++], m_uniformData[pos++]);
    int numSpheres = (int) m_uniformData[pos++];

    glPushMatrix();

    coneDir = coneDir.unitary();
    
    float angle = coneDir.angle(Vector3(0,1,0));
    Vector3 axis = coneDir^Vector3(0,1,0);
    if(!(axis == Vector3(0,0,0)))
    {
      axis = axis.unitary();
      glRotated(-angle, axis.x, axis.y, axis.z);
    }

    //SPHERES
    //SPHERES
    //SPHERES
    for(int j = 0; j < numSpheres; ++j)
    {
      float r = m_uniformData[pos++];
      float d = m_uniformData[pos++];
      int numSamplers = m_uniformData[pos++];

      Vector3 center = getSphereCenter(Vector3(0,0,0), Vector3(0,1,0), 1, d);
      float radius = getSphereRadius(1, r);
      GLfloat color3[4] = {0,1,1,1};
      color3[0] = ((float)((j*453+89)%255))/255;
      color3[1] = ((float)((j*876+8)%255))/255;
      color3[2] = ((float)((j*537+254)%255))/255;

      glPushMatrix();
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  color3);
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  color3);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color3);

      glTranslatef(center.x, center.y, center.z);
      glutSolidSphere(radius/2, 30, 30);
      glPopMatrix();
    }

    //CONES
    //CONES
    //CONES
    glRotated(90,1,0,0);
    glTranslatef(0,0,-1);
    float coneHeight = 1.0f;
    float coneAngle = m_coneRevolutionAngle;
    //float coneAngle = atan(sqrt(2.0f/(numCones)));
    float coneRadius = coneHeight*tan(coneAngle);

    GLfloat color[4] = {1,1,0,1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);

    //glutWireCone(coneRadius/2, coneHeight, 50, 50);

    glPopMatrix();
  }

  glPopMatrix();
  glPopAttrib();
}

/*
void KernelSSAO_Vox_ConeTracingUniformData::renderSphereSamplerDistribution(int distribution, int sphereIndex)
{
  int numSphereSamplers = getNumSphereSamplers(sphereIndex);
  //int numSphereSamplers = m_numSphereSamplers;
  int aritProgSum = getNumSphereSamplersAccumulated(sphereIndex);

  int totalAritProgSum = getTotalNumSphereSamplersAccumulated(m_numSpheresByCone);

  glPushMatrix();
  glPushAttrib(GL_ALL_ATTRIB_BITS);
#ifdef TEXTURE_SPHERE_SAMPLERS
  GLTextureObject t(m_texIdSphereSamplers, GL_TEXTURE_1D);
  GLfloat *texData =  &t.read1DTextureFloatData(GL_RGB)[distribution*totalAritProgSum*3 + aritProgSum*3];
#else
  GLfloat *texData =  &m_sphereSamplers[distribution*totalAritProgSum*3 + aritProgSum*3];
#endif
  glPushMatrix();
  glScalef(2.0f,2.0f, 0.000000001f);
  //glTranslatef(0.0f, .5f, 0.0f);
  glutWireCube(1);
  glPopMatrix();
  glPointSize(3);
  glColor3f(1.0f, 1.0f, 1.0f);

  glBegin(GL_POINTS);
  for(int i = 0; i < numSphereSamplers; ++i)
  {
    Vector3 pos(
      texData[i*3 + 0],
      texData[i*3 + 1],
      texData[i*3 + 2]);
    glVertex3f(pos.x, pos.y, pos.z);
  }
  glEnd();
  glPopAttrib();
  glPopMatrix();
}

void KernelSSAO_Vox_ConeTracingUniformData::renderSphereInfoDistribution(int distribution)
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glPushMatrix();

  GLTextureObject t(m_texIdSphereInfo, GL_TEXTURE_1D);
  GLfloat *texDataInfo =  &t.read1DTextureFloatData(GL_LUMINANCE_ALPHA)[distribution*m_numSpheresByCone*2];

  //SPHERES
  //SPHERES
  for(int k = 0; k < m_numSpheresByCone; ++k)
  {
    float dist = texDataInfo[k*2 + 0];
    float radius = texDataInfo[k*2 + 1];
    glPushMatrix();
    glTranslatef(0.0f, dist, 0.0f);
    glBegin(GL_LINE_STRIP);
    glVertex3f(radius*sin(0.0f), radius*cos(0.0f), 0.0f);
    for(int i = 0; i < 36; ++i)
      glVertex3f(radius*sin(DEG_TO_RAD(i*10.0f)), radius*cos(DEG_TO_RAD(i*10.0f)), 0.0f);
    glVertex3f(radius*sin(0.0f), radius*cos(0.0f), 0.0f);
    glEnd();
    glPopMatrix();
  }

  //CONE
  //CONE
  glDisable(GL_DEPTH_TEST);

  glBegin(GL_LINE_STRIP);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(-tan(m_coneRevolutionAngle), 1.0f, 0.0f);
  glVertex3f(tan(m_coneRevolutionAngle), 1.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 1.0f, 0.0f);
  glEnd();

  glPopMatrix();
  glPopAttrib();
}
*/

