//////SHADER CONTROLS\\\\\\\\
//////SHADER CONTROLS\\\\\\\\
//////SHADER CONTROLS\\\\\\\\
//////SHADER CONTROLS\\\\\\\\


#define ONE_MINUS_AO
//#define SIZE_OVER_DEPTH
//#define DEPTH_PEELING
#define MAX_OVER_PEELING
#define SPHERE_POSITION
#define SPHERE_CENTER_MINUS_NORMAL .1
#define INV_DIST_DIVIDE 2.

//#define Z_MINUS_R -.5
//#define INVERT_NORMAL

//#define SAMPLER_QUAD
#define SAMPLER_VECTOR

//////SHADER BEGIN\\\\\\\\
//////SHADER BEGIN\\\\\\\\
//////SHADER BEGIN\\\\\\\\
//////SHADER BEGIN\\\\\\\\
varying vec3 lightDir;
uniform sampler2D positionTex;
uniform sampler2D depth0_normalTex;
uniform sampler2D depth1_normalTex;
uniform sampler2D depth2_normalTex;

#ifdef SAMPLER_QUAD
  uniform sampler2D sampleTex;
#else
  uniform sampler1D sampleTex;
#endif
uniform float samplerSize;

uniform float pixelmask;

uniform float rfar;

uniform float screenWidth;
uniform float screenHeight;
uniform float near;
uniform float far;
uniform float right;
uniform float top;

uniform float offsets_size;
uniform float intensity;

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

vec4 getSphere(float xw, float yw, float zw);
float getAproxAO(vec4 sphereQ, vec3 posP, vec3 normalP);
float calcLocalAO(float i, float j, float depth, vec4 depth0_normal, float dx, float dy, vec4 position, inout int n);





void main()
{
  //vec4 diffuse = texture2D(diffuseTex,  gl_TexCoord[0].st);
  //vec4 color = texture2D(colorTex,  gl_TexCoord[0].st);
  vec4 color = vec4(1,1,1,1);
  vec4 depth0_normal = texture2D(depth0_normalTex,  gl_TexCoord[0].st);
  float depth = depth0_normal.a;
  
  #ifdef SPHERE_POSITION
    vec4 position = getSphere(gl_FragCoord.x , gl_FragCoord.y , depth);
  #else
    vec4 position = texture2D(positionTex,  gl_TexCoord[0].st);
  #endif


	if(depth < 0.0)
	{
		gl_FragData[0] = vec4(.8, .8, 1.0, -1.0);
		return;
	}
  //vec3 nn = (depth0_normal.xyz*1.)*.5 + .5;
  //gl_FragData[0] = vec4(nn, 1.);
  //return;
  
  //vec4 pp = getSphere(gl_FragCoord.x, gl_FragCoord.y , depth);
  //if(length(pp.xyz - position.xyz) < .1)
    //gl_FragData[0] = RED;
  //else gl_FragData[0] = BLUE;
  //return;
  
  
  float dx = 1.0/screenWidth;
  float dy = 1.0/screenHeight;
  float totalAO = 0.0;
  
  float samplerTotalSize = samplerSize*2. + 1.;
 
  
#ifdef SAMPLER_QUAD
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
  //if (size == samplerSize)
    //gl_FragData[0] = ORANGE;
  //else if (size > samplerSize*.9)
    //gl_FragData[0] = WHITE;
  //else if (size > samplerSize*.75)
    //gl_FragData[0] = RED;
  //else if (size > samplerSize*.6)
    //gl_FragData[0] = CYAN;
  //else if (size > samplerSize*.45)
    //gl_FragData[0] = BLUE;
  //else if (size > samplerSize*.3)
    //gl_FragData[0] = BLACK;
  //else if (size > samplerSize*.15)
    //gl_FragData[0] = GREEN;
  //else if (size >= samplerSize*.05)
    //gl_FragData[0] = YELLOW;
  //else 
    //gl_FragData[0] = PINK;
  //return;

  int n = 0;
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
    if(i > 3. || j > 3.)
    {
      i *= offsets_size;
      j *= offsets_size;
    }
   
    
    totalAO += calcLocalAO(i,j, depth, depth0_normal, dx, dy, position, n);
  }
