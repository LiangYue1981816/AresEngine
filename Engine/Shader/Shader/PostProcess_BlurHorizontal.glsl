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

	float offsets[5] = float[](0.0, 1.0, 2.0, 3.0, 4.0);
	float weights[5] = float[](0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

	vec2 texelSize = range / vec2(textureSize(texColor, 0));
	vec3 result = UnpackHDR(texture(texColor, inTexcoord)) * weights[0];

	for (int i = 1; i < 5; i++) {
		vec2 offset = vec2(offsets[i], 0.0) * texelSize;
		result += UnpackHDR(texture(texColor, inTexcoord + offset)) * weights[i];
		result += UnpackHDR(texture(texColor, inTexcoord - offset)) * weights[i];
	}

	outFragColor = PackHDR(result);
}
#endif
