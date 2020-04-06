#version 310 es

#ifdef VERTEX_SHADER
precision mediump float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM;
USE_ENGINE_UNIFORM;
USE_SCENE_DATA_STORAGE;

// Output
// ...

// Descriptor
// ...

void main()
{
	highp mat4 worldMatrix = sceneData.data[inInstanceIndex].transformMatrix;
	highp vec3 worldPosition = (worldMatrix * vec4(inPosition.xyz, 1.0)).xyz;
	gl_Position = cameraProjectionViewMatrix * vec4(worldPosition, 1.0);
}
#endif

#ifdef FRAGMENT_SHADER
precision mediump float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM;
USE_ENGINE_UNIFORM;

#include "light.inc"

// Input
// ...

// Output
layout (location = 0) out mediump vec4 outFragColor;

// Descriptor
layout (input_attachment_index = 0, binding = 1) uniform mediump subpassInput inFragGBuffer0;
layout (input_attachment_index = 1, binding = 2) uniform mediump subpassInput inFragGBuffer1;
layout (input_attachment_index = 2, binding = 3) uniform mediump subpassInput inFragGBuffer2;

void main()
{
	vec4 texAlbedo = subpassLoad(inFragGBuffer0);
	vec4 texNormal = subpassLoad(inFragGBuffer1);
	vec4 texRoughnessMetallicSpecularAO = subpassLoad(inFragGBuffer2);
	
	outFragColor.rgb = texAlbedo.rgb;
	outFragColor.a = 1.0;
}
#endif
