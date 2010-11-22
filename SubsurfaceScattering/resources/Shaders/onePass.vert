uniform float outWidth;
uniform float outHeight;

varying vec3 normalR;
varying vec4 color;

uniform float n1;
uniform float n2;

uniform float numVertices;
uniform float sizeofVertexInfo; 
uniform sampler2D vertexInfoTex;


uniform float sphereDivisionsTeta, sphereDivisionsAlpha;
uniform float qSize; 
float qHeight = qSize/outWidth;
uniform sampler2D qTex;



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
             0., 1.);
}

float calcFresnelTransmissionTerm(vec3 normal, vec3 wi, float n1, float n2);
//float interpolateq(float qindex, vec3 wi);
vec4 interpolateq(float qindex, vec3 wi);

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
	
	coord2D = coord2D + vec2(d.s, 0.);
	while(coord2D.s >= 1.0)
		coord2D = vec2(coord2D.s - 1.0, coord2D.t + d.t);
	tmp = texture2D(vertexInfoTex, coord2D);

	//vec3 normal = tmp.rgb;
	//float area = tmp.a;

	coord2D = coord2D + vec2(d.s, 0.);
	while(coord2D.s >= 1.0)
		coord2D = vec2(coord2D.s - 1.0, coord2D.t + d.t);
	tmp = texture2D(vertexInfoTex, coord2D);

	//float numNeighbors = floor(tmp.r + .5);
	//float neighborFileIndex = floor(tmp.g + .5);
	//float RFileIndex = floor(tmp.b + .5);
	//vec3 Q;
	//Q.x = tmp.a;

	coord2D = coord2D + vec2(d.s, 0.);
	while(coord2D.s >= 1.0)
		coord2D = vec2(coord2D.s - 1.0, coord2D.t + d.t);
	tmp = texture2D(vertexInfoTex, coord2D);

	//Q.yz = tmp.rg;
	float q = tmp.b;
	float qindex = tmp.a;
	
	
	vec3 wi = normalize((gl_ModelViewMatrixInverse*gl_LightSource[0].position).xyz);
	vec4 qq = interpolateq(qindex, wi)*500;
	
	
	//float vertexIndex =  (gl_TexCoord[0].s*outWidth) + (gl_TexCoord[0].t*outHeight)*outWidth;
	//vec2 index2D = calc2DIndex(vertexIndex, outWidth);
	//gl_Position = calcProjectionPositons(index2D, outWidth, outHeight);
	//color = qq;
	////color = vec4(1, wi.x, wi.y, wi.z);
	//return;
//
	
	
	gl_Position = ftransform();

	
	float ft = calcFresnelTransmissionTerm(gl_Normal, wi, n1, n2);
	float ft2 = 1-calcFresnelTransmissionTerm(normalize(gl_Normal), 
		normalize(gl_ModelViewMatrixInverse*vec4(0,0,0,1) - gl_ModelViewMatrixInverse*gl_Position).xyz, n1, n2);
	
	vec4 scatter = qq.a*vec4(.5,0.,0.,1.);
	
	color = scatter;
	normalR = gl_NormalMatrix*gl_Normal;
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

vec2 invSphere(vec3 p)
{
	vec2 r;
	r.x = acos(clamp(p.z,-1.,1.));
	r.y = acos(clamp( p.x/sin(r.x), -1., 1.));
	return r;
}

float interpolate(vec2 p, vec2 l11, float fl11, vec2 l12, float fl12, vec2 l21, float fl21, vec2 l22, float fl22)
{
	float i11 = fl11*(l12.x - p.x)*(l21.y - p.y)/((l12.x - l11.x)*(l21.y - l11.y));
	float i12 = fl12*(p.x - l11.x)*(l21.y - p.y)/((l12.x - l11.x)*(l22.y - l12.y));
	float i21 = fl21*(l22.x - p.x)*(p.y - l11.y)/((l22.x - l21.x)*(l21.y - l11.y));
	float i22 = fl22*(p.x - l21.x)*(p.y - l12.y)/((l22.x - l21.x)*(l22.y - l12.y));
	return i11 + i12 + i21 + i22;
}

vec2 sphereDivisions = vec2(radians(180./sphereDivisionsTeta),radians(360./sphereDivisionsAlpha));

float getq(float qindex, vec2 sphereIndex)
{
	float j = degrees(sphereIndex.x)*sphereDivisionsTeta/180. - .5;
	float k = degrees(sphereIndex.y)*sphereDivisionsAlpha/360.;
	float index = qindex + j*sphereDivisionsAlpha + k;
	
	vec2 index2D2 = calc2DIndex(index, outWidth, qHeight);		
	
	return texture2D(qTex, index2D2).a;
} 

vec4 interpolateq(float qindex, vec3 wi)
{
	vec2 p = invSphere(normalize(wi));
	//p = floor(p*10+.5)/10.; // round a little bit
	vec2 l11 = floor(p/sphereDivisions)*sphereDivisions - vec2(sphereDivisions.x/2, 0.0);
	vec2 l12 = l11 + vec2(sphereDivisions.x, 0.0);
	vec2 l21 = l11 + vec2(0.0, sphereDivisions.y);
	vec2 l22 = l11 + sphereDivisions;
	float fl11 = getq(qindex, l11);
	float fl12 = getq(qindex, l12);
	float fl21 = getq(qindex, l21);
	float fl22 = getq(qindex, l22);
	return vec4(interpolate(p, l11, fl11, l12, fl12, l21, fl21, l22, fl22));
	//return vec4(interpolate(p, l11, fl11, l12, fl12, l21, fl21, l22, fl22), degrees(p.x), degrees(l11.x), degrees(l11.y));
	return vec4(degrees(p.x), p.y, -1,-1);
	//return vec4(fl11,fl22,fl21,fl22);
}