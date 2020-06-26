#version 310 es

#ifdef VERTEX_SHADER
precision highp float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM
USE_ENGINE_UNIFORM

// Output
layout (location = 0) out vec2 outTexcoord;

// Descriptor
// ...

void main()
{
#ifdef _VULKAN_
	mat4 projectionViewMatrix = mat4(1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, 1.0);
	mat4 worldMatrix = mat4(1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
#else
	mat4 projectionViewMatrix = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, 1.0);
	mat4 worldMatrix = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
#endif
	gl_Position = projectionViewMatrix * worldMatrix * vec4(inPosition.xyz, 1.0);
	outTexcoord = inTexcoord0;
}
#endif

#ifdef FRAGMENT_SHADER
precision highp float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM
USE_ENGINE_UNIFORM
USE_COLOR_TEXTURE_UNIFORM

// Input
layout (location = 0) in vec2 inTexcoord;

// Output
layout (location = 0) out vec4 outFragColor;

// Descriptor
layout(push_constant) uniform PushConstantParam {
	float threshold;
} Param;

void main()
{
	float threshold = Param.threshold;
	float softKnee = 0.5;

	float knee = threshold * softKnee + 1e-5;
	vec3 curve = vec3(threshold - knee, knee * 2.0, 0.25 / knee);

	vec3 color = UnpackHDR(texture(texColor, inTexcoord));
	float br = max(max(color.r, color.g), color.b);
	float rq = clamp(br - curve.x, 0.0, curve.y);
	rq = curve.z * rq * rq;

	color *= max(rq, br - threshold) / max(br, 1e-5);

	outFragColor = PackHDR(color);
}
#endif
