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
	float range;
} Param;

void main()
{
	vec2 range = vec2(Param.range);

	vec3 result = vec3(0.0);
	vec2 texelSize = range / vec2(textureSize(texColor, 0));

	for (float x = -1.0; x <= 1.0; x++) {
		for (float y = -1.0; y <= 1.0; y++) {
			vec2 offset = vec2(x, y) * texelSize;
			result += UnpackHDR(texture(texColor, inTexcoord + offset));
		}
	}

	result /= 9.0;

	outFragColor = PackHDR(result);
}
#endif
