/**
 *	Eduardo Ceretta Dalla Favera
 *  eduardo.ceretta@gmail.com
 *  Mar 2011
 *
 *  Does a deffered shading and a depth peeling of the scene.
 *  Peels the geometry and writes the render targets.
 */

/**
 * Depth of the scene.
 * Is compared with the current depth in the peeling process
 */
uniform sampler2D dephtTex;   

varying vec4 pos;             /**< Eye space point position */
varying vec3 normal;          /**< Eye space point normal */

uniform float screenWidth; 
uniform float screenHeight;


void main()
{
   gl_TexCoord[0] = gl_MultiTexCoord0;
   gl_Position = ftransform();

   pos = gl_ModelViewMatrix*gl_Vertex;
   normal = gl_NormalMatrix*gl_Normal;
}
