/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Oct 2011
 *
 *  Calculate the AmbientOcclusion of a Scene using the voxelization of it.
 *  Make Cone Tracing to calculate the occlusion. Each Cone Integral is achieved 
 *  by calculating n sphere integrals.
 */
#include "KernelSSAO_Vox_ConeTracing.h"
#include <cmath>
#include <limits>

#include "MathUtils/Matrix4.h"
#include "MathUtils/Vector4.h"
#include "MathUtils/Bit.h"
#include "GLUtils/GLProjectionMatrix.h"
#include "MathUtils/UniformPoissonDiskSampler.h"


#define CALC_NUM_SPHERES_BASED_ON_RFAR(rfar) \
  (floor(pow(rfar, .5f)*SPHERE_MAX_NUM + .5f))

//SPHERE SAMPLERS//////////////////////////////
//SPHERE SAMPLERS//////////////////////////////
//SPHERE SAMPLERS//////////////////////////////
#define TEXTURE_SPHERE_SAMPLERS

//Functions:
#define ARITPROG_AN(a, d, n) int((a) + ((n-1.0)*(d)))
#define ARITPROG_SUM(a, d, n) int(((n)*((a)+ARITPROG_AN(a,d,n))/2.0))

#define getNumSphereSamplers(n) \
  m_numSphereSamplers
  //ARITPROG_AN(PROG_A0, PROG_STEP, (n)+1)
  //(3)

#define getNumSphereSamplersAccumulated(n) \
  (0)
  //ARITPROG_SUM(PROG_A0, PROG_STEP, (n))
  //(m_numSphereSamplers*(n))

#define getTotalNumSphereSamplersAccumulated(numSpheresByCone) \
(getNumSphereSamplers(1))
  //getNumSphereSamplersAccumulated(numSpheresByCone)
//(m_numSphereSamplers*numSpheresByCone)


//SPHERE INFO//////////////////////////////////////////
//SPHERE INFO//////////////////////////////////////////
//SPHERE INFO//////////////////////////////////////////
//#define CONE_REVOLUTION_ANGLE 1.04719755   //60
//#define CONE_REVOLUTION_ANGLE 0.785398163    //45
//#define CONE_REVOLUTION_ANGLE 0.523598776  //30
#define CONE_REVOLUTION_ANGLE 0.261799388  //15

#define getSphereCenter(eyePosition, coneDir, rfar, multiplier)  \
  ((eyePosition) + (coneDir)*(rfar)*(multiplier))

#define getSphereRadius(rfar, multiplier) \
  ((multiplier)*(rfar))

void SphereInfo::setParameters( float coneRevolutionAngle, int numSpheresByCone, int numCones )
{
  m_coneRevolutionAngle = coneRevolutionAngle;
  m_numSpheresByCone = numSpheresByCone;
  m_numCones = numCones;

  switch(currCalcMethod)
  {
  case RadiusProgression:
    break;
  case  RadiusInitDistance:
    radDistParms.xo = RadDist_GetConeInitialDist(radDistParms.numMaxSpheres, m_coneRevolutionAngle);
    break;
  }
}

void SphereInfo::nextCalcMethod()
{
  currCalcMethod = CalcMethods((currCalcMethod + 1)%EndEnum);
}

void SphereInfo::getSphereInfo( int k, float* center, float* radius )
{
  switch(currCalcMethod)
  {
  case RadiusProgression:
    *center = RadProg_GetSphereCenterMultiplier(k, m_numSpheresByCone);
    *radius = RadProg_GetSphereRadiusMultiplier(k, m_numSpheresByCone, m_numCones, m_coneRevolutionAngle);
    break;
  case  RadiusInitDistance:
    int index = floor(  ((m_numSpheresByCone>1)? (radDistParms.numMaxSpheres - 1) *  pow(float(k+1)/(m_numSpheresByCone ), float(m_numSpheresByCone)/radDistParms.numMaxSpheres) : radDistParms.numMaxSpheres - 2 )   );
    float initDist = RadDist_GetInitDist(radDistParms.xo, index, m_coneRevolutionAngle);
    float rad = RadDist_GetRadius(initDist, m_coneRevolutionAngle);
    *center = (initDist + rad);
    *radius = rad;
    break;
  }
}

