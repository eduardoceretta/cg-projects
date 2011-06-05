uniform sampler2D colorTex;
uniform sampler2D ssaoTex;


#define PI 3.14159265

#define RED vec4(1,0,0,1)
#define GREEN vec4(0,1,0,1)
#define BLUE vec4(0,0,1,1)
#define CYAN vec4(0,1,1,1)
#define PINK vec4(1,0,1,1)
#define YELLOW vec4(1,1,0,1)
#define WHITE vec4(1,1,1,1)
#define BLACK vec4(0,0,0,1)
#define ORANGE vec4(1.,.5, 0., 1.)

void main()
{
  vec4 color = texture2D(colorTex,  gl_TexCoord[0].st);
	
  vec4 ssao = texture2D(ssaoTex,  gl_TexCoord[0].st);
  
  if(ssao.a < 0.0 || color.a < 0.0)
	{
		gl_FragData[0] = vec4(.8, .8, 1.0, -1.0);
		return;
	}

  gl_FragData[0] = color*ssao;
  ////gl_FragData[0] = RED;
  //gl_FragData[0] = color*ssao;
}
