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


// Input
layout (location = 0) in mediump vec2 inTexcoord;

// Output
layout (location = 0) out mediump vec4 outFragColor;

// Descriptor
USE_COLOR_TEXTURE_UNIFORM;
USE_ADD_TEXTURE_UNIFORM;

layout(push_constant) uniform PushConstantParam {
	float factor;
} Param;


void main()
{
	mediump vec3 factor = vec3(Param.factor);

	mediump vec3 add = UnpackHDR(texture(texAdd, inTexcoord));
	mediump vec3 color = UnpackHDR(texture(texColor, inTexcoord));

	color = color + factor * add;

	outFragColor = PackHDR(color);
}
#endif
