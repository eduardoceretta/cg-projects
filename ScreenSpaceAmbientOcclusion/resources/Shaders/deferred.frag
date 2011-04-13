varying vec4 pos;
varying vec3 normal;
varying vec3 lightDir;

//gl_FragCoord
  //.x - posição absoluta do pixel na tela (meio do pixel) [viewport.x + .5, viewport.x + viewport.width + .5]
  //.y - posição absoluta do pixel na tela (meio do pixel) [viewport.y + .5 , viewport.y + viewport.height+ .5]
  //.z - profundidade do pixel [depth_near, depth_far] (o range pode ser setado por glDepthRange (default 0,1))
  //.w - um sobre a profundidade no espaço do olho vezes menos um (== 1.0/-ze) [0,1 desde que o near seja > 0]

void main()
{
   gl_FragData[0] = pos;
   //gl_FragData[0] = max(dot(normal, normalize(lightDir)), 0.0) * gl_FrontMaterial.diffuse;
   
   //gl_FragData[0] = vec4(gl_FragCoord.x, gl_FragCoord.y, gl_FragCoord.z, gl_FragCoord.w);
   gl_FragData[1] = vec4(normalize(normal), gl_FragCoord.z);
   gl_FragData[2] = gl_FrontMaterial.diffuse;
   gl_FragData[3] = vec4(vec3(gl_FrontMaterial.specular.rgb), gl_FrontMaterial.shininess);
}

