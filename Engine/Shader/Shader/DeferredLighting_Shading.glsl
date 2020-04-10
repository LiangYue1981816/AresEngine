#version 310 es

#ifdef VERTEX_SHADER
precision mediump float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM;
USE_ENGINE_UNIFORM;
USE_SCENE_DATA_STORAGE;

// Output
layout (location = 0) out highp vec4 outPosition;
layout (location = 1) flat out highp int outInstanceIndex;

// Descriptor
// ...

void main()
{
	outInstanceIndex = inInstanceIndex;

	highp mat4 worldMatrix = sceneData.data[inInstanceIndex].transformMatrix;
	highp vec3 worldPosition = (worldMatrix * vec4(inPosition.xyz, 1.0)).xyz;
	gl_Position = outPosition = cameraProjectionViewMatrix * vec4(worldPosition, 1.0);
}
#endif

#ifdef FRAGMENT_SHADER
precision mediump float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM;
USE_ENGINE_UNIFORM;
USE_SCENE_DATA_STORAGE;
USE_DEPTH_TEXTURE_UNIFORM;

#include "light.inc"

// Input
layout (location = 0) in highp vec4 inPosition;
layout (location = 1) flat in highp int inInstanceIndex;

// Output
layout (location = 0) out mediump vec4 outFragColor;

// Descriptor
DESCRIPTOR_SET_INPUTATTACHMENT(1, 6) uniform mediump subpassInput texGBuffer0;
DESCRIPTOR_SET_INPUTATTACHMENT(2, 7) uniform mediump subpassInput texGBuffer1;

highp vec3 ScreenToWorldPosition(highp vec2 screen, highp float depth)
{
	highp vec4 clipPosition = vec4(screen * vec2(2.0) - vec2(1.0), depth * 2.0 - 1.0, 1.0);
	highp vec4 worldPosition = cameraProjectionViewInverseMatrix * clipPosition;
	return (worldPosition.xyz / worldPosition.w);
}

void main()
{
	mediump vec4 pixelColorGBuffer0 = subpassLoad(texGBuffer0);
	mediump vec4 pixelColorGBuffer1 = subpassLoad(texGBuffer1);

	highp vec4 projectCoord = inPosition;
	projectCoord.xy = projectCoord.xy / projectCoord.w;
 	projectCoord.xy = projectCoord.xy * 0.5 + 0.5;

	highp float depth = texture(texDepth, projectCoord.xy).r;
	highp vec3 worldPosition = ScreenToWorldPosition(projectCoord.xy, depth);
	highp vec3 worldCameraPosition = (cameraViewInverseMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	mediump vec3 worldViewDirection = normalize(worldCameraPosition - worldPosition);
	mediump vec3 worldNormal = NormalDecode(pixelColorGBuffer1.rg);

	mediump vec3 albedo = pixelColorGBuffer0.rgb;
	mediump float ao = pixelColorGBuffer0.a;
	mediump float roughness = pixelColorGBuffer1.b;
	mediump float metallic = pixelColorGBuffer1.a;

	highp vec3 pointLightPosition = sceneData.data[inInstanceIndex].center.xyz;
	highp vec3 pointLightDirection = normalize(pointLightPosition - worldPosition);

	highp float distance = length(pointLightPosition - worldPosition);
	highp float pointLightRange = sceneData.data[inInstanceIndex].lightAttenuation.w;

	mediump vec3 pointLightAttenuation = sceneData.data[inInstanceIndex].lightAttenuation.xyz;
	mediump vec3 pointLightColor = sceneData.data[inInstanceIndex].lightColor.rgb * Attenuation(distance, pointLightAttenuation.x, pointLightAttenuation.y, pointLightAttenuation.z);

	mediump vec3 fresnel = Fresnel(worldNormal, worldViewDirection, albedo.rgb, metallic);
	mediump vec3 pointLighting = PBRLighting(worldNormal, worldViewDirection, pointLightDirection, pointLightColor, albedo, fresnel, metallic, roughness) * pointLightFactor;

	outFragColor.rgb = (pointLightRange > distance) ? (ao * pointLighting) : vec3(0.0);
	outFragColor.a = 1.0;
}
#endif
