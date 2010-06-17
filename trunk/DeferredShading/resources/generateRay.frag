uniform vec3 eyeDir;
uniform vec3 eyePos;
uniform vec3 eyeRight;
uniform float nearPlane;
uniform vec3 eyeUp;
uniform vec3 bbMin;
uniform vec3 bbMax;
uniform vec3 nearPlaneSize;
uniform vec3 screenSize;
uniform vec3 gridVoxelSize;
uniform vec3 gridSize;

//Ray states (stored on rayDir.a)
#define INACTIVE 0.0
#define ACTIVE_TRAVERSE 1.0
#define ACTIVE_INTERSECT 2.0
#define OVERFLOW 3.0

//Ray voxel (stored on rayPos.a)

vec3 getRay(vec2 screenPos){

	vec3 u = ((screenPos.x)) * normalize(eyeRight) * (nearPlaneSize.x);
	vec3 v = ((screenPos.y)) * normalize(eyeUp) * (nearPlaneSize.y);
	vec3 point = eyePos + nearPlane * normalize(eyeDir) + u + v;
	point = point - (nearPlaneSize.x/2.0) * normalize(eyeRight) - (nearPlaneSize.y/2.0) * normalize(eyeUp);

	/*
	vec3 u = (screenPos.x/(screenSize.x)) * eyeRight;
	vec3 v = (screenPos.y/(screenSize.y)) * eyeUp;
	vec3 point = u + v;
	*/

	return normalize(point - eyePos);

}

bool insideGrid(vec3 position){

	if(position.x >= bbMin.x && position.y >= bbMin.y && position.z >= bbMin.z
		&& position.x <= bbMax.x && position.y <= bbMax.y && position.z <= bbMax.z)
		return true;
	else
		return false;

}

bool hitGrid(vec3 position, vec3 direction, out float intersectionMin, out float intersectionMax){

	//Woo's method for ray/AABB collision test

	intersectionMin = 0.0;
	intersectionMax = 0.0;
	float auxMin = 0.0;
	float auxMax = 0.0;
	bool hit = true;

	if(direction.x >= 0.0){
		intersectionMin = (bbMin.x - position.x) / direction.x;
		intersectionMax = (bbMax.x - position.x) / direction.x;
	}
	else{
		intersectionMin = (bbMax.x - position.x) / direction.x;
		intersectionMax = (bbMin.x - position.x) / direction.x;
	}


	if(direction.y >= 0.0){
		auxMin = (bbMin.y - position.y) / direction.y;
		auxMax = (bbMax.y - position.y) / direction.y;
	}
	else{
		auxMin = (bbMax.y - position.y) / direction.y;
		auxMax = (bbMin.y - position.y) / direction.y;
	}

	if(intersectionMin > auxMax || auxMin > intersectionMax) hit = false;

	if(auxMin > intersectionMin) intersectionMin = auxMin;
	if(auxMax < intersectionMax) intersectionMax = auxMax;

	if(direction.z >= 0.0){
		auxMin = (bbMin.z - position.z) / direction.z;
		auxMax = (bbMax.z - position.z) / direction.z;
	}
	else{
		auxMin = (bbMax.z - position.z) / direction.z;
		auxMax = (bbMin.z - position.z) / direction.z;
	}

	if(intersectionMin > auxMax || auxMin > intersectionMax) hit = false;

	if(auxMin > intersectionMin) intersectionMin = auxMin;
	if(auxMax < intersectionMax) intersectionMax = auxMax;

	return hit;
}

vec3 findVoxel(vec3 rayPos){

	vec3 index = vec3(-1.0, -1.0, -1.0);

	index = (rayPos - bbMin) / gridVoxelSize;
	index.x = int(rayPos.x);
	index.y = int(rayPos.y);
	index.z = int(rayPos.z);

	return index;

}

float findVoxelLinearArray(vec3 voxelIndex){

	return voxelIndex.x*gridSize.y*gridSize.z + voxelIndex.y*gridSize.z + voxelIndex.z;

}

vec3 findVoxelPosition(vec3 voxelIndex){
	return voxelIndex * gridVoxelSize + bbMin;
}

vec3 findIntersectionOut(vec3 rayPos, vec3 rayDir, vec3 voxelIndex){

	vec3 intersectionOut = vec3(0.0, 0.0, 0.0);

	if(rayDir.x < 0.0)
		intersectionOut.x = (findVoxelPosition(voxelIndex).x - rayPos.x) / rayDir.x;
	else if(rayDir.x > 0.0)
		intersectionOut.x = (findVoxelPosition(voxelIndex + vec3(1, 0, 0)).x - rayPos.x) / rayDir.x;

	if(rayDir.y < 0.0)
		intersectionOut.y = (findVoxelPosition(voxelIndex).y - rayPos.y) / rayDir.y;
	else if(rayDir.y > 0.0)
		intersectionOut.y = (findVoxelPosition(voxelIndex + vec3(0, 1, 0)).y - rayPos.y) / rayDir.y;

	if(rayDir.z < 0.0)
		intersectionOut.z = (findVoxelPosition(voxelIndex).z - rayPos.z) / rayDir.z;
	else if(rayDir.z > 0.0)
		intersectionOut.z = (findVoxelPosition(voxelIndex + vec3(0, 0, 1)).z - rayPos.z) / rayDir.z;

	return intersectionOut;


}

void main(){

	vec4 rayDir = vec4(getRay(gl_TexCoord[0].xy), 1.0);
	vec4 rayPos = vec4(eyePos, -1.0);
	vec3 voxelIndex = vec3(0, 0, 0);
	vec3 posIntersectionOut = vec3(0,0,0);


	float intersectionMin = 0.0;
	float intersectionMax = 0.0;
	//Sets the ray state
	//The camera is inside the grid
	float breakpoint = 0.0;

	if(insideGrid(rayPos.xyz)){
		rayDir.a = ACTIVE_TRAVERSE;
		rayPos = rayPos;
	}
	else{
		//Check if the ray hits the grid
		if(hitGrid(rayPos.xyz, rayDir.xyz, intersectionMin, intersectionMax)){
			rayDir.w = ACTIVE_TRAVERSE;
			rayPos.xyz = rayPos.xyz + intersectionMin * rayDir.xyz;
			voxelIndex = findVoxel(rayPos.xyz);
			rayPos.w = findVoxelLinearArray(voxelIndex.xyz);
			posIntersectionOut = findIntersectionOut(rayPos.xyz, rayDir.xyz, voxelIndex.xyz);

			breakpoint = 1.0;
		}
		else{
			rayDir.a = INACTIVE;
			breakpoint = -1.0;
		}
	}

	//gl_FragData[0] = vec4(rayDir.xyz, 0.5);
//	gl_FragData[0] = vec4(vec3(breakpoint), 0.5);
//	gl_FragData[0] = vec4(normalize(rayPos.xyz + intersectionMax * rayDir.xyz;), 0.5);
	//gl_FragData[0] = vec4(vec3(rayPos.w/(gridSize.x * gridSize.y * gridSize.z)), 0.5);
	//gl_FragData[0] = vec4(normalize(rayPos.xyz), 0.5);
	/**/
	gl_FragData[0] = rayPos;
	gl_FragData[1] = rayDir;
	gl_FragData[2] = vec4(intersectionMin);
	gl_FragData[3] = vec4(posIntersectionOut, 1.0);

}
