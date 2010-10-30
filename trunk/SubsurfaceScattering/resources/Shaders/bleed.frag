varying vec3 vertex;
varying vec4 color;

void main()
{
	//gl_FragColor = vec4(1,0,0,1);
	//gl_FragData[0] = vec4(1,0,0,1);
	gl_FragData[0] = color;
	//gl_FragData[0] = vec4(gl_TexCoord[0].s, -1,-1,-1);
	//gl_FragData[0] = vec4(gl_TexCoord[0].s, vertex.x, vertex.y, vertex.z);
	//gl_FragData[0] = vec4(gl_TexCoord[0].s, vertex.x, vertex.y, vertex.z);
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
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

//uniform float outWidth;
//uniform float outHeight;
//uniform float numVertices;
//uniform float sizeofVertexInfo; 
//uniform float n1;
//uniform float n2;
//uniform sampler2D vertexInfoTex;
//
//vec2 index1Dto2D(float index, float width, float size)
//{
  //float height = float(trunc(size/width))+1.0;
  //vec2 r = vec2(float(mod(index,width)), float(trunc(index/width)));
  //r.x = (r.x+.5)/width;
  //r.y = (r.y+.5)/height;
//
  //return r;
//}

	
	
	
	/*
	float fragIndex = floor((gl_TexCoord[0].t*outHeight - .5)*outWidth + (gl_TexCoord[0].s*outWidth - .5) + .5);
	
	if(fragIndex > numVertices)
	{
		gl_FragData[0] = vec4(-1,-1,-1,-1);
		return;
	}
	
	float texStep = (sizeofVertexInfo/4.0)/4.0;
	
	vec2 d = vec2(1./outWidth, 1./outHeight);
	
	vec2 coord2D = (gl_TexCoord[0] - d/2.)*texStep + d/2.;

	while(coord2D.s > 1.0)
		coord2D = vec2(coord2D.s - 1.0, coord2D.t + d.t);

    vec4 tmp = texture2D(vertexInfoTex, coord2D);

    float index = floor(tmp.r + .5);
    vec3 vertex = tmp.gba;
    
    
    coord2D = coord2D + vec2(d.s, 0);
    while(coord2D.s > 1.0)
		coord2D = vec2(coord2D.s - 1.0, coord2D.t + d.t);
    tmp = texture2D(vertexInfoTex, coord2D);
    
    vec3 normal = tmp.rgb;
	float area = tmp.a;

    coord2D = coord2D + vec2(d.s, 0);
    while(coord2D.s > 1.0)
		coord2D = vec2(coord2D.s - 1.0, coord2D.t + d.t);
    tmp = texture2D(vertexInfoTex, coord2D);

	float numNeighbors = floor(tmp.r + .5);
	float neighborFileIndex = floor(tmp.g + .5);
	float RFileIndex = floor(tmp.b + .5);
	vec3 Q;
	Q.x = tmp.a;
	
	coord2D = coord2D + vec2(d.s, 0);
    while(coord2D.s > 1.0)
		coord2D = vec2(coord2D.s - 1.0, coord2D.t + d.t);
    tmp = texture2D(vertexInfoTex, coord2D);
    
    Q.yz = tmp.rg;
	float q = tmp.b;


	vec3 wi = normalize(gl_LightSource[0].position.xyz - vertex);

	float cosOi = dot(wi, normal);
	if(cosOi < 0)
	{
		gl_FragData[0] = vec4(0, 0, index, -2);	
		return ;
	}
	float sinOi = sin(acos(cosOi));
	float n1_n2 = n1/n2;

	float rs =  pow(
			  (n1 * cosOi - n2 * sqrt(1. - pow((n1_n2 * sinOi), 2.))) /
			  (n1 * cosOi + n2 * sqrt(1. - pow((n1_n2 * sinOi), 2.)))
			  , 2.);

	float rp =  pow(
			  (n1 * sqrt(1. - pow((n1_n2 * sinOi), 2.)) - n2 * cosOi) /
			  (n1 * sqrt(1. - pow((n1_n2 * sinOi), 2.)) + n2 * cosOi)
			  , 2.);

	float fr = (rs + rp) / 2.;

	float ft = 1. - fr;

   gl_FragData[0] = vec4(ft, fr, index, -1);
//   gl_FragData[1] = normal;
//   gl_FragData[1] = vec4(normalize(normal), 1.0);
//   gl_FragData[2] = gl_FrontMaterial.diffuse;
//   gl_FragData[3] = vec4(vec3(gl_FrontMaterial.specular.rgb), gl_FrontMaterial.shininess);
}

*/