#endif

  //float detph_weight = 1.-pow((depth),50.10);
  float detph_weight = 1.;
  //gl_FragData[0] = WHITE*detph_weight;
  //return;
  totalAO = intensity * detph_weight * totalAO/(PI*float(n));
  totalAO = clamp(totalAO,0.0,1.0);

	if(depth <= 1.0)
	{
	  #ifdef ONE_MINUS_AO
	    gl_FragData[0] = color*(1.0 - totalAO);
	  #else
	    gl_FragData[0] = color*(totalAO);
	  #endif
	  
	  if(totalAO < 0.0)
		  gl_FragData[0] = YELLOW;
	  //else if(totalAO >= 1.0)
		  //gl_FragData[0] = CYAN;
		//else if((float(size)/samplerSize) >= 1)
		  //gl_FragData[0] = GREEN;
		//else if((float(size)/samplerSize) < 1)
		  //gl_FragData[0] = BLUE;
	  //else if(totalAO >= 0.95)
		  //gl_FragData[0] = BLUE;
		//else if(totalAO <= 900.0)
		  //gl_FragData[0] = GREEN;
		//else  gl_FragData[0] =  PINK;
		//else   gl_FragData[0] =  RED*totalAO;
		//if(t == false)
	    //gl_FragData[0] =  vec4(1,0,0,1) ;
	  //else 
	  //gl_FragData[0] =  getSphere(gl_FragCoord.x, gl_FragCoord.y , depth);
	  //gl_FragData[0] = vec4(1,0,0,1)*(1.0 - totalAO);
	  	  //gl_FragData[0] = vec4(1,0,0,1)*totalAO;
  }
	else gl_FragData[0] = vec4(1,1,1,-1)*pixelmask;
  //gl_FragData[0] = max(dot(normal, normalize(lightDir)), 0.0) * gl_FrontMaterial.diffuse;
	//gl_FragData[1] = vec4(normalize(normal), gl_FragCoord.z);
	//gl_FragData[2] = gl_FrontMaterial.diffuse;
	//gl_FragData[3] = vec4(vec3(gl_FrontMaterial.specular.rgb), gl_FrontMaterial.shininess);
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
    vec2 inc = vec2(i*dx, j*dy);
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

    vec4 sphere = getSphere(gl_FragCoord.x + i, gl_FragCoord.y + j, depth_normal.a);
    #ifdef SPHERE_CENTER_MINUS_NORMAL
      sphere.xyz = sphere.xyz - normalize(depth_normal.xyz)*SPHERE_CENTER_MINUS_NORMAL;
    #endif

    
    float eye_dist = length(sphere.xyz - position.xyz);

    if(eye_dist > rfar)
    #ifdef DEPTH_PEELING
      continue;
    #else
      return localAO;
    #endif

    #ifdef INV_DIST_DIVIDE
      float dist_divide = 1./pow(eye_dist,INV_DIST_DIVIDE);
    #else  
      float dist_divide = 1.;
    #endif  
    
    #ifdef MAX_OVER_PEELING
      localAO = max(localAO, getAproxAO(sphere, position.xyz, depth0_normal.xyz)/dist_divide);
      nn = 1;
    #else
      localAO += (getAproxAO(sphere, position.xyz, depth0_normal.xyz)/dist_divide);
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
  float zn = (2. * w.z - 1.)/1.;
  
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
  //float rr = abs(ex2.x - ex1.x)/2.;
  float rr = length(ex2 - ex1)/2.;
  #ifdef Z_MINUS_R
    ex1.z -= rr * Z_MINUS_R;
  #endif
  return vec4(ex1,rr);
    
  //float xn = 2.0 * xw/screenWidth - 1.0;
  //float yn = 2.0 * yw/screenHeight - 1.0;
  //float zn = 2.0 * zw - 1.0;
  //
  //float ze = 2.0 * far * near/(zn * (far - near) - (far + near));
  //float ye = -ze * yn * top/near;
  //float xe = -ze * xn * right/near;
//
  //float r = -ze * right/(screenWidth * near);
//

  //if(rr - r < 0.00001)
    //return vec4(xe, ye, ze, r);
  //else return vec4(-512,-512,-512, r);
}

float getAproxAO(vec4 sphereQ, vec3 posP, vec3 normalP)
{
  vec3 PQ = (sphereQ.xyz - posP);
  float S = 2.0 * PI * (1.0 - cos(asin(sphereQ.a / length(PQ))));
  #ifdef INVERT_NORMAL
    normalP = -normalP;
  #endif
  float max_dot = max(dot(normalize(normalP), normalize(PQ)), 0.0);
  return S*max_dot;
}
