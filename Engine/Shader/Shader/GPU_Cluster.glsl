#version 310 es
layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

precision mediump float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM;
USE_SCENE_DATA_STORAGE;
USE_CLUSTER_DATA_STORAGE;
USE_FULL_LIGHT_LIST_DATA_STORAGE;
USE_CULL_LIGHT_LIST_DATA_STORAGE;

// Output
// ...

// Descriptor
layout(push_constant) uniform PushConstantParam {
	float tileSizeX;
	float tileSizeY;
	int numDepthTiles;
	int numPointLights;
} Param;

bool Intersection(highp vec3 minAABBPosition, highp vec3 maxAABBPosition, highp vec3 spherePosition, highp float radius)
{
	highp float dis2 = 0.0;

	for (int i = 0; i < 3; i++) {
		if (spherePosition[i] < minAABBPosition[i]) {
			dis2 += (spherePosition[i] - minAABBPosition[i]) * (spherePosition[i] - minAABBPosition[i]);
		}
		if (spherePosition[i] > maxAABBPosition[i]) {
			dis2 += (spherePosition[i] - maxAABBPosition[i]) * (spherePosition[i] - maxAABBPosition[i]);
		}
	}

	return dis2 <= (radius * radius);
}

void main()
{
	highp vec2 tileSize = vec2(Param.tileSizeX, Param.tileSizeY);
	highp int numDepthTiles = Param.numDepthTiles;
	highp int numPointLights = Param.numPointLights;

	highp vec2 minScreenPosition = (vec2(float(gl_GlobalInvocationID.x), float(gl_GlobalInvocationID.y)) + vec2(0.0, 0.0)) * tileSize / screenSize;
	highp vec2 maxScreenPosition = (vec2(float(gl_GlobalInvocationID.x), float(gl_GlobalInvocationID.y)) + vec2(1.0, 1.0)) * tileSize / screenSize;
	highp float minDepthValue = (float(gl_GlobalInvocationID.z) + 0.0) / float(numDepthTiles);
	highp float maxDepthValue = (float(gl_GlobalInvocationID.z) + 1.0) / float(numDepthTiles);

	highp vec3 minViewPositionNear = ScreenToViewPosition(minScreenPosition, minDepthValue, camera.projectionInverseMatrix).xyz;
	highp vec3 maxViewPositionNear = ScreenToViewPosition(maxScreenPosition, minDepthValue, camera.projectionInverseMatrix).xyz;
	highp vec3 minViewPositionFar = ScreenToViewPosition(minScreenPosition, maxDepthValue, camera.projectionInverseMatrix).xyz;
	highp vec3 maxViewPositionFar = ScreenToViewPosition(maxScreenPosition, maxDepthValue, camera.projectionInverseMatrix).xyz;

	highp vec3 minAABBPosition = min(min(minViewPositionNear, maxViewPositionNear), min(minViewPositionFar, maxViewPositionFar));
	highp vec3 maxAABBPosition = max(max(minViewPositionNear, maxViewPositionNear), max(minViewPositionFar, maxViewPositionFar));

	highp int visibleLightCount = 0;
	highp int visibleLightIndices[256];

	for (int i = 0; i < numPointLights; i++) {
		highp int indexLight = fullLightListData.index[i];
		highp vec3 spherePosition = sceneData.data[indexLight].center.xyz;
		highp float radius = sceneData.data[indexLight].lightAttenuation.w;

		if (Intersection(minAABBPosition, maxAABBPosition, spherePosition, radius)) {
			visibleLightIndices[visibleLightCount] = indexLight;
			visibleLightCount += 1;
		}

		if (visibleLightCount == 256) {
			break;
		}
	}

	cullLightListData.count = 0;

	barrier();

	highp int offset = atomicAdd(cullLightListData.count, visibleLightCount);
	highp int tileIndex = int(gl_WorkGroupID.z * gl_NumWorkGroups.x * gl_NumWorkGroups.y + gl_WorkGroupID.y * gl_NumWorkGroups.x + gl_WorkGroupID.x);

	for (int i = 0; i < visibleLightCount; i++) {
		cullLightListData.index[offset + i] = visibleLightIndices[i];
	}

	clusterData.data[tileIndex].minAABBPosition = vec4(minAABBPosition, float(offset));
	clusterData.data[tileIndex].maxAABBPosition = vec4(maxAABBPosition, float(visibleLightCount));
}
