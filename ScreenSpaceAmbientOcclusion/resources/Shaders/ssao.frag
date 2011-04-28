varying vec3 lightDir;
uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D positionTex;
uniform sampler2D depth0_normalTex;
uniform sampler2D depth1_normalTex;
uniform sampler2D depth2_normalTex;
uniform sampler2D sampleTex;
uniform float samplerSize;

uniform float rfar;
uniform float pixelmask_size;
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

vec4 getSphere(float xw, float yw, float zw);
float getAproxAO(vec4 sphereQ, vec3 posP, vec3 normalP);



void main()
{
  vec4 diffuse = texture2D(diffuseTex,  gl_TexCoord[0].st);
  vec4 specular = texture2D(specularTex,  gl_TexCoord[0].st);
  vec4 depth0_normal = texture2D(depth0_normalTex,  gl_TexCoord[0].st);
  vec4 position = texture2D(positionTex,  gl_TexCoord[0].st);
  float depth = depth0_normal.a;
	if(depth < 0.0)
	{
		gl_FragData[0] = vec4(.8, .8, 1.0, -1.0);
		return;
	}

  float dx = 1.0/screenWidth;
  float dy = 1.0/screenHeight;
  float totalAO = 0.0;
  
  float samplerTotalSize = samplerSize*2. + 1.;
 
  //pixelmask_size = min(samplerSize, pixelmask_size);
  
  float v = pixelmask_size;
  float n_depth = clamp(depth - v,0.,v)*1./(1.-v);
  int size = max(floor((1.-n_depth)*samplerSize + .5), 1);

  //if (size > 25)
    //gl_FragData[0] = WHITE;
  //else if (size > 24)
    //gl_FragData[0] = BLACK;
  //else if (size > 20)
    //gl_FragData[0] = RED;
  //else if (size > 15)
    //gl_FragData[0] = CYAN;
  //else if (size > 10)
    //gl_FragData[0] = BLUE;
  //else if (size > 5)
    //gl_FragData[0] = GREEN;
  //else if (size > 3)
    //gl_FragData[0] = YELLOW;
  //else 
    //gl_FragData[0] = PINK;
  //return;
  for(int i=-size; i < size + 1; ++i)
    for(int j = -size; j < size + 1; ++j)
      if(!(i==0 && j==0))
      {
        vec2 coord = vec2((float(i) + samplerSize + .5)/samplerTotalSize, (float(j) + samplerSize + .5)/samplerTotalSize);
        float sample = floor(texture2D(sampleTex, coord).a + .5);
        if(sample == 1.0)
        {
          for(int k = 0; k < 3; ++k)
          //int k = 0;
          {
            vec2 inc = vec2(float(i)*dx, float(j)*dy);
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
            }
            
            if(depth_normal.a < 0.0)
              continue;
            if(depth_normal.a >= depth)
              continue;

            vec4 sphere = getSphere(gl_FragCoord.x + float(i), gl_FragCoord.y + float(j), depth_normal.a);
            
            float eye_dist = length(sphere.xyz - position.xyz);
            //sphere.xyz = sphere.xyz - normalize(depth_normal.xyz)*.1;
            
            if(eye_dist > rfar)
              continue;
            totalAO += (getAproxAO(sphere, position.xyz, depth0_normal.xyz));//pow(eye_dist,2);

            if(totalAO >= 1.0)
            {
              totalAO = 1.0;
              break;
            }
          }
        }
      }
      

	//gl_FragData[0] = gl_FrontMaterial.diffuse*(1.0-totalAO);
	if(depth <= 1.0)
	{
	  //if(getSphere(gl_FragCoord.x , gl_FragCoord.y, depth).a > 0.3)
	    //gl_FragData[0] = RED;
	  //else gl_FragData[0] = BLUE;
	  //gl_FragData[0] = depth0_normal;
	  //return;

	  //gl_FragData[0] = RED*(1. - clamp(totalAO, 0.,1.));
	  gl_FragData[0] = RED*(1. - totalAO);
	  if(totalAO < 0.0)
		  gl_FragData[0] = YELLOW;
	  //else if(totalAO == 0.0)
		  //gl_FragData[0] = CYAN;
		//else if(totalAO < 0.05)
		  //gl_FragData[0] = GREEN;
		////else if(totalAO < 0.5)
		  ////gl_FragData[0] = BLUE;
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
	else gl_FragData[0] = vec4(1,1,1,-1);
  //gl_FragData[0] = max(dot(normal, normalize(lightDir)), 0.0) * gl_FrontMaterial.diffuse;
	//gl_FragData[1] = vec4(normalize(normal), gl_FragCoord.z);
	//gl_FragData[2] = gl_FrontMaterial.diffuse;
	//gl_FragData[3] = vec4(vec3(gl_FrontMaterial.specular.rgb), gl_FrontMaterial.shininess);
}


vec4 getSphere(float xw, float yw, float zw)
{
  //assuming window (0,0), (w, h)
  //assuming r == -l, t == -b
  float xn = 2.0 * xw/screenWidth - 1.0;
  float yn = 2.0 * yw/screenHeight - 1.0;
  float zn = 2.0 * zw - 1.0;
  
  float ze = 2.0 * far * near/(zn * (far - near) - (far + near));
  float ye = -ze * yn * top/near;
  float xe = -ze * xn * right/near;

  float r = -ze * right/(screenWidth * near);
 
  //ze -= r;
  return vec4(xe, ye, ze, r);
}

float getAproxAO(vec4 sphereQ, vec3 posP, vec3 normalP)
{
  vec3 PQ = (sphereQ.xyz - posP);
  float S = 2.0 * PI * (1.0 - cos(asin(sphereQ.a / length(PQ))));
  float max_dot = max(dot(normalize(normalP), normalize(PQ)), 0.0);
  return S*max_dot;
}
