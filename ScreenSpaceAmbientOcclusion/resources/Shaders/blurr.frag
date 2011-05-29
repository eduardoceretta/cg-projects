uniform sampler2D inputTex;
uniform float pixelmask_size;
uniform float screenWidth;
uniform float screenHeight;

uniform float offsets_size;
uniform float intensity;

//uniform sampler1D sampleTex;
//uniform float samplerSize;

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
  vec4 input = texture2D(inputTex,  gl_TexCoord[0].st);

	if(input.a < 0.0)
	{
		gl_FragData[0] = vec4(.8, .8, 1.0, -1.0);
		return;
		//discard;
	}
  
  
  float dx = 1.0/screenWidth;
  float dy = 1.0/screenHeight;

  
  int size = int(floor(pixelmask_size));
  float kernel_size = float(size)*2. + 1.;

  float weight = intensity/(kernel_size*kernel_size);
  vec4 sum = BLACK;
  int ssize = size*int(offsets_size);
  for(int i=-ssize; i < ssize + 1; i+=int(offsets_size))
    for(int j = -ssize; j < ssize + 1; j+=int(offsets_size))
    {
      vec2 inc = vec2(float(i)*dx, float(j)*dy);
      vec4 neigh = texture2D(inputTex,  gl_TexCoord[0].st + inc); 
      sum += neigh * weight;
    }
      
      
  //for(int k = 0; k < size ; ++k)
  //{
    //float coord = (float(k) + .5) / samplerSize;
    //vec2 sample = texture1D(sampleTex, coord).xy;
    //float i = floor(sample.x + .5);
    //float j = floor(sample.y + .5);
    //
    //vec2 inc = vec2(i*dx, j*dy);
    //vec4 neigh = texture2D(inputTex,  gl_TexCoord[0].st + inc);  
  //}    
  
  
  gl_FragData[0] = sum;
}
