uniform float outWidth;
uniform float outHeight;

varying vec3 lightDir;
varying vec3 vertex;
varying vec3 normalR;
varying vec4 color;

uniform float n1;
uniform float n2;

uniform float numVertices;
uniform float sizeofVertexInfo; 
uniform sampler2D vertexInfoTex;

uniform float neighborSize;
float neighborHeight = neighborSize/outWidth;
uniform sampler2D neighborTex;

uniform float neighborRSize;
float neighborRHeight = neighborRSize/outWidth;
uniform sampler2D neighborRTex;

uniform float fresnelSize;
float fresnelHeight = fresnelSize/outWidth;
uniform sampler2D fresnelTex;



vec2 calc2DIndex(float index1D, float numColumns)
{
  return vec2(index1D/numColumns, mod(index1D, numColumns));
}

vec2 calc2DIndex(float index1D, float texWidth, float texHeight)
{
  return vec2(mod(index1D, texWidth)/texWidth, trunc(index1D/texWidth)/texHeight) + vec2(1./texWidth, 1./texHeight)/2.;		
}


vec4 calcProjectionPositons(vec2 index2D, float outWidth, float outHeight)
{
  return vec4(((index2D.t)/outWidth)*2. - 1.,
			((index2D.s)/outHeight)*2. - 1.,
             //1. + ((index2D.s-.5)/outHeight)*2.,
             0., 1.);
             
 //return vec4(((index2D.t+.5)/outWidth)*2. - 1.,
			//((index2D.s+.5)/outHeight)*2. - 1.,
             ////1. + ((index2D.s-.5)/outHeight)*2.,
             //0., 1.);
}

  //gl_TexCoord[0] = gl_MultiTexCoord0;
  //vec2 index2D = calc2DIndex(gl_MultiTexCoord0.s, outWidth);
  //vertex = gl_Vertex;
  
  //gl_PointSize = 30.;
  //gl_Position = calcProjectionPositons(index2D, outWidth, outHeight);
  
