uniform sampler2D positionTex;
uniform sampler2D normalTex;
uniform sampler2D diffuseTex;
uniform sampler2D specularTex;

uniform sampler1D lightTex;
uniform float lightTexSize;

uniform mat4 lightModelViewMatrix;


void calcDirLight(float i, vec3 N, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular);
void calcPointLight(float i, vec3 N, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular);
void calcSpotLight(float i, vec3 N, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular);

//lightStruct
//{
//0|   difuse   //alpha == spotExponent                           [0-3]
//1|   specular //alpha == enable or disable                      [4-7]
//2|   pos      //w == type :0 directional, 1 point, 2 = spot     [8-11]
//3|   spot     //rgb == spotDir, a == spotAngle(rad)             [12-15]
//}

const vec4 clearColor  = vec4(0.8, 0.8, 0.8, 1.0);
const vec3 defaultAmbientMaterial  = vec3(0.2, 0.2, 0.2);
vec3 ambient, diffuse, specular;

vec4 lightSpecular;
vec4 lightPosition;
vec3 lightDir;

vec3 fragPos;
vec3 eyeDir;

struct material
{
  vec3 diffuse;
  vec3 specular;
  float shininess;
}fragMaterial;


void main()
{

  ambient = defaultAmbientMaterial;
  diffuse = vec3(0, 0, 0);
  specular = vec3(0, 0, 0);

  vec4 fragPos = texture2D(positionTex, gl_TexCoord[0].st);

  if (floor(fragPos.a+.5)==-1.0) //Clear Color
    discard;

  vec3 normal = normalize(texture2D(normalTex, gl_TexCoord[0].st).xyz);

  eyeDir = -fragPos.xyz/fragPos.w;
  vec4 materialTexInfo = texture2D(specularTex, gl_TexCoord[0].st);



  fragMaterial.diffuse = texture2D(diffuseTex, gl_TexCoord[0].st).rgb;
  fragMaterial.specular = materialTexInfo.rgb;
  fragMaterial.shininess = materialTexInfo.a;


  float numLights = lightTexSize/4.0;
  for (float i = 0.0; i<numLights; i += 1.0)
  {
    lightSpecular = texture1D(lightTex, (i*4.0 + 1.0 + 0.5)/lightTexSize); //A soma de 0.5 é pq o acesso a textura é feito no meio do texel e não na borda

    if (floor(lightSpecular.a + .5) != 0.0) //Is Light Enabled?
    {
      lightPosition = texture1D(lightTex, (i*4.0 + 2.0 + 0.5)/lightTexSize);
      float lightType = floor(lightPosition.w + 0.5);
      if (lightType == 0.0) //Directional Light
      {
        lightPosition.xyz = (lightModelViewMatrix * vec4(lightPosition.xyz, 0.0)).xyz;
        lightDir = lightPosition.xyz;
        calcDirLight(i, normal, ambient, diffuse, specular);
//            ambient = diffuse = specular = vec3(0,1,0); //DEBUG
      }
      else if (lightType == 2.0) //Spot Light
      {
        lightPosition.xyz = (lightModelViewMatrix * vec4(lightPosition.xyz, 1.0)).xyz;
        lightDir = lightPosition.xyz - fragPos.xyz;
        calcSpotLight(i, normal, ambient, diffuse, specular);
//            ambient = diffuse = specular = vec3(1,0,0); //DEBUG
      }
      else //Point Light
      {
        lightPosition.xyz = (lightModelViewMatrix * vec4(lightPosition.xyz, 1.0)).xyz;
        lightDir =  lightPosition.xyz - fragPos.xyz;
        calcPointLight(i, normal, ambient, diffuse, specular);
//            ambient = diffuse = specular = vec3(0,0,1); //DEBUG
      }

    }

  }
  vec3 intensity = ambient + diffuse;
//  gl_FragColor = vec4(intensity + specular, 1.0);
  gl_FragData[0] = vec4(intensity + specular, 1.0);
  /**/
}

void calcDirLight(float i, vec3 N, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular)
{
  vec3 L = normalize(lightDir);
  vec3 H = normalize(L + normalize(eyeDir));

  float NdotL = max(0.0, dot(N, L));
  if ( NdotL > 0.0 )
  {
    vec3 lightDiffuse = texture1D(lightTex, (i*4.0+.5)/lightTexSize).rgb;
    float att = 1.0; //future work
    float NdotH = max(0.0, dot(N, H));

    diffuse += fragMaterial.diffuse * lightDiffuse * NdotL;

    specular += fragMaterial.specular * lightSpecular.rgb * pow(NdotH, fragMaterial.shininess);
  }
}


void calcPointLight(float i, vec3 N, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular)
{
  vec3 L = normalize(lightDir);
  vec3 H = normalize(L + normalize(eyeDir));

  float NdotL = max(0.0, dot(N, L));
  if ( NdotL > 0.0 )
  {
    vec3 lightDiffuse = texture1D(lightTex, (i*4.0+.5)/lightTexSize).rgb;
    float att = 1.0; //future work
    float NdotH = max(0.0, dot(N, H));

    diffuse += fragMaterial.diffuse * lightDiffuse * NdotL;

    specular += fragMaterial.specular * lightSpecular.rgb * pow(NdotH, fragMaterial.shininess);
  }
}


void calcSpotLight(float i, vec3 N, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular)
{
  vec3 L = normalize(lightDir);
  vec3 H = normalize(L + normalize(eyeDir));

  float NdotL = max(0.0, dot(N, L));
  if ( NdotL > 0.0 )
  {
    vec4 lightSpotInfo = texture1D(lightTex, (i*4.0 + 3.0 + .5)/lightTexSize);
    vec3 lightSpotDir = normalize((lightModelViewMatrix * vec4(lightSpotInfo.xyz, 0.0)).xyz);

    float lightSpotAngle = cos(lightSpotInfo.a);

    float spotEffect = dot(lightSpotDir,-L);
    if (spotEffect > lightSpotAngle)
    {
      vec4 lightDiffuse = texture1D(lightTex, (i*4.0 + .5)/lightTexSize);
      float att = 1.0; //future work
      float NdotH = max(0.0, dot(N, H));

      spotEffect = pow(spotEffect, lightDiffuse.a);
      diffuse += fragMaterial.diffuse * lightDiffuse.rgb * NdotL * spotEffect;

      specular += fragMaterial.specular * lightSpecular.rgb * pow(NdotH, fragMaterial.shininess)*spotEffect;
    }
  }
}
