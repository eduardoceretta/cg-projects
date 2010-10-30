uniform float outWidth;
uniform float outHeight;

varying vec3 vertex;
varying vec4 color;

uniform float n1;
uniform float n2;


//
//vec2 calc2DIndex(float index1D, float numColumns)
//{
  //return vec2(index1D/numColumns, mod(index1D, numColumns));
//}
//vec4 calcProjectionPositons(vec2 index2D, float outWidth, float outHeight)
//{
  //return vec4(((index2D.t+.5)/outWidth)*2. - 1.,
			//((index2D.s+.5)/outWidth)*2. - 1.,
             ////1. + ((index2D.s-.5)/outHeight)*2.,
             //0., 1.);
//}
float calcFresnelTransmissionTerm(vec3 normal, vec3 wi, float n1, float n2);

void main()
{

  //gl_TexCoord[0] = gl_MultiTexCoord0;
  //vec2 index2D = calc2DIndex(gl_MultiTexCoord0.s, outWidth);
  //vertex = gl_Vertex;
  //
  ////gl_PointSize = 30.;
  //gl_Position = calcProjectionPositons(index2D, outWidth, outHeight);
  
  vec3 wi = normalize(vec3(0,100,0) - vec3(gl_Vertex));
  float ft = calcFresnelTransmissionTerm(gl_Normal, wi, n1, n2);
  //if(ft>1.)
	//color = vec4(1,0,0,1);
  //else color = vec4(0,0,1,1);
  
  color = vec4(ft,ft,ft,1);
  gl_Position = ftransform();
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