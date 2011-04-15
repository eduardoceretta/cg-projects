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
vec4 getSphere(float xw, float yw, float zw);
float getAproxAO(vec4 sphereQ, vec3 posP, vec3 normalP);

void main()
{
  vec4 diffuse = texture2D(diffuseTex,  gl_TexCoord[0].st);
  vec4 specular = texture2D(specularTex,  gl_TexCoord[0].st);
  vec4 depth0_normal = texture2D(depth0_normalTex,  gl_TexCoord[0].st);
  vec4 position = texture2D(positionTex,  gl_TexCoord[0].st);
  float depth = depth0_normal.a;
  //if(depth < 0.)
    //discard;
    
  //float dx = dFdx(gl_TexCoord[0].s);
  //float dy = dFdy(gl_TexCoord[0].t);
  
  float dx = 1.0/screenWidth;
  float dy = 1.0/screenHeight;
  
  float totalAO = 0.0;
  for(int i=-1; i < 2; ++i)
    for(int j = -1; j < 2; ++j)
      if(!(i==0 && j==0))
      //int i = 1;
      //int j = 0;
      {
        vec2 inc = vec2(float(i)*dx, float(j)*dy);
        vec4 depth_normal = texture2D(depth0_normalTex,  gl_TexCoord[0].st + inc);
        vec4 sphere = getSphere(gl_FragCoord.x + float(i), gl_FragCoord.y + float(j), depth_normal.a);
        totalAO += getAproxAO(sphere, position.xyz, depth0_normal.xyz);
      }
      
  //vec4 depth1_normal = texture2D(depth1_normalTex,  gl_TexCoord[0]);
  //vec4 depth2_normal = texture2D(depth2_normalTex,  gl_TexCoord[0]);

	//|| gl_FragCoord.z <= depth)
	//{
	  ////gl_FragData[0] = vec4(1,1,0,1);
	  ////return;
	  //discard;
	//}

	//gl_FragData[0] = gl_FrontMaterial.diffuse*(1.0-totalAO);
	if(depth < 0.0)
		gl_FragData[0] = vec4(1,1,0,1);
	else if(depth <= 1.0)
	{
	  //if(totalAO >= 0.0)
		  //gl_FragData[0] = vec4(0,0,1,1);
		//else  gl_FragData[0] =  vec4(0,1,0,1);
				 gl_FragData[0] =  diffuse * (1 - totalAO) ;
  }
	else gl_FragData[0] = vec4(1,1,1,1);
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
 
  //return vec4(far, near, zn * (far - near), (zn * (far - near) - (far + near)));
  return vec4(xe, ye, ze, r);
}

float getAproxAO(vec4 sphereQ, vec3 posP, vec3 normalP)
{
  vec3 PQ = (posP - sphereQ.xyz);
  float S = 2.0 * PI * (1.0 - cos(asin( sphereQ.a / length(PQ))));
  float max_dot = max(dot(normalize(normalP), normalize(PQ)), 0.0);
  return max_dot;
    //return S*max_dot;
}
