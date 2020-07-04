#version 310 es

#ifdef VERTEX_SHADER
precision highp float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM
USE_ENGINE_UNIFORM

// Output
layout (location = 0) out vec2 outTexcoord;

// Descriptor
// ...

void main()
{
#ifdef _VULKAN_
	mat4 projectionViewMatrix = mat4(1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, 1.0);
	mat4 worldMatrix = mat4(1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
#else
	mat4 projectionViewMatrix = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, 1.0);
	mat4 worldMatrix = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
#endif
	gl_Position = projectionViewMatrix * worldMatrix * vec4(inPosition.xyz, 1.0);
	outTexcoord = inTexcoord0;
}
#endif

#ifdef FRAGMENT_SHADER
precision highp float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM
USE_ENGINE_UNIFORM
USE_SCENE_DATA_STORAGE
USE_CLUSTER_DATA_STORAGE
USE_CULL_LIGHT_LIST_DATA_STORAGE
USE_DEPTH_TEXTURE_UNIFORM

#include "lighting.inc"

// Input
layout (location = 0) in vec2 inTexcoord;

// Output
layout (location = 0) out vec4 outFragColor;

// Descriptor
DESCRIPTOR_SET_INPUTATTACHMENT(1, 6) uniform highp subpassInput texGBuffer0;
DESCRIPTOR_SET_INPUTATTACHMENT(2, 7) uniform highp subpassInput texGBuffer1;

layout(push_constant) uniform PushConstantParam {
	int numDepthTiles;
	int numWidthTiles;
	int numHeightTiles;
} Param;

void main()
{
	int numDepthTiles = Param.numDepthTiles;
	int numWidthTiles = Param.numWidthTiles;
	int numHeightTiles = Param.numHeightTiles;

	vec4 pixelColorGBuffer0 = subpassLoad(texGBuffer0);
	vec4 pixelColorGBuffer1 = subpassLoad(texGBuffer1);

	float depth = texture(texDepth, inTexcoord.xy).r;
	vec3 worldPosition = ScreenToWorldPosition(inTexcoord.xy, depth, cameraProjectionInverseMatrix, cameraViewInverseMatrix).xyz;
	vec3 worldCameraPosition = (cameraViewInverseMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	vec3 worldViewDirection = normalize(worldCameraPosition - worldPosition);
	vec3 worldNormal = NormalDecode(pixelColorGBuffer1.rg);

	vec3 albedo = pixelColorGBuffer0.rgb;
	float ao = pixelColorGBuffer0.a;
	float roughness = pixelColorGBuffer1.b;
	float metallic = pixelColorGBuffer1.a;

	float scale = float(numDepthTiles) / log2(cameraZFar / cameraZNear);
	float bias = -float(numDepthTiles) * log2(cameraZNear) / log2(cameraZFar / cameraZNear);

	int indexTileX = int(inTexcoord.x * float(numWidthTiles));
	int indexTileY = int(inTexcoord.y * float(numHeightTiles));
	int indexTileZ = int(log2(LinearDepth(depth, cameraZNear, cameraZFar)) * scale + bias);
	int indexTile = int(indexTileZ * numWidthTiles * numHeightTiles + indexTileY * numWidthTiles + indexTileX);

	int offset = GetClusterLightOffset(indexTile);
	int count = GetClusterLightCount(indexTile);

	vec3 pointLighting = vec3(0.0);

	for (int index = 0; index < count; index++)
	{
		int indexLight = GetCullLightListIndex(index + offset);

		vec3 pointLightPosition = GetInstance(indexLight).center.xyz;
		float pointLightRange = GetInstance(indexLight).lightAttenuation.w;
		float distance = length(pointLightPosition - worldPosition);

		if(pointLightRange > distance)
		{
			vec3 pointLightDirection = normalize(pointLightPosition - worldPosition);
			vec3 pointLightAttenuation = GetInstance(indexLight).lightAttenuation.xyz;
			vec3 pointLightColor = GetInstance(indexLight).lightColor.rgb * Attenuation(distance, pointLightAttenuation.x, pointLightAttenuation.y, pointLightAttenuation.z);
			vec3 lighting = PBRLighting(worldNormal, worldViewDirection, pointLightDirection, pointLightColor, albedo, 0.5, metallic, roughness) * pointLightFactor;
			pointLighting += ao * lighting;
		}
	}

//	outFragColor.rgb = vec3(float(indexTileX) / float(numWidthTiles));
//	outFragColor.rgb = vec3(float(indexTileY) / float(numHeightTiles));
//	outFragColor.rgb = vec3(float(indexTileZ) / float(numDepthTiles));
//	outFragColor.rgb = vec3(float(indexTileX) / float(numWidthTiles), float(indexTileY) / float(numHeightTiles), float(indexTileZ) / float(numDepthTiles));
	outFragColor.rgb = pointLighting;
	outFragColor.a = 1.0;
}
#endif