float SphereInfo::RadDist_GetRadius( float initDist, float coneRevolutionAngle )
{
  float t_2 = tan(coneRevolutionAngle)*tan(coneRevolutionAngle);
  return (initDist + initDist*sqrt(1.0f + 1.0f/t_2))*t_2;
}

float SphereInfo::RadDist_GetInitDist( float xo, int i, float coneRevolutionAngle )
{
  float t_2 = tan(coneRevolutionAngle)*tan(coneRevolutionAngle);
  float delta = sqrt(1.0f+(1.0f/t_2));
  return xo * pow(((1.0f - radDistParms.sphereOverlap) * 2.0f * t_2 * (1.0f + delta) + 1.0f) , i);
}

float SphereInfo::RadDist_GetConeInitialDist(int numSpheresByCone, float coneRevolutionAngle )
{
  //Derivated from the Xn recorrence(x = init distance) Xn = 2*RX[n-1] + X[n-1]
  //Where R(x) is the radius function: R(x) = (x + x*sqrt(1+1/tan(ang)^2))*tan(ang)^2
  //X[n] = X[0]*(2*(tan(ang)^2)*(1+sqrt(1+1/tan(ang)^2)) + 1)^n
  //So, if the height of the cone is 1 (X[n] = 1) X[0] is the return of this function
  //The Overlap value is an approximation and 2.42 is a magic number that makes the result almost correct for 15 degrees
  float t_2 = tan(coneRevolutionAngle)*tan(coneRevolutionAngle);
  float delta = sqrt(1.0f+(1.0f/t_2));
  float div = 2.0f*(1.0f-radDistParms.sphereOverlap)*t_2*(1.0f+delta) + 1.0f;
  float div_pow = pow(div, numSpheresByCone);
  return (1.0f - radDistParms.sphereOverlap/2.42f)/div_pow;
}

float SphereInfo::RadProg_GetSphereRadiusMultiplier( int i, int numSpheresByCone, int numCones, float coneRevolutionAngle )
{
  return pow(((float(i) + 2.0f)/(float(numSpheresByCone) + 1.0f)), radProgParms.sphereRadiusParm)*tan(coneRevolutionAngle)*.8f;//1.5
  //return (pow(((float(i) + 1.0f)/(float(numSpheresByCone) + 1.0f)), radProgParms.sphereRadiusParm)*sqrt(2.0/float(numCones)))*.8f;//1.5
  //(pow(((float(i) + 2.0)/(float(numSpheresByCone) + 1.0)), SPHERERADIUS_PARM)*sqrt(2.0/float(numCones)))//1.5
}

float SphereInfo::RadProg_GetSphereCenterMultiplier( int i, int numSpheresByCone )
{
  return pow(((float(i)+2.0f)/(float(numSpheresByCone) + 1.0f)), radProgParms.sphereCenterParm)*.8f;//3
}




////////KERNEL////////////////////////////////////////////////
////////KERNEL////////////////////////////////////////////////
////////KERNEL////////////////////////////////////////////////
////////KERNEL////////////////////////////////////////////////
KernelSSAO_Vox_ConeTracing::KernelSSAO_Vox_ConeTracing(char* path, int width, int height, 
                                                 GLuint texIdEyePos, GLuint texIdNormalDepth, 
                                                 GLuint texIdVoxelGrid, GLuint texIdGridInvFunction)
