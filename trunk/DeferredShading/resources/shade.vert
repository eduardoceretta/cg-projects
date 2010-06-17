uniform sampler2D rayPos;
uniform sampler2D rayDir;

uniform sampler2D triangleInfo;

uniform sampler1D vertexes;
uniform sampler1D normals;
uniform sampler1D diffuse;
uniform sampler1D especular;
uniform sampler1D lights;


void main()
{
   gl_TexCoord[0] = gl_MultiTexCoord0; //triangleInfo
//   gl_TexCoord[1] = gl_MultiTexCoord1; //vertexes
//   gl_TexCoord[2] = gl_MultiTexCoord2; //normals
//   gl_TexCoord[3] = gl_MultiTexCoord3; //diffuse
//   gl_TexCoord[4] = gl_MultiTexCoord3; //especular
//   gl_TexCoord[5] = gl_MultiTexCoord3; //ambient
//   gl_TexCoord[6] = gl_MultiTexCoord6;
//   gl_TexCoord[7] = gl_MultiTexCoord7;

   gl_Position = ftransform();
}