float getNeighborIndexTerm(float index);
vec4 getFresenelInfo(float index);
float getRdTerm(float index);
float calcFresnelTransmissionTerm(vec3 normal, vec3 wi, float n1, float n2);
float PI = 3.1415926535897932384626433;
void main()
{
    gl_TexCoord[0] = gl_MultiTexCoord0;
  	float texStep = (sizeofVertexInfo/4.0)/4.0;
	vec2 d = vec2(1./outWidth, 1./outHeight);
	
	vec2 coord2D = (gl_TexCoord[0].st )*texStep + d/2.;
	while(coord2D.s >= 1.0)
		coord2D = vec2(coord2D.s - 1.0 , coord2D.t + d.t );
	vec4 tmp = texture2D(vertexInfoTex, coord2D);
	float index = floor(tmp.r + .5);
	coord2D = coord2D + vec2(d.s, 0);
	while(coord2D.s >= 1.0)
		coord2D = vec2(coord2D.s - 1.0, coord2D.t + d.t);
	tmp = texture2D(vertexInfoTex, coord2D);
	vec3 normal = tmp.rgb;
	float area = tmp.a;
	coord2D = coord2D + vec2(d.s, 0);
	while(coord2D.s >= 1.0)
		coord2D = vec2(coord2D.s - 1.0, coord2D.t + d.t);
	tmp = texture2D(vertexInfoTex, coord2D);
	float numNeighbors = floor(tmp.r + .5);
	float neighborFileIndex = floor(tmp.g + .5);
	float RFileIndex = floor(tmp.b + .5);
	vec3 Q;
	Q.x = tmp.a;

	coord2D = coord2D + vec2(d.s, 0);
	while(coord2D.s >= 1.0)
		coord2D = vec2(coord2D.s - 1.0, coord2D.t + d.t);
	tmp = texture2D(vertexInfoTex, coord2D);

	Q.yz = tmp.rg;
	float qR = tmp.b;

	vec3 wi = normalize(vec3(0,1,0));

	//float q = .0;
	//for(float i = 0.0; i< numNeighbors; ++i)
	//{
		//float neighborIndex = neighborFileIndex + i;
		//float neighborRIndex = RFileIndex + i;
		//vec4 tmp = getFresenelInfo(getNeighborIndexTerm(neighborIndex));
		//float ftArea = tmp.r;
		//vec3 nNormal = tmp.gba;
		//float cosOi = dot(nNormal, wi);
		//if(cosOi > 0.)
		//{
			//float Rd = getRdTerm(neighborRIndex);
			//q += ftArea * (Rd / PI) * cosOi;
		//}
	//}
	

	
	
	
	
	
	
	
		//vec3 wi = normalize(vec3(0,100,0) - vec3(gl_Vertex));


	float vertexIndex =  (gl_TexCoord[0].s*outWidth) + (gl_TexCoord[0].t*outHeight)*outWidth;
	vec2 index2D = calc2DIndex(vertexIndex, outWidth) ;
	gl_Position = calcProjectionPositons(index2D, outWidth, outHeight);
	color = vec4(index, numNeighbors, neighborFileIndex, -2);
	//color = vec4(Q,-1);
	return;
	//
	
	/*
	gl_Position = ftransform();

	
	float ft = calcFresnelTransmissionTerm(gl_Normal, wi, n1, n2);
	
	float ft2 = calcFresnelTransmissionTerm(normalize(gl_Normal*gl_NormalMatrix), normalize(-gl_Position), n1, n2);
	
	vec4 dif = ft*vec4(.5,0.,0.,1.);
	
	vec4 scatter = qq*vec4(.5,0.,0.,1.);
	color = scatter;

	vertex = gl_Position.xyz;
	normalR = gl_Normal*gl_NormalMatrix;
	lightDir = wi*gl_NormalMatrix;
/*
	
  
  
  
  
  vec3 wi = normalize(vec3(0,100,0) - vec3(gl_Vertex));
  float ft = calcFresnelTransmissionTerm(gl_Normal, wi, n1, n2);
  vec4 dif = ft*vec4(.5,0.,0.,1.);
  //color = dif;
  
  float vertexIndex =  gl_TexCoord[0].s*8192 + gl_TexCoord[0].t*2*8192;
 
  if( index < 0.)
	color = vec4(1,0,0,1);
  else
  	color = vec4(0,0,1,1);
  
  
  
  gl_Position = ftransform();
  
  //float ft2 = calcFresnelTransmissionTerm(normalize(gl_Normal), normalize(+vec3(193,0,230)-gl_Vertex.xyz), n1, n2);
  //float ft2 = calcFresnelTransmissionTerm(normalize(gl_Normal*gl_NormalMatrix), normalize(-gl_Position), n1, n2);
 
  //vec4 spec = ft2*vec4(1.,1.,1.,1.);
/**/
}


float calcFresnelTransmissionTerm(vec3 normal, vec3 wi, float n1, float n2)
{	
	float cosOi = dot(wi, normal);
	if(cosOi < 0.)
		return 0.;	

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
	return ft;
}


float getNeighborIndexTerm(float index)
{
	vec2 index2D2 = calc2DIndex(index/4, outWidth, neighborHeight);		
	float p = mod(index, 4.);
	if(p == 0.)		
		return texture2D(neighborTex, index2D2).r;
	if(p == 1.)		
		return texture2D(neighborTex, index2D2).g;
	if(p == 2.)		
		return texture2D(neighborTex, index2D2).b;
	if(p == 3.)		
		return texture2D(neighborTex, index2D2).a;
}

float getRdTerm(float index)
{
	vec2 index2D2 = calc2DIndex(index/4., outWidth, neighborRHeight);
	float p = mod(index, 4.);
	if(p == 0.)		
		return texture2D(neighborRTex, index2D2).r;
	if(p == 1.)		
		return texture2D(neighborRTex, index2D2).g;
	if(p == 2.)		
		return texture2D(neighborRTex, index2D2).b;
	if(p == 3.)		
		return texture2D(neighborRTex, index2D2).a;
}

vec4 getFresenelInfo(float index)
{
	vec2 index2D2 = calc2DIndex(index, outWidth, fresnelHeight);		
	return texture2D(fresnelTex, index2D2);
}