: KernelBase(path, "ssao_vox_cone_tracing.vert", "ssao_vox_cone_tracing.frag", width, height)
,m_width(width)
,m_height(height)
,m_coneDirSamplersWidth(0)
,m_bitCount16Height(0)
,m_bitCount16Width(0)
,m_sphereSamplersWidth(0)
,m_sphereInfoWidth(0)
,m_rfarPercent(.1f)
,m_contrast(1.0f)
,m_numCones(6) //HINT. MIN 4
,m_numSpheresByCone(0)
,m_numSamplersDistributions(5)
,m_coneRevolutionAngle(CONE_REVOLUTION_ANGLE)
,m_numSphereSamplers(3)
,m_jitterEnabled(true)
,m_sphereInfo(new SphereInfo())
,m_sphereSamplers(NULL)
,m_texIdConeDirSamplers(0)
,m_texIdSphereSamplers(0)
,m_texIdSphereInfo(0)
,m_texIdBitCount16(0)
{
  m_numSpheresByCone = CALC_NUM_SPHERES_BASED_ON_RFAR(m_rfarPercent);
  
  m_fbo->attachToDepthBuffer(GL_FBOBufferType::RenderBufferObject);

  generateConeSamplerTexture();
  generateSphereSamplerTexture();
  generateSphereInfoTexture();
  generateBitCount16Texture();

  GLTextureObject t;
  t.createTexture2D(width, height, GL_RGBA32UI_EXT, GL_RGBA_INTEGER_EXT, GL_UNSIGNED_INT);
  t.setFilters(GL_NEAREST, GL_NEAREST);
  t.setWraps(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
  //Output
  m_texIdSSAO = addOutput(KernelSSAO_Vox_ConeTracing::SSAO);
  m_texIdDebug = addOutput(KernelSSAO_Vox_ConeTracing::Debug, t.getId());
  
	//Input
	m_shader->setActive(true);
    addInputFloat("rfarPercent", m_rfarPercent);
    addInputFloat("contrast", m_contrast);

    addInputFloat("screenWidth", width);
    addInputFloat("screenHeight", height);
    addInputInt("numCones", m_numCones);
    addInputInt("numSpheresByCone", m_numSpheresByCone);
    addInputInt("numSphereSamplers", m_numSphereSamplers);
    addInputInt("jitterEnabled", int(m_jitterEnabled));
    addInputInt("numSamplersDistributions", m_numSamplersDistributions);
    addInputInt("coneDirSamplersWidth", m_coneDirSamplersWidth);
    addInputInt("bitCount16Width", m_bitCount16Width);
    addInputInt("bitCount16Height", m_bitCount16Height);
    
    addInputInt("sphereInfoWidth", m_sphereInfoWidth);
    addInputTexture(GL_TEXTURE_1D, "sphereInfo", m_texIdSphereInfo);

#ifdef TEXTURE_SPHERE_SAMPLERS
    addInputInt("sphereSamplersWidth", m_sphereSamplersWidth);
    addInputTexture(GL_TEXTURE_1D, "sphereSamplers", m_texIdSphereSamplers);
#else
    //addInputVec3Array("sphereSamplersArray", m_sphereSamplers, m_sphereSamplersWidth);
    addInputVec3Array("sphereSamplersArray", m_sphereSamplers, getTotalNumSphereSamplersAccumulated(m_numSpheresByCone)*3);
    //addInputVec3Array("sphereSamplersArray", m_sphereSamplers, m_numSphereSamplers*3);
#endif // TEXTURE_SPHERE_SAMPLERS

    addInputTexture(GL_TEXTURE_1D, "coneDirSamplers", m_texIdConeDirSamplers);
    addInputTexture(GL_TEXTURE_1D, "gridInvFunc", texIdGridInvFunction);
    addInputTexture(GL_TEXTURE_2D, "bitCount16", m_texIdBitCount16);
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

KernelSSAO_Vox_ConeTracing::~KernelSSAO_Vox_ConeTracing(){
  if(m_sphereSamplers)
    delete[] m_sphereSamplers;
}


void KernelSSAO_Vox_ConeTracing::setActive( bool op, GLProjectionMatrix *projectionMatrix )
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

void KernelSSAO_Vox_ConeTracing::setActiveShaderOnly( bool op, GLProjectionMatrix *projectionMatrix )
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


void KernelSSAO_Vox_ConeTracing::step( GLProjectionMatrix *projectionMatrix )
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


GLuint KernelSSAO_Vox_ConeTracing::getTexIdSSAO() const
{
  return m_texIdSSAO;
}

int KernelSSAO_Vox_ConeTracing::getNumCones() const
{
  return m_numCones;
}

void KernelSSAO_Vox_ConeTracing::setNumCones( int val )
{
  m_numCones = val;
  reloadShaderInput();
}

int KernelSSAO_Vox_ConeTracing::getNumSpheresByCone() const
{
  return m_numSpheresByCone;
}

//void KernelSSAO_Vox_ConeTracing::setNumSpheresByCone( int val )
//{
//  if(m_sphereInfo->currCalcMethod == SphereInfo::RadiusInitDistance)
//    m_numSpheresByCone = min(val, m_sphereInfo->radDistParms.numMaxSpheres);
//  else
//    m_numSpheresByCone = val;
//  reloadShaderInput();
//}

SphereInfo* KernelSSAO_Vox_ConeTracing::getSphereInfo() 
{
  return m_sphereInfo;
}

bool KernelSSAO_Vox_ConeTracing::isJitterEnabled() const
{
  return m_jitterEnabled;
}

void KernelSSAO_Vox_ConeTracing::setJitterEnabled( bool val )
{
  m_jitterEnabled = val;
  m_shader->setActive(true);
  addInputInt("jitterEnabled", int(m_jitterEnabled));
  m_shader->setActive(false);

}

float KernelSSAO_Vox_ConeTracing::getContrast() const
{
  return m_contrast;
}

void KernelSSAO_Vox_ConeTracing::setContrast( float val )
{
  m_contrast = val;
  m_shader->setActive(true);
  addInputFloat("contrast", m_contrast);
  m_shader->setActive(false);
}

float KernelSSAO_Vox_ConeTracing::getRfarPercent() const
{
  return m_rfarPercent;
}

void KernelSSAO_Vox_ConeTracing::setRfarPercent( float val )
{
  m_rfarPercent = val;
  m_numSpheresByCone = CALC_NUM_SPHERES_BASED_ON_RFAR(m_rfarPercent);
  reloadShaderInput();
}
float KernelSSAO_Vox_ConeTracing::getConeRevolutionAngle() const
{
  return m_coneRevolutionAngle;
}

void KernelSSAO_Vox_ConeTracing::setConeRevolutionAngle( float val )
{
  m_coneRevolutionAngle = val;
  reloadShaderInput();
}

int KernelSSAO_Vox_ConeTracing::getNumberSphereSamplers() const
{
  return m_numSphereSamplers;
}

void KernelSSAO_Vox_ConeTracing::setNumSphereSamplers( int val )
{
  m_numSphereSamplers = val;
  reloadShaderInput();
}
int KernelSSAO_Vox_ConeTracing::getNumSamplersDistributions() const
{
  return m_numSamplersDistributions;
}

void KernelSSAO_Vox_ConeTracing::setNumSamplersDistributions( int val )
{
  m_numSamplersDistributions = val;
  reloadShaderInput();
}

void KernelSSAO_Vox_ConeTracing::reloadShaderInput()
{
  generateConeSamplerTexture();
  generateSphereSamplerTexture();
  generateSphereInfoTexture();

  //Input
  m_shader->setActive(true);
  addInputFloat("rfarPercent", m_rfarPercent);

  addInputInt("numCones", m_numCones);
  addInputInt("numSpheresByCone", m_numSpheresByCone);
  addInputInt("numSphereSamplers", m_numSphereSamplers);
  addInputInt("numSamplersDistributions", m_numSamplersDistributions);
  addInputInt("coneDirSamplersWidth", m_coneDirSamplersWidth);
  //addInputTexture(GL_TEXTURE_1D, "coneDirSamplers", m_texIdConeDirSamplers);

#ifdef TEXTURE_SPHERE_SAMPLERS
  addInputInt("sphereSamplersWidth", m_sphereSamplersWidth);
  //addInputTexture(GL_TEXTURE_1D, "sphereSamplers", m_texIdSphereSamplers);
#else
  //addInputVec3Array("sphereSamplersArray", m_sphereSamplers, m_sphereSamplersWidth);
  addInputVec3Array("sphereSamplersArray", m_sphereSamplers, getTotalNumSphereSamplersAccumulated(m_numSpheresByCone)*3);
#endif // TEXTURE_SPHERE_SAMPLERS

  addInputInt("sphereInfoWidth", m_sphereInfoWidth);
  //addInputTexture(GL_TEXTURE_1D, "sphereInfo", m_texIdSphereInfo);

  m_shader->setActive(false);
}

void KernelSSAO_Vox_ConeTracing::generateBitCount16Texture()
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

void KernelSSAO_Vox_ConeTracing::generateConeSamplerTexture()
{
  int numSamplersBeta = ceil(sqrt(float(m_numCones)));            //Longitude  (|||)
  int numSamplersAlpha = ceil(float(m_numCones)/numSamplersBeta); //Latitude   (===)

  m_numCones = numSamplersAlpha*numSamplersBeta;

  m_coneDirSamplersWidth = m_numSamplersDistributions * numSamplersAlpha * numSamplersBeta;
  GLfloat* texData = new GLfloat[m_coneDirSamplersWidth*3];

  for(int k = 0; k < m_numSamplersDistributions; ++k)
  {
    float soma = 0;
    GLfloat *d = &texData[k*numSamplersBeta*numSamplersAlpha*3];
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

        Vector3 dir = Vector3(sin(angleBeta)*sin(angleAlpha), cos(angleBeta), sin(angleBeta)*cos(angleAlpha)); //Longitude
        d[int(soma-betaSize)*3 + j*3 + 0] = dir.x;
        d[int(soma-betaSize)*3 + j*3 + 1] = dir.y;
        d[int(soma-betaSize)*3 + j*3 + 2] = dir.z;
      }
    }
    //printf("%f %d %f!!!!!!!!!!!\n\n", soma, m_numSamplersBeta*m_numSamplersAlpha, soma/(m_numSamplersBeta*m_numSamplersAlpha));
  }

  GLTextureObject t;
  if(!m_texIdConeDirSamplers)
    m_texIdConeDirSamplers = t.createTexture1D(m_coneDirSamplersWidth, GL_RGB32F_ARB, GL_RGB, GL_FLOAT, texData);
  else 
    t.setTexture1D(m_texIdConeDirSamplers, m_coneDirSamplersWidth, GL_RGB32F_ARB, GL_RGB, GL_FLOAT, texData);

  t.setFilters(GL_NEAREST, GL_NEAREST);
  t.setWraps(GL_CLAMP_TO_EDGE);

  delete [] texData;
}


