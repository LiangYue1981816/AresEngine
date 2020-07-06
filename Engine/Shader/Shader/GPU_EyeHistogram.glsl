#version 310 es
layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

precision highp float;
#include "engine.inc"
#include "common.inc"

USE_HISTOGRAM_STORAGE
USE_COLOR_TEXTURE_UNIFORM

// Output
// ...

// Descriptor
// ...

// Shared
shared uint share_histogram[64];


float GetHistogram(float linear, float minLogValue, float maxLogValue)
{
	return clamp((log2(linear) - minLogValue) / (maxLogValue - minLogValue), 0.0, 1.0);
}

void main()
{
	const float minLinearValue = 0.001;
	const float maxLinearValue = 4.0;

	const float minLogValue = log2(minLinearValue);
	const float maxLogValue = log2(maxLinearValue);

	if (gl_LocalInvocationIndex < uint(64)) {
		share_histogram[gl_LocalInvocationIndex] = uint(0);
	}

	barrier();

	vec2 texCoord = vec2(gl_GlobalInvocationID.xy) / vec2(textureSize(texColor, 0));
	vec3 color = texture(texColor, texCoord).rgb;

	float l = clamp(max(color.r, max(color.g, color.b)), minLinearValue, maxLinearValue);
	float h = GetHistogram(l, minLogValue, maxLogValue);

	atomicAdd(share_histogram[int(h * (64.0 - 1.0))], uint(1));

	barrier();

	if (gl_LocalInvocationIndex < uint(64)) {
		atomicAdd(histogramData.histogram[gl_LocalInvocationIndex], share_histogram[gl_LocalInvocationIndex]);
	}
}
