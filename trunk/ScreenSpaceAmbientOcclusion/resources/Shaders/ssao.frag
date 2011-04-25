varying vec3 lightDir;
uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D positionTex;
uniform sampler2D depth0_normalTex;
uniform sampler2D depth1_normalTex;
uniform sampler2D depth2_normalTex;
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

  //float dx = dFdx(gl_TexCoord[0].s);
  //float dy = dFdy(gl_TexCoord[0].t);
  
  float dx = 1.0/screenWidth;
  float dy = 1.0/screenHeight;
  //bool t = true;
  float totalAO = 0.0;
  int size = 3;
  float rfar = 0.00001;
  for(int i=-size; i < size + 1; ++i)
    for(int j = -size; j < size + 1; ++j)
      if(!(i==0 && j==0))
      {
        vec2 inc = vec2(float(i)*dx, float(j)*dy);
        vec4 depth_normal = texture2D(depth0_normalTex,  gl_TexCoord[0].st + inc);
        if(depth_normal.a < 0.0)
          continue;
        if(depth_normal.a >= depth)
          continue;
        if(depth - depth_normal.a > rfar)
          continue;
        //if(depth_normal.a < 0.0 || depth_normal.a > depth)
          //continue;
        vec4 sphere = getSphere(gl_FragCoord.x + float(i), gl_FragCoord.y + float(j), depth_normal.a);
        totalAO += getAproxAO(sphere, position.xyz, depth0_normal.xyz);
      }
      

	//gl_FragData[0] = gl_FrontMaterial.diffuse*(1.0-totalAO);
	if(depth <= 1.0)
	{
	  
	  //gl_FragData[0] = depth0_normal;
	  gl_FragData[0] = RED*(1. - clamp(totalAO, 0.,1.));
	  //if(totalAO < 0.0)
		  //gl_FragData[0] = YELLOW;
	  //else if(totalAO == 0.0)
		  //gl_FragData[0] = CYAN;
		////else if(totalAO < 0.5)
		  ////gl_FragData[0] = BLUE;
	  ////else if(totalAO <= 1.0)
		  ////gl_FragData[0] = RED;
		//else if(totalAO >= 5.0)
		  //gl_FragData[0] = GREEN;
		////else  gl_FragData[0] =  PINK;
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
 
  //ze += r;
  //return vec4(far, near, zn * (far - near), (zn * (far - near) - (far + near)));
  return vec4(xe, ye, ze, r);
}

float getAproxAO(vec4 sphereQ, vec3 posP, vec3 normalP)
{
  vec3 PQ = (posP - sphereQ.xyz);
  float S = 2.0 * PI * (1.0 - cos(asin( sphereQ.a / length(PQ))));
  float max_dot = max(dot(normalize(normalP), normalize(PQ)), 0.0);
  //return  sphereQ.a / length(PQ);
    return S;
    //return S*max_dot;
}
