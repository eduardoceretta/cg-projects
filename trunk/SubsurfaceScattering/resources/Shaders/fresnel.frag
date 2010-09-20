varying float ft;

void main()
{
   gl_FragData[0] = vec4(ft,0,0,1);
//   gl_FragData[1] = normal;
//   gl_FragData[1] = vec4(normalize(normal), 1.0);
//   gl_FragData[2] = gl_FrontMaterial.diffuse;
//   gl_FragData[3] = vec4(vec3(gl_FrontMaterial.specular.rgb), gl_FrontMaterial.shininess);
}

