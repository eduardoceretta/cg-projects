varying vec4 pos;
varying vec3 normal;
varying vec3 lightDir;


void main()
{
   gl_TexCoord[0] = gl_MultiTexCoord0;
   gl_Position = ftransform();

   pos = gl_ModelViewMatrix*gl_Vertex;
   normal = gl_NormalMatrix*gl_Normal;
   lightDir = gl_NormalMatrix*vec3(1,1,1);
//   normal = gl_Normal;
}
