//varying float ft;
//
//uniform float outWidth;
//uniform float outHeight;
//uniform float n1, n2;

//uniform gl_LightSourceParameters gl_LightSource[gl_MaxLights];

//vec2 calc2DIndex(float index1D, float numColumns)
//{
  //return vec2(index1D/numColumns, mod(index1D, numColumns));
//}
//vec4 calcProjectionPositons(vec2 index2D, float outWidth, float outHeight)
//{
  //return vec4(((index2D.t+.5)/outWidth)*2. - 1.,
             //1. + ((index2D.s-.5)/outHeight)*2.,
             //0., 1.);
//}
void main()
{

  
  //vec2 index2D = calc2DIndex(gl_MultiTexCoord0.s, outWidth);
  //gl_Position = calcProjectionPositons(index2D, outWidth, outHeight);
  
  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_Position = ftransform();
}


