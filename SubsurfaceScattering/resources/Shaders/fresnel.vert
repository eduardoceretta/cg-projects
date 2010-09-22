varying float ft;

uniform float outWidth;
uniform float outHeight;
uniform float n1, n2;

//uniform gl_LightSourceParameters gl_LightSource[gl_MaxLights];

vec2 calc2DIndex(float index1D, float numColumns)
{
  return vec2(index1D/numColumns, mod(index1D, numColumns));
}
vec4 calcProjectionPositons(vec2 index2D, float outWidth, float outHeight)
{
  return vec4(((index2D.t+.5)/outWidth)*2. - 1.,
             1. + ((index2D.s-.5)/outHeight)*2.,
             0., 1.);
}
void main()
{
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

  vec2 index2D = calc2DIndex(gl_MultiTexCoord0.s, outWidth);
  gl_Position = calcProjectionPositons(index2D, outWidth, outHeight);
}


