#version 310 es

#ifdef VERTEX_SHADER
precision mediump float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM;
USE_ENGINE_UNIFORM;

// Output
layout (location = 0) out mediump vec2 outTexcoord;

// Descriptor
// ...

void main()
{
#ifdef _VULKAN_
	highp mat4 projectionViewMatrix = mat4(1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, 1.0);
	highp mat4 worldMatrix = mat4(1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
#else
	highp mat4 projectionViewMatrix = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, 1.0);
	highp mat4 worldMatrix = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
#endif
	gl_Position = projectionViewMatrix * worldMatrix * vec4(inPosition.xyz, 1.0);
	outTexcoord = inTexcoord0;
}
#endif

#ifdef FRAGMENT_SHADER
precision mediump float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM;
USE_ENGINE_UNIFORM;
USE_SCENE_DATA_STORAGE;
USE_CLUSTER_DATA_STORAGE;
USE_CULL_LIGHT_LIST_DATA_STORAGE;
USE_DEPTH_TEXTURE_UNIFORM;

#include "lighting.inc"

// Input
layout (location = 0) in mediump vec2 inTexcoord;

// Output
layout (location = 0) out mediump vec4 outFragColor;

// Descriptor
DESCRIPTOR_SET_INPUTATTACHMENT(1, 6) uniform mediump subpassInput texGBuffer0;
DESCRIPTOR_SET_INPUTATTACHMENT(2, 7) uniform mediump subpassInput texGBuffer1;

layout(push_constant) uniform PushConstantParam {
	float tileSizeX;
	float tileSizeY;
} Param;

void main()
{
	highp vec2 tileSize = vec2(Param.tileSizeX, Param.tileSizeY);

	mediump vec4 pixelColorGBuffer0 = subpassLoad(texGBuffer0);
	mediump vec4 pixelColorGBuffer1 = subpassLoad(texGBuffer1);

	highp float depth = UnpackFloat(texture(texDepth, inTexcoord.xy));
	highp vec3 worldPosition = ScreenToWorldPosition(inTexcoord.xy, depth, cameraProjectionInverseMatrix, cameraViewInverseMatrix).xyz;
	highp vec3 worldCameraPosition = (cameraViewInverseMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	mediump vec3 worldViewDirection = normalize(worldCameraPosition - worldPosition);
	mediump vec3 worldNormal = NormalDecode(pixelColorGBuffer1.rg);

	mediump vec3 albedo = pixelColorGBuffer0.rgb;
	mediump float ao = pixelColorGBuffer0.a;
	mediump float roughness = pixelColorGBuffer1.b;
	mediump float metallic = pixelColorGBuffer1.a;

	outFragColor.rgb = albedo;
	outFragColor.a = 1.0;
}
#endif
