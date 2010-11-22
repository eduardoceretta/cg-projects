varying vec3 normalR;
varying vec4 color;

void main()
{
	float ndotl = dot(normalize(normalR), normalize(gl_LightSource[0].position.xyz));
	vec4 specular = vec4(0,0,0,0);
	if(ndotl > 0)
	{
		vec3 half = normalize(gl_LightSource[0].halfVector.xyz);
		float spec =  max(dot(normalize(normalR), half),0.0);
		specular = vec4(1,1,1,1) * pow(spec, 51.);
	}

	gl_FragData[0] = color + specular;
}