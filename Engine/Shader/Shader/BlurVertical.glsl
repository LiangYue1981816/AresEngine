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

	highp float offsets[5] = float[](0.0, 1.0, 2.0, 3.0, 4.0);
	highp float weights[5] = float[](0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

	highp vec2 texelSize = range / vec2(textureSize(texColor, 0));
	highp vec3 result = UnpackHDR(texture(texColor, inTexcoord)) * weights[0];

	for (int i = 1; i < 5; i++) {
		highp vec2 offset = vec2(0.0, offsets[i]) * texelSize;
		result += UnpackHDR(texture(texColor, inTexcoord + offset)) * weights[i];
		result += UnpackHDR(texture(texColor, inTexcoord - offset)) * weights[i];
	}

	outFragColor = PackHDR(result);
}
#endif
