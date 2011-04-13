varying vec4 pos;
varying vec3 normal;
uniform sampler2D dephtTex;
uniform float screenWidth;
uniform float screenHeight;

void main()
{
  vec2 texCoord = (gl_FragCoord.xy/* - vec2(.5,.5)*/)/vec2(screenWidth, screenHeight);
  float depth = texture2D(dephtTex, texCoord).a;

	if(depth < 0. || gl_FragCoord.z <= depth)
	{
	  //gl_FragData[0] = vec4(1,1,0,1);
	  //return;
	  discard;
	}

	//gl_FragData[0] = pos;
  gl_FragData[0] = max(dot(normal, normalize(vec3(1,1,1))), 0.0) * vec4(1,0,0,1);
	gl_FragData[1] = vec4(normalize(normal), gl_FragCoord.z);
	//gl_FragData[2] = gl_FrontMaterial.diffuse;
	//gl_FragData[3] = vec4(vec3(gl_FrontMaterial.specular.rgb), gl_FrontMaterial.shininess);
}