void KernelSSAO_Vox_ConeTracing::generateSphereSamplerTexture()
{
  UniformPoissonDiskSampler u;
  int totalAritProgSum = getTotalNumSphereSamplersAccumulated(m_numSpheresByCone);

  m_sphereSamplersWidth = m_numSamplersDistributions*totalAritProgSum;

#ifdef TEXTURE_SPHERE_SAMPLERS
  GLfloat* texData = new GLfloat[m_sphereSamplersWidth*3];
#else
  if(m_sphereSamplers)
    delete[] m_sphereSamplers;
  m_sphereSamplers = new GLfloat[m_sphereSamplersWidth*3];
  GLfloat* texData = m_sphereSamplers;
#endif // TEXTURE_SPHERE_SAMPLERS

  for(int l = 0; l < m_numSamplersDistributions; ++l)
    for(int k = 0; k < m_numSpheresByCone; ++k)
    {
      int numSphereSamplers = getNumSphereSamplers(k);
      //int numSphereSamplers = m_numSphereSamplers;

      vector<Vector3> v;
      for(int m = 0; m < 10 && v.size() < numSphereSamplers; ++m)
        v = u.sampleCircle(Vector3(0,0,0), 1.0f, (1+m*0.1f)*sqrt(1.0f/numSphereSamplers)*1.35f);
      while(v.size() > numSphereSamplers)
      {
        int k = rand()%v.size();
        v.erase(v.begin()+k);
      }
      assert(v.size() == numSphereSamplers);

      int aritProgSum = getNumSphereSamplersAccumulated(k);

      GLfloat *d = &texData[l*totalAritProgSum*3 + aritProgSum*3];

      for(int i = 0; i < numSphereSamplers; ++i)
      {
        d[i*3 + 0] = v[i].x;
        d[i*3 + 1] = v[i].y;
        d[i*3 + 2] = v[i].z;
      }
    }

#ifdef TEXTURE_SPHERE_SAMPLERS
    GLTextureObject t;
    if(!m_texIdSphereSamplers)
      m_texIdSphereSamplers = t.createTexture1D(m_sphereSamplersWidth, GL_RGB32F_ARB, GL_RGB, GL_FLOAT, texData);
    else 
      t.setTexture1D(m_texIdSphereSamplers, m_sphereSamplersWidth, GL_RGB32F_ARB, GL_RGB, GL_FLOAT, texData);

    t.setFilters(GL_NEAREST, GL_NEAREST);
    t.setWraps(GL_CLAMP_TO_EDGE);

    delete [] texData;
#endif // TEXTURE_SPHERE_SAMPLERS

}


