varying vec4 pos;
varying vec3 normal;
varying vec4 lightPos;


void main()
{
   gl_TexCoord[0] = gl_MultiTexCoord0;
   gl_Position = ftransform();

   pos = gl_ModelViewMatrix*gl_Vertex;
   normal = gl_NormalMatrix*gl_Normal;
   lightPos = gl_ModelViewMatrix*vec4(0,100,0,1);
//   normal = gl_Normal;
}
