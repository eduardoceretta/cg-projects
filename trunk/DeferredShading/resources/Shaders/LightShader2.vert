varying vec4 color,ambient;

varying vec3 normal,ecPos3;

varying mat4 modelView;
varying mat3 nModelView;

void main()
{
   color = gl_Color;
   normal = gl_NormalMatrix * gl_Normal;
   vec4 ecPos = gl_ModelViewMatrix * gl_Vertex;
   modelView = gl_ModelViewMatrix;
   nModelView = gl_NormalMatrix;
   ecPos3 = vec3(ecPos)/ecPos.w;

   ambient = vec4(0,0,0,1);

   ambient = gl_LightModel.ambient * gl_FrontMaterial.ambient;


   gl_TexCoord[0] = gl_MultiTexCoord0;
//   vec4 texel = texture2D(tex,gl_TexCoord[0].st);
   gl_Position = ftransform();
}



