uniform sampler2D rayDir;
uniform sampler2D triangleInfo;

uniform sampler1D vertexes;
uniform sampler1D normals;
uniform sampler1D diffuseTex;
uniform sampler1D especularTex;
uniform sampler1D lights;

uniform float normalsSize;
uniform float vertexesSize;
uniform float diffuseSize;
uniform float especularSize;
uniform float lightsSize;

uniform vec3 eyePos;


void calcDirLight(float i, vec3 N, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular);
void calcPointLight(float i, vec3 N, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular);
void calcSpotLight(float i, vec3 N, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular);

#define INACTIVE 0
#define ACTIVE_TRAVERSE 1
#define ACTIVE_INTERSECT 2
#define ACTIVE_SHADING 3
#define DONE 4
#define OVERFLOW 5

//lightStruct
//{
//0|   difuse   //alpha == spotExponent
//1|   specular //alpha == enable or disable
//2|   pos      //w == type :0 directional, 1 point, 2 = spot
//3|   spot     //rgb == spotDir, a == spotAngle(rad)
//}
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
   vec4 rDir = texture2D(rayDir, gl_TexCoord[0].st);
   int triangleFlag = int(floor(rDir.w+.5));

   if(triangleFlag == ACTIVE_SHADING)
   {
      vec4 triangleInfos = texture2D(triangleInfo, gl_TexCoord[0].st);
      fragPos = triangleInfos.rgb;
      float triangleIndex = floor(triangleInfos.a + .5);

      vec3 normal = texture1D(normals, triangleIndex/normalsSize).xyz;
      ambient = defaultAmbientMaterial;
      diffuse = vec3(0, 0, 0);

      vec4 matInfo = texture1D(diffuseTex, triangleIndex/diffuseSize);
      specular = vec3(0, 0, 0);
      eyeDir = eyePos - fragPos;

      fragMaterial.specular = texture1D(diffuseTex, triangleIndex/diffuseSize).rgb;
      fragMaterial.diffuse = matInfo.rgb;
      fragMaterial.shininess = matInfo.a;

      float numLights = lightsSize/4.0;
      for(float i = 0.0; i<numLights; i += 1.0)
      {
         lightSpecular = texture1D(lights, (i*4.0 + 1.0)/lightsSize);
         if(floor(lightSpecular.a + .5) != 0.0) //Is Light Enabled?
         {
            lightPosition = texture1D(lights, (i*4.0 + 2.0)/lightsSize);
            float lightType = floor(lightPosition.w+.5);
            if(lightType == 0.0) //Directional Light
            {
               lightDir = - lightPosition.xyz;
               calcDirLight(i, normal, ambient, diffuse, specular);
            }
            else if(lightType == 2.0) //Spot Light
            {
               lightDir = lightPosition.xyz - fragPos;
               calcSpotLight(i, normal, ambient, diffuse, specular);
            }
            else //Point Light
            {
               lightDir = lightPosition.xyz - fragPos;
               calcPointLight(i, normal, ambient, diffuse, specular);
            }
         }
      }
      ///Set Ray State to Done
      rDir.w = float(DONE);
      gl_FragData[0] = rDir;

      vec3 intensity = ambient + diffuse;
      gl_FragData[1] = vec4(intensity + specular, 1.0);
   }else
   {
      ///Discard Pixel
      gl_FragData[0] = rDir;
      gl_FragData[1] = vec4(-1, -1, -1, -1);
   }
}

void calcDirLight(float i, vec3 N, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular)
{
   vec3 L = normalize(lightDir);
   vec3 H = normalize(L + normalize(eyeDir));

   float NdotL = max(0.0, dot(N, L));
   if ( NdotL > 0.0 )
   {
      vec3 lightDiffuse = texture1D(lights, (i*4.0)/lightsSize).rgb;
      float att = 1.0; //future work
      float NdotH = max(0.0, dot(N, H));

      diffuse += fragMaterial.diffuse * lightDiffuse * NdotL;

		specular += fragMaterial.specular * lightSpecular.rgb * NdotL * pow(NdotH, fragMaterial.shininess);
	}
}


void calcPointLight(float i, vec3 N, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular)
{
   vec3 L = normalize(lightDir);
   vec3 H = normalize(L + normalize(eyeDir));

   float NdotL = max(0.0, dot(N, L));
   if ( NdotL > 0.0 )
   {
      vec3 lightDiffuse = texture1D(lights, (i*4.0)/lightsSize).rgb;
      float att = 1.0; //future work
      float NdotH = max(0.0, dot(N, H));

      diffuse += fragMaterial.diffuse * lightDiffuse * NdotL;

		specular += fragMaterial.specular * lightSpecular.rgb * NdotL * pow(NdotH, fragMaterial.shininess);
	}
}


void calcSpotLight(float i, vec3 N, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular)
{
   vec3 L = normalize(lightDir);
   vec3 H = normalize(L + normalize(eyeDir));

   float NdotL = max(0.0, dot(N, L));
   if ( NdotL > 0.0 )
   {
      vec4 lightSpotInfo = texture1D(lights, (i*4.0 + 3.0)/lightsSize);
      vec3 lightSpotDir = lightSpotInfo.rgb;
      float lightSpotAngle = lightSpotInfo.a;

      float spotEffect = dot(normalize(lightSpotDir),-L);
      if (spotEffect > lightSpotAngle)
      {
         vec4 lightDiffuse = texture1D(lights, (i*4.0)/lightsSize);
         float att = 1.0; //future work
         float NdotH = max(0.0, dot(N, H));

         spotEffect = pow(spotEffect, lightDiffuse.a);
         diffuse += fragMaterial.diffuse * lightDiffuse.rgb * NdotL * spotEffect;

         specular += fragMaterial.specular * lightSpecular.rgb * NdotL * pow(NdotH, fragMaterial.shininess)*spotEffect;
      }
	}
}
