#version 310 es

#ifdef VERTEX_SHADER
precision mediump float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM
USE_ENGINE_UNIFORM
USE_SCENE_DATA_STORAGE

// Output
layout (location = 0) out mediump vec2 outTexcoord;

// Descriptor
// ...

void main()
{
	highp mat4 worldMatrix = GetInstance(inInstanceIndex).transformMatrix;
	highp vec3 worldPosition = (worldMatrix * vec4(inPosition.xyz, 1.0)).xyz;

	outTexcoord = inTexcoord0;
	gl_Position = cameraProjectionViewMatrix * vec4(worldPosition, 1.0);
}
#endif

#ifdef FRAGMENT_SHADER
precision mediump float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM
USE_ENGINE_UNIFORM

// Input
layout (location = 0) in mediump vec2 inTexcoord;

// Output
layout (location = 0) out mediump vec4 outFragColor;

// Descriptor
DESCRIPTOR_SET_MATPASS(8) mediump uniform sampler2D texAlbedo;

void main()
{
	mediump vec4 color = texture(texAlbedo, inTexcoord);

	if (color.a < 0.5)
		discard;

	outFragColor = PackHDR(color.rgb);
}
#endif
