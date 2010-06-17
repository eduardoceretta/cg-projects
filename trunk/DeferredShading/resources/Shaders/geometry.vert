varying vec4 pos;
varying vec3 normal;

void main()
{
   gl_TexCoord[0] = gl_MultiTexCoord0;
   gl_Position = ftransform();

   pos = gl_Position;
   normal = gl_NormalMatrix*gl_Normal;
//   normal = gl_Normal;
}
