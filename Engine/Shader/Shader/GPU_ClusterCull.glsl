#version 310 es
layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

precision mediump float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM
USE_SCENE_DATA_STORAGE
USE_CLUSTER_DATA_STORAGE
USE_FULL_LIGHT_LIST_DATA_STORAGE
USE_CULL_LIGHT_LIST_DATA_STORAGE

// Output
// ...

// Descriptor
layout(push_constant) uniform PushConstantParam {
	int numPointLights;
} Param;

void main()
{
	highp int indexTile = int(gl_WorkGroupID.z * gl_NumWorkGroups.x * gl_NumWorkGroups.y + gl_WorkGroupID.y * gl_NumWorkGroups.x + gl_WorkGroupID.x);
	highp int numPointLights = Param.numPointLights;

	highp vec3 minAABBPosition = GetCluster(indexTile).minAABBPosition.xyz;
	highp vec3 maxAABBPosition = GetCluster(indexTile).maxAABBPosition.xyz;

	highp int count = 0;
	highp int indexLights[256];

	for (int i = 0; i < numPointLights; i++) {
		highp int indexLight = GetFullLightListIndex(i);

		highp vec3 center = (cameraViewMatrix * GetInstance(indexLight).center).xyz;
		highp float radius = GetInstance(indexLight).lightAttenuation.w;

		if (Intersection(minAABBPosition, maxAABBPosition, center, radius)) {
			indexLights[count] = indexLight;
			count += 1;
		}

		if (count == 256) {
			break;
		}
	}

	ResetCullLightListCount();

	memoryBarrierBuffer();
	barrier();

	highp int offset = AddCullLightListCount(count);

	for (int i = 0; i < count; i++) {
		SetCullLightListIndex(offset + i, indexLights[i]);
	}

	SetClusterLightInfo(indexTile, offset, count);
}
