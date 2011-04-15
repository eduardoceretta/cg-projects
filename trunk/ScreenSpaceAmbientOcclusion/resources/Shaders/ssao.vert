varying vec3 lightDir;


void main()
{
  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_Position = ftransform();
  lightDir = gl_NormalMatrix*vec3(1,1,1);
}
