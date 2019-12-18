#version 310 es

#ifdef VERTEX_SHADER
precision mediump float;
#include "engine.inc"
#include "common.inc"


// VERTEX_ATTRIBUTE_POSITION;
// VERTEX_ATTRIBUTE_TEXCOORD0;

// Output
layout (location = 0) out mediump vec2 outTexcoord;


void main()
{
#ifdef _VULKAN_
	highp mat4 projectionViewMatrix = mat4(1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0,-0.5, 0.0, 0.0, 0.0, 1.0, 1.0);
#else
	highp mat4 projectionViewMatrix = mat4(1.0, 0.0, 0.0, 0.0, 0.0,  1.0, 0.0, 0.0, 0.0, 0.0,-0.5, 0.0, 0.0, 0.0, 1.0, 1.0);
#endif
	gl_Position = projectionViewMatrix * vec4(inPosition.xyz, 1.0);
	outTexcoord = inTexcoord0;
}
#endif

#ifdef FRAGMENT_SHADER
precision mediump float;
#include "engine.inc"
#include "common.inc"


// Input
layout (location = 0) in mediump vec2 inTexcoord;

// Output
layout (location = 0) out mediump vec4 outFragColor;

// Descriptor
USE_COLOR_TEXTURE_UNIFORM;

layout(push_constant) uniform PushConstantParam {
	float range;
} Param;


void main()
{
	highp vec2 range = vec2(Param.range);

	highp vec3 result = vec3(0.0);
	highp vec2 texelSize = range / vec2(textureSize(texColor, 0));

	for (highp float x = -1.0; x <= 1.0; x++) {
		for (highp float y = -1.0; y <= 1.0; y++) {
			highp vec2 offset = vec2(x, y) * texelSize;
			result += UnpackHDR(texture(texColor, inTexcoord + offset));
		}
	}

	result /= 9.0;

	outFragColor = PackHDR(result);
}
#endif
