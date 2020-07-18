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
USE_HISTOGRAM_STORAGE

#include "lighting.inc"

// Input
layout (location = 0) in vec2 inTexcoord;

// Output
layout (location = 0) out vec4 outFragColor;

// Descriptor
layout(push_constant) uniform PushConstantParam{
	float lower;
	float upper;
	float luminance;
	float minScaleLuminance;
	float maxScaleLuminance;
} Param;


float GetLuminance(float histogram, float minLogValue, float maxLogValue)
{
	return exp2(histogram * (maxLogValue - minLogValue) + minLogValue);
}

void main()
{
	const float minLinearValue = 0.001;
	const float maxLinearValue = 4.0;

	const float minLogValue = log2(minLinearValue);
	const float maxLogValue = log2(maxLinearValue);

	float lower = Param.lower;
	float upper = Param.upper;
	float luminance = Param.luminance;
	float minScaleLuminance = Param.minScaleLuminance;
	float maxScaleLuminance = Param.maxScaleLuminance;

	vec3 color = UnpackHDR(texture(texColor, inTexcoord));

	uint totalValue = uint(0);
	for (int i = 0; i < HISTOGRAM_SIZE; i++) {
		totalValue += histogramData.histogram[i];
	}

	float totalLuminance = 0.0;
	uint  totalCount = uint(0);
	uint  numLower = uint(lower * float(totalValue));
	uint  numUpper = uint(upper * float(totalValue));
	for (int i = 0; i < HISTOGRAM_SIZE; i++) {
		uint num = histogramData.histogram[i];
		uint offset = min(numLower, num);

		num -= offset;
		numLower -= offset;
		numUpper -= offset;

		num = min(numUpper, num);
		numUpper -= num;

		totalLuminance += GetLuminance(float(i) / float(HISTOGRAM_SIZE - 1), minLogValue, maxLogValue) * float(num);
		totalCount += num;
	}

	float averageLuminance = totalLuminance / float(totalCount);
	float scaleLuminance = clamp(luminance / averageLuminance, minScaleLuminance, maxScaleLuminance);

	vec3 scaleColor = color * scaleLuminance;
	vec3 finalColor = mix(color, scaleColor, 0.5);

	outFragColor.rgb = finalColor;
	outFragColor.a = 1.0;
}
#endif
