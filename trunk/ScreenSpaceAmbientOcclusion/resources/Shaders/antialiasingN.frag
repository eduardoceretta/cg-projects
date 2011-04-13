uniform sampler2D colorTex;
uniform sampler2D factorTex;



void main()
{
//  gl_FragData[0] = texture2D(colorTex, gl_TexCoord[0].st);
//  gl_FragData[0] = vec4(1,0,0,1);
  vec3 factorVec = texture2D(factorTex, gl_TexCoord[0].st).xyz;

  float dx = dFdx(gl_TexCoord[0].s);
  float dy = dFdx(gl_TexCoord[0].t);
  vec2 pixelSize = vec2(dx,dy);

  vec2 window[8];
  window[0] = vec2(-1,1);
  window[1] = vec2(1,-1);
  window[2] = vec2(1,1);
  window[3] = vec2(-1,-1);
  window[4] = vec2(-1,0);
  window[5] = vec2(0,1);
  window[6] = vec2(0,-1);
  window[7] = vec2(1,0);
//  window[8] = vec2(-2,0);
//  window[9] = vec2(2,0);
//  window[10] = vec2(0,-2);
//  window[11] = vec2(0,2);


//  vec4 color = vec4(vec3(factor), 1.0);
  vec4 color;
  float factor = factorVec.r;

  if(factor > 0.05)
  {
    color = vec4(0.,0.,0.,0.);
    for(int i=0;i<8;++i)
      color += texture2D(colorTex, gl_TexCoord[0].st + window[i]*pixelSize);
    color += texture2D(colorTex, gl_TexCoord[0].st)*2.0;
    color *= 1.0/10.0;
  } else color = texture2D(colorTex, gl_TexCoord[0].st);


  gl_FragData[0] = color;
  /**/
  //  gl_FragColor = color;
}

