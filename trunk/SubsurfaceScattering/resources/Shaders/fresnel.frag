/*
struct 
{
  //int index;
  float indexF;
  Vector3 vertex;
  Vector3 normal;

  float area; // One third of the triangle's area that contains this vertex

  //int numNeighbors;
  float numNeighborsF;
  

  //int neighborFileIndex;
  float neighborFileIndexF;

  //int RFileIndex;
  float RFileIndexF;
  Vector3 Q; // One Directional light g_lightDirection
  float q; // One Directional light g_lightDirection
}VertexInfo;
*///

uniform float outWidth;
uniform float outHeight;
uniform float numVertices;
uniform float sizeofVertexInfo; 
uniform float n1;
uniform float n2;
uniform sampler2D vertexInfoTex;

vec2 index1Dto2D(float index, float width, float size)
{
  float height = float(trunc(size/width))+1.0;
  vec2 r = vec2(float(mod(index,width)), float(trunc(index/width)));
  r.x = (r.x+.5)/width;
  r.y = (r.y+.5)/height;

  return r;
}

void main()
{
	float fragIndex = floor((gl_TexCoord[0].t*outHeight - .5)*outWidth + (gl_TexCoord[0].s*outWidth - .5) + .5);
	
	if(fragIndex > numVertices)
	{
		gl_FragData[0] = vec4(1,0,0,1);
		return;
	}
	
    vec2 coord2D = index1Dto2D(fragIndex, outWidth, outHeight*outWidth);

    //float triangleIndex = floor(texture2D(grid, coord2D).a + .5);


  //vec4 pos = gl_ModelViewMatrix*gl_Vertex;
  //vec3 normal = gl_NormalMatrix*gl_Normal;
  
  //ERRADO TEM QUE SER O Wo

  //vec4 wi = gl_LightSource[0].position - pos;
//
  //float cosOi = dot(wi.xyz, normal);
  //float sinOi = sin(acos(cosOi));
  //float n1_n2 = n1/n2;
//
  //float rs =  pow(
              //(n1 * cosOi - n2 * sqrt(1. - pow((n1_n2 * sinOi), 2.))) /
              //(n1 * cosOi + n2 * sqrt(1. - pow((n1_n2 * sinOi), 2.)))
              //, 2.);
//
  //float rp =  pow(
              //(n1 * sqrt(1. - pow((n1_n2 * sinOi), 2.)) - n2 * cosOi) /
              //(n1 * sqrt(1. - pow((n1_n2 * sinOi), 2.)) + n2 * cosOi)
              //, 2.);
//
  //float fr = (rs + rp) / 2.;
//
  //ft = 1. - fr;

   //gl_FragData[0] = vec4(ft,0,0,1);
//   gl_FragData[1] = normal;
//   gl_FragData[1] = vec4(normalize(normal), 1.0);
//   gl_FragData[2] = gl_FrontMaterial.diffuse;
//   gl_FragData[3] = vec4(vec3(gl_FrontMaterial.specular.rgb), gl_FrontMaterial.shininess);
}

