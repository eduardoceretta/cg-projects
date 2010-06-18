varying vec4 pos;
varying vec3 normal;
varying vec4 lightPos;

void main()
{
   gl_FragData[0] = pos;
   gl_FragData[1] = vec4(normalize(normal), 1.0);
   gl_FragData[2] = gl_FrontMaterial.diffuse;
   gl_FragData[3] = vec4(vec3(gl_FrontMaterial.specular.rgb), gl_FrontMaterial.shininess);
}

