/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Sep 2011
 *
 *  Get the eye nearest fragment
 */
/**
 * Projection Parameters
 */
varying vec4 color;
varying vec3 eyePos;
varying vec3 normal;

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
  float dist = length(eyePos);
 
  
  //OutPut
  gl_FragData[0] = vec4(normal, dist);
}
