uniform sampler2D samplerRayPos;
uniform sampler2D samplerRayDir;
uniform sampler2D samplerGridIntersectionMax;
uniform sampler2D samplerGridIntersectionMin;
uniform sampler1D samplerGrid;

uniform vec3 gridSize;
uniform float gridArraySize;

//Ray states (stored on rayDir.a)
#define INACTIVE 0.0
#define ACTIVE_TRAVERSE 1.0
#define ACTIVE_INTERSECT 2.0
#define OVERFLOW 3.0

//Voxel index (stored on rayPos.a)

float findVoxelLinearArray(vec3 voxelIndex){

	return voxelIndex.x*gridSize.y*gridSize.z + voxelIndex.y*gridSize.z + voxelIndex.z;

}


void main(){

	vec4 rayDir = texture2D(samplerRayDir, gl_TexCoord[0].st);
	vec4 rayPos = texture2D(samplerRayPos, gl_TexCoord[0].st);
	vec4 gridIndex = texture1D(samplerGrid, floor(rayPos.w+.5)/gridArraySize);
	vec4 gridIntersectionMax = texture2D(samplerGridIntersectionMax, gl_TexCoord[0].st);
	vec4 gridIntersectionMin = texture2D(samplerGridIntersectionMin, gl_TexCoord[0].st);

	float rayLength = length(gridIntersectionMax.xyz - gridIntersectionMin.xyz);

//	if(gridIndex.x > gridSize.x || gridIndex.y > gridSize.y || gridIndex.z > gridSize.z)
//   {
//      gl_FragData[0] = vec4(0,1,0,1);
//      return;
//      discard;
//   }

	if(gridIndex.a > 0.0){
		rayDir.a = ACTIVE_INTERSECT;
	}
	else{
		rayDir.a = ACTIVE_TRAVERSE;
	}

	if(rayDir.a == ACTIVE_TRAVERSE){

		vec3 delta = abs(rayLength / rayDir.xyz);
		vec3 step = vec3(1.0, 1.0, 1.0);
		if(rayDir.x < 0.0) step.x = -1.0;
		if(rayDir.y < 0.0) step.y = -1.0;
		if(rayDir.z < 0.0) step.z = -1.0;

		if(rayPos.x < rayPos.y && rayPos.x < rayPos.z){
			gridIndex.x += 1.0;
			rayPos.x += delta.x;
		}
		else if((rayPos.x < rayPos.y && rayPos.x > rayPos.z) || (rayPos.x > rayPos.y && rayPos.y > rayPos.z)){
			gridIndex.z += 1.0;
			rayPos.z += delta.z;
		}
		else if(rayPos.x > rayPos.y && rayPos.y < rayPos.z){
			gridIndex.y += 1.0;
			rayPos.y += delta.y;
		}
	}


	if(gridIndex.a > 0.0){
		rayDir.a = ACTIVE_INTERSECT;
		gl_FragData[2] = vec4(1.0, 0.0, 1.0, .5);
	}
	else{
		rayDir.a = ACTIVE_TRAVERSE;
		gl_FragData[2] = vec4(0.0, 0.0, 1.0, 0.5);
	}
//   gl_FragData[0] = rayPos;
   gl_FragData[0] = vec4(rayPos.xyz, findVoxelLinearArray(gridIndex.xyz));
//   gl_FragData[0] = vec4(rayPos.xyz, 0.8);
	gl_FragData[1] = rayDir;
//	gl_FragData[2] = rayDir;
}
