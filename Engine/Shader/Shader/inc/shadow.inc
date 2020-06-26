#ifndef _SHADOW_INC_
#define _SHADOW_INC_

#include "engine.inc"
#include "common.inc"


float FilterPCF(sampler2D texShadow, vec2 texcoord, float curDepth, float resolution)
{
	float result = 0.0;
	vec2 texelSize = vec2(1.0 / resolution);

	for (float x = -1.0; x <= 1.0; x++) {
		for (float y = -1.0; y <= 1.0; y++) {
			vec2 offset = vec2(x, y) * texelSize;
			float shadowDepth = texture(texShadow, texcoord + offset).r;
			result += step(curDepth, shadowDepth);
		}
	}

	return result / 9.0;
}

float LinearFilterPCF(sampler2D texShadow, vec2 texcoord, float curDepth, float resolution)
{
	vec2 texelSize = vec2(1.0 / resolution);
	vec2 center = floor(texcoord * resolution + 0.5) * texelSize;
	vec2 f = fract(texcoord * resolution + 0.5);
	float lb = FilterPCF(texShadow, center + vec2(0.0, 0.0) * texelSize, curDepth, resolution);
	float lt = FilterPCF(texShadow, center + vec2(0.0, 1.0) * texelSize, curDepth, resolution);
	float rb = FilterPCF(texShadow, center + vec2(1.0, 0.0) * texelSize, curDepth, resolution);
	float rt = FilterPCF(texShadow, center + vec2(1.0, 1.0) * texelSize, curDepth, resolution);
	float a = mix(lb, lt, f.y);
	float b = mix(rb, rt, f.y);
	float c = mix(a, b, f.x);
	return c;
}

float ShadowValue(vec3 worldPosition, vec3 worldNormal, sampler2D texShadow)
{
	int indexLevel;
	vec3 worldCameraPosition = (cameraViewInverseMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	float factor = length(worldCameraPosition - worldPosition) / (cameraZFar - cameraZNear);
	if (factor < mainShadowLevelFactor.w) indexLevel = 3;
	if (factor < mainShadowLevelFactor.z) indexLevel = 2;
	if (factor < mainShadowLevelFactor.y) indexLevel = 1;
	if (factor < mainShadowLevelFactor.x) indexLevel = 0;

	vec2 texOffset[4] = vec2[](vec2(0.0, 0.0), vec2(0.5, 0.0), vec2(0.0, 0.5), vec2(0.5, 0.5));
	vec4 projectCoord = mainShadowProjectionViewMatrix(indexLevel) * vec4(worldPosition, 1.0);
	projectCoord.xy /= projectCoord.w;
 	projectCoord.xy = clamp(projectCoord.xy * 0.5 + vec2(0.5), vec2(0.0), vec2(1.0));
	projectCoord.xy = clamp(projectCoord.xy * 0.5 + texOffset[indexLevel], texOffset[indexLevel], texOffset[indexLevel] + vec2(0.5));

	float resolution[4] = float[](2.0, 2.0, 2.0, 2.0);
	float curDepth = (mainShadowProjectionViewMatrix(indexLevel) * vec4(worldPosition, 1.0)).z;

//	return FilterPCF(texShadow, projectCoord.xy, curDepth, resolution[indexLevel] * 2048.0);
	return LinearFilterPCF(texShadow, projectCoord.xy, curDepth, resolution[indexLevel] * 2048.0);
}

#endif
