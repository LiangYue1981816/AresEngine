#version 310 es
layout (local_size_x = 16, local_size_y = 8, local_size_z = 8) in;

precision highp float;
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

// Shared
shared int indexLightCount;

void main()
{
	indexLightCount = 0;

	int indexTile = int(gl_LocalInvocationID.z * gl_WorkGroupSize.x * gl_WorkGroupSize.y + gl_LocalInvocationID.y * gl_WorkGroupSize.x + gl_LocalInvocationID.x);
	int numPointLights = Param.numPointLights;

	vec3 minAABBPosition = GetCluster(indexTile).minAABBPosition.xyz;
	vec3 maxAABBPosition = GetCluster(indexTile).maxAABBPosition.xyz;

	int count = 0;
	int indexLights[256];

	for (int i = 0; i < numPointLights; i++) {
		int indexLight = GetFullLightListIndex(i);

		vec3 center = (cameraViewMatrix * GetInstance(indexLight).center).xyz;
		float radius = GetInstance(indexLight).lightAttenuation.w;

		if (Intersection(minAABBPosition, maxAABBPosition, center, radius)) {
			indexLights[count] = indexLight;
			count += 1;
		}

		if (count == 256) {
			break;
		}
	}

	barrier();

	int offset = atomicAdd(indexLightCount, count);

	for (int i = 0; i < count; i++) {
		SetCullLightListIndex(offset + i, indexLights[i]);
	}

	SetClusterLightInfo(indexTile, offset, count);
}