void KernelSSAO_Vox_ConeTracing::generateSphereInfoTexture()
{
  m_sphereInfoWidth = m_numSamplersDistributions*m_numSpheresByCone;
  GLfloat* texData = new GLfloat[m_sphereInfoWidth*2];

  for(int l = 0; l < m_numSamplersDistributions; ++l)
  {
    GLfloat *d = &texData[l*m_numSpheresByCone*2];
    
    m_sphereInfo->setParameters(m_coneRevolutionAngle, m_numSpheresByCone, m_numCones);
    for(int k = 0; k < m_numSpheresByCone; ++k)
    {
      float r = 1.0;
      if(l > 0)
        r = (1.0f + float(rand()%10)/100 - .05f);

      float center, radius;
      m_sphereInfo->getSphereInfo(k, &center, &radius);
      d[k*2 + 0] = center*r;
      d[k*2 + 1] = radius*r;
    }
  }

  GLTextureObject t;
  if(!m_texIdSphereInfo)
    m_texIdSphereInfo = t.createTexture1D(m_sphereInfoWidth, GL_LUMINANCE_ALPHA32F_ARB, GL_LUMINANCE_ALPHA, GL_FLOAT, texData);
  else 
    t.setTexture1D(m_texIdSphereInfo, m_sphereInfoWidth, GL_LUMINANCE_ALPHA32F_ARB, GL_LUMINANCE_ALPHA, GL_FLOAT, texData);

  t.setFilters(GL_NEAREST, GL_NEAREST);
  t.setWraps(GL_CLAMP_TO_EDGE);
  delete [] texData;
}

