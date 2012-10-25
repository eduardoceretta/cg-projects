/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2012
 *
 *  Does Pixel Light Calculation with opengl lights in deferred shading.
 */
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


#define RED vec4(.8,0,0,1)
#define GREEN vec4(0,.8,0,1)
#define BLUE vec4(0,0,.8,1)
#define CYAN vec4(0,.8,.8,1)
#define PINK vec4(.8,0,.8,1)
#define YELLOW vec4(.8,.8,0,1)
#define WHITE vec4(1,1,1,1)
#define BLACK vec4(0,0,0,1)
#define ORANGE vec4(1.,.5, 0., 1.)

uniform sampler2D normalTex;
uniform sampler2D positionTex;
uniform sampler2D ambientTex;

uniform int ambientEnabled;
uniform int diffuseEnabled;
uniform vec3 ambientColor;
uniform vec3 lightPosition;
uniform vec3 lightDiffColor;

uniform mat4 modelView;

void main()
{
  vec4 lightEyePosition4 = modelView*vec4(lightPosition, 1);
  vec3 lightEyePosition = lightEyePosition4.xyz/lightEyePosition4.w;
  vec4 lightDiffuseColor = vec4(lightDiffColor, 1.0);
  float att = 1.0;

  vec2 fragCoord = gl_TexCoord[0].st;//gl_FragCoord.xy/vec2(screenWidth, screenHeight);
  
  vec4 normalData = texture2D(normalTex, fragCoord);
  vec3 normal = normalize(normalData.xyz);
  float depth = normalData.a;

  if(depth < 0.0)
    discard;
    
  /**
	 * TEST NORMAL
	 *  Print the Normal's Color.
	 *  Result must be Blue for normals facing the screen
   * /
    vec3 nn = (normal.xyz)*.5 + .5;
    //vec3 nn = (normal.xyz);
    gl_FragData[0] = vec4(nn, 1.);
    return;
  /**/
    
  vec3 eyePosition = texture2D(positionTex, fragCoord).xyz;
  
  float ambientValue = 0.0;
  if(ambientEnabled != 0)
    ambientValue = texture2D(ambientTex, fragCoord).r;

  vec4 ambient;
  
  vec4 diffuse = BLACK;
  //vec4 specular = BLACK;
  if(diffuseEnabled != 0)
  {
    vec3 l = vec3(lightEyePosition.xyz - eyePosition);
    vec3 l_norm = normalize(l);
    float cosNorLig = max(dot(normal, l_norm), 0.0);
    if (cosNorLig > 0.0)
    {
      diffuse += gl_FrontMaterial.diffuse * lightDiffuseColor*cosNorLig*att;

      //vec3 eye = -normalize(eyePosition).xyz;
      //vec3 halfV = normalize(l_norm + eye);
      //float cosNorHv= max(dot(normal, halfV),0.0);
      //specular += gl_FrontMaterial.specular * gl_LightSource[i].specular * pow(cosNorHv,gl_FrontMaterial.shininess)*att;
      //specular += gl_FrontMaterial.specular * WHITE * pow(cosNorHv, gl_FrontMaterial.shininess)*att;
    }
    ambient = vec4(ambientColor, 1.0);
  }else ambient = WHITE;
    
  if(ambientEnabled != 0)
    ambient *= ambientValue;
  
  gl_FragColor = clamp(diffuse + ambient, 0.0, 1.0);
  
}