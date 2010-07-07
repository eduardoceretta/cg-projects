/*
struct gl_LightSourceParameters {
		vec4 ambient;
		vec4 diffuse;
		vec4 specular;
		vec4 position;
		vec4 halfVector;
		vec3 spotDirection;
		float spotExponent;
		float spotCutoff; // (range: [0.0,90.0], 180.0)
		float spotCosCutoff; // (range: [1.0,0.0],-1.0)
		float constantAttenuation;
		float linearAttenuation;
		float quadraticAttenuation;
	};
*/


//uniform sampler2D texture_0;
uniform sampler2D tex,l3d;

varying vec4 color,ambient;
varying vec3 normal,ecPos3;

uniform int numLights;


void calcDirLight(int i, vec3 n, inout vec4 diffuse,inout vec4 specular);
void calcPointLight(int i, vec3 n,vec3 ecPos3, inout vec4 diffuse,inout vec4 specular);
void calcSpotLight(int i, vec3 n,vec3 ecPos3,inout vec4 diffuse,inout vec4 specular);



void main()
{

   float cosNorLig;
   vec4 intensity;//,amb,diffuse,specular;

   vec4 amb,diffuse,specular;

   vec4 c = vec4(0,0,0,0);
   vec3 n,lightDir[gl_MaxLights];

   amb = ambient;

   specular = vec4(0,0,0,1);
   diffuse = vec4(0,0,0,1);

   n = normalize(normal);


  if(all(equal(gl_FrontMaterial.emission.xyz,vec3(0.0,0.0,0.0))))
  {
    for(int i=0;i<numLights;++i)
    {
      if(gl_LightSource[i].position.w==0.0)
      {
        calcDirLight(i, n, diffuse,specular);
      }
      else if(gl_LightSource[i].spotCutoff==180.0)
      {
        calcPointLight(i, n,ecPos3, diffuse,specular);
      }
      else
      {
        calcSpotLight(i, n, ecPos3, diffuse,specular);
      }
      amb += gl_FrontMaterial.ambient * gl_LightSource[i].ambient;
    }

    intensity = specular +  diffuse + amb;

  }else  intensity = gl_FrontMaterial.emission;

  c=intensity;

  gl_FragColor = c;
}


void calcDirLight(int i, vec3 n,inout vec4 diffuse,inout vec4 specular)
{
   float cosNorHv;
   vec3 halfV;
   float cosNorLig = max(dot(n,normalize(gl_LightSource[i].position.xyz)),0.0);

   if (cosNorLig > 0.0)
   {
      diffuse += gl_FrontMaterial.diffuse * gl_LightSource[i].diffuse*cosNorLig;

      halfV = gl_LightSource[i].halfVector.xyz;
      cosNorHv = max(dot(n,halfV),0.0);
      specular += gl_FrontMaterial.specular * gl_LightSource[i].specular * pow(cosNorHv,gl_FrontMaterial.shininess);
   }
}


void calcPointLight(int i, vec3 n, vec3 ecPos3,inout vec4 diffuse,inout vec4 specular)
{
   float att = 1.0;
   vec3 viewV,ldir;

   float cosNorHv;
   vec3 halfV;
   float dist;
   vec3 aux;

   aux = vec3(gl_LightSource[i].position.xyz-ecPos3);
   float cosNorLig = max(dot(n,normalize(aux)),0.0);

   vec3 eye = -normalize(ecPos3).xyz;


   if (cosNorLig > 0.0)
   {
      dist = length(aux);
      aux = normalize(aux);

      diffuse += gl_FrontMaterial.diffuse * gl_LightSource[i].diffuse*cosNorLig*att;

      halfV = normalize(aux+eye);
      cosNorHv= max(dot(n,halfV),0.0);
		specular += gl_FrontMaterial.specular * gl_LightSource[i].specular * pow(cosNorHv,gl_FrontMaterial.shininess)*att;

	}

}


void calcSpotLight(int i, vec3 n, vec3 ecPos3,inout vec4 diffuse,inout vec4 specular)
{

  vec3 L = normalize(gl_LightSource[i].position.xyz - ecPos3);

  float NdotL = max(0.0, dot(n, L));
  if ( NdotL > 0.0 )
  {
    float lightSpotAngle = cos(radians(gl_LightSource[i].spotCutoff));

    float spotEffect = dot(gl_LightSource[i].spotDirection,-L);
    if (spotEffect > lightSpotAngle)
    {
      diffuse += gl_FrontMaterial.diffuse * gl_LightSource[i].diffuse*NdotL;

      vec3 halfV = normalize(L - normalize(ecPos3));
      float cosNorHv = max(dot(n,halfV),0.0);
      specular += gl_FrontMaterial.specular * gl_LightSource[i].specular * pow(cosNorHv,gl_FrontMaterial.shininess);
    }
  }
}
