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

// Descriptor
// ...

void main()
{
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
USE_DEPTH_TEXTURE_UNIFORM;

#include "light.inc"

// Input
layout (location = 0) in highp vec4 inPosition;

// Output
layout (location = 0) out mediump vec4 outFragColor;

// Descriptor
DESCRIPTOR_SET_INPUTATTACHMENT(1, 6) uniform mediump subpassInput texGBuffer0;
DESCRIPTOR_SET_INPUTATTACHMENT(2, 7) uniform mediump subpassInput texGBuffer1;
DESCRIPTOR_SET_INPUTATTACHMENT(3, 8) uniform mediump subpassInput texGBuffer2;

void main()
{
	highp vec4 texCoord = inPosition;
	texCoord.xy = texCoord.xy / texCoord.w;
	texCoord.xy = texCoord.xy * 0.5 + 0.5;

	mediump vec4 texAlbedo = subpassLoad(texGBuffer0);
	mediump vec4 texNormal = subpassLoad(texGBuffer1);
	mediump vec4 texRoughnessMetallicSpecularAO = subpassLoad(texGBuffer2);

	outFragColor.rgb = vec3(texCoord.x, texCoord.y, 0.0);
	outFragColor.a = 1.0;
}
#endif
