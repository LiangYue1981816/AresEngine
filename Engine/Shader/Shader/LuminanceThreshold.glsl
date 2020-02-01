#version 310 es

#ifdef VERTEX_SHADER
precision mediump float;
#include "engine.inc"
#include "common.inc"

// VERTEX_ATTRIBUTE_POSITION;
// VERTEX_ATTRIBUTE_TEXCOORD0;

// Output
layout (location = 0) out mediump vec2 outTexcoord;

// Descriptor
USE_CAMERA_UNIFORM;
USE_ENGINE_UNIFORM;

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
USE_CAMERA_UNIFORM;
USE_ENGINE_UNIFORM;
USE_COLOR_TEXTURE_UNIFORM;

layout(push_constant) uniform PushConstantParam {
	float threshold;
} Param;

void main()
{
	mediump float threshold = Param.threshold;
	mediump float softKnee = 0.5;

	mediump float knee = threshold * softKnee + 1e-5;
	mediump vec3 curve = vec3(threshold - knee, knee * 2.0, 0.25 / knee);

	mediump vec3 color = UnpackHDR(texture(texColor, inTexcoord));
	mediump float br = max(max(color.r, color.g), color.b);
	mediump float rq = clamp(br - curve.x, 0.0, curve.y);
	rq = curve.z * rq * rq;

	color *= max(rq, br - threshold) / max(br, 1e-5);

	outFragColor = PackHDR(color);
}
#endif