void KernelSSAO_Vox_ConeTracing::renderConeDistribution(int distribution)
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glPushMatrix();

  int numSamplersBeta = ceil(sqrt(float(m_numCones)));            //Longitude  (|||)
  int numSamplersAlpha = ceil(float(m_numCones)/numSamplersBeta); //Latitude   (===)

  GLTextureObject t(m_texIdConeDirSamplers, GL_TEXTURE_1D);
  GLfloat *texData =  &t.read1DTextureFloatData(GL_RGB)[distribution*numSamplersAlpha*numSamplersBeta*3];

  GLTextureObject t2(m_texIdSphereInfo, GL_TEXTURE_1D);
  GLfloat *texDataInfo =  &t2.read1DTextureFloatData(GL_LUMINANCE_ALPHA)[distribution*m_numSpheresByCone *2];

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
  glRotated(f+=3, 0,1,0);
  glTranslated(2,0,0);
  m_pointLight.render();
  glPopMatrix();


  glEnable(GL_NORMALIZE);
  for(int i = 0; i < numSamplersAlpha; ++i)
  {
    for(int j = 0; j < numSamplersBeta; ++j)
    {
      glPushMatrix();
      Vector3 dir(
        texData[i*numSamplersBeta*3 + j*3 + 0],
        texData[i*numSamplersBeta*3 + j*3 + 1],
        texData[i*numSamplersBeta*3 + j*3 + 2]);
      dir = dir.unitary();
      float angle = dir.angle(Vector3(0,1,0));
      Vector3 axis = dir^Vector3(0,1,0);
      if(!(axis == Vector3(0,0,0)))
      {
        axis = axis.unitary();
        glRotated(-angle, axis.x, axis.y, axis.z);
      }

      //SPHERES
      //SPHERES
      //SPHERES
      GLfloat color2[4] = {0,1,1,1};
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  color2);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  color2);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color2);

      for(int i = 0; i < m_numSpheresByCone; ++i)
      {
        glPushMatrix();

        Vector3 center = getSphereCenter(Vector3(0,0,0), Vector3(0,1,0), 1, texDataInfo[i*2 + 0]);
        float radius = getSphereRadius(1, texDataInfo[i*2 + 1]);

        glTranslatef(center.x, center.y, center.z);
        glutSolidSphere(radius, 30, 30);
        glPopMatrix();
      }

      //CONES
      //CONES
      //CONES
      /**/
      glRotated(90,1,0,0);
      glTranslatef(0,0,-1);
      float coneHeight = 1.0f;
      float coneAngle = m_coneRevolutionAngle;
      //float coneAngle = atan(sqrt(2.0f/(m_numCones)));
      float coneRadius = coneHeight*tan(coneAngle);

      GLfloat color[4] = {1,1,0,1};
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  color);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  color);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);

      glutWireCone(coneRadius, coneHeight, 50, 50);
      /**/
      glPopMatrix();

    }
  }
  glPopMatrix();
  glPopAttrib();
}


void KernelSSAO_Vox_ConeTracing::renderSphereSamplerDistribution(int distribution, int sphereIndex)
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

void KernelSSAO_Vox_ConeTracing::renderSphereInfoDistribution(int distribution)
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
