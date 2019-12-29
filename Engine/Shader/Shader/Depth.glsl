#version 310 es

#ifdef VERTEX_SHADER
precision mediump float;
#include "engine.inc"
#include "common.inc"


// VERTEX_ATTRIBUTE_POSITION;
// VERTEX_ATTRIBUTE_TEXCOORD0;
// INSTANCE_ATTRIBUTE_INDEX;

USE_SCENE_DATA_STORAGE;

// Output
layout (location = 0) out mediump vec2 outTexcoord;


void main()
{
	highp mat4 worldMatrix = sceneData.data[inInstanceIndex].transformMatrix;
	highp vec3 worldPosition = (worldMatrix * vec4(inPosition.xyz, 1.0)).xyz;

	outTexcoord = inTexcoord0;
	gl_Position = cameraProjectionViewMatrix * vec4(worldPosition, 1.0);
}
#endif

#ifdef FRAGMENT_SHADER
precision mediump float;
#include "engine.inc"
#include "common.inc"


// Input
layout (location = 0) in mediump vec2 inTexcoord;

// Descriptor
DESCRIPTOR_SET_MATPASS(8) mediump uniform sampler2D texAlbedo;


void main()
{
	mediump vec4 color = texture(texAlbedo, inTexcoord);

	if (color.a < 0.5)
		discard;
}
#endif
