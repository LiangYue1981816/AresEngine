#ifndef _SHADOW_INC_
#define _SHADOW_INC_

#include "engine.inc"
#include "common.inc"


highp float FilterPCF(highp sampler2D texShadow, highp vec2 texcoord, highp float curDepth, highp float resolution)
{
	highp float result = 0.0;
	highp vec2 texelSize = vec2(1.0 / resolution);

	for (highp float x = -1.0; x <= 1.0; x++) {
		for (highp float y = -1.0; y <= 1.0; y++) {
			highp vec2 offset = vec2(x, y) * texelSize;
			highp float shadowDepth = texture(texShadow, texcoord + offset).r;
			result += step(curDepth, shadowDepth);
		}
	}

	return result / 9.0;
}

highp float LinearFilterPCF(highp sampler2D texShadow, highp vec2 texcoord, highp float curDepth, highp float resolution)
{
	highp vec2 texelSize = vec2(1.0 / resolution);
	highp vec2 center = floor(texcoord * resolution + 0.5) * texelSize;
	highp vec2 f = fract(texcoord * resolution + 0.5);
	highp float lb = FilterPCF(texShadow, center + vec2(0.0, 0.0) * texelSize, curDepth, resolution);
	highp float lt = FilterPCF(texShadow, center + vec2(0.0, 1.0) * texelSize, curDepth, resolution);
	highp float rb = FilterPCF(texShadow, center + vec2(1.0, 0.0) * texelSize, curDepth, resolution);
	highp float rt = FilterPCF(texShadow, center + vec2(1.0, 1.0) * texelSize, curDepth, resolution);
	highp float a = mix(lb, lt, f.y);
	highp float b = mix(rb, rt, f.y);
	highp float c = mix(a, b, f.x);
	return c;
}

highp float ShadowValue(highp vec3 worldPosition, highp vec3 worldNormal, highp sampler2D texShadow)
{
	highp int indexLevel;
	highp vec3 worldCameraPosition = (cameraViewInverseMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	highp float factor = length(worldCameraPosition - worldPosition) / (cameraZFar - cameraZNear);
	if (factor < mainShadowLevelFactor.w) indexLevel = 3;
	if (factor < mainShadowLevelFactor.z) indexLevel = 2;
	if (factor < mainShadowLevelFactor.y) indexLevel = 1;
	if (factor < mainShadowLevelFactor.x) indexLevel = 0;

	highp vec2 texOffset[4] = vec2[](vec2(0.0, 0.0), vec2(0.5, 0.0), vec2(0.0, 0.5), vec2(0.5, 0.5));
	highp vec4 projectCoord = mainShadowProjectionViewMatrix(indexLevel) * vec4(worldPosition, 1.0);
	projectCoord.xy /= projectCoord.w;
 	projectCoord.xy = clamp(projectCoord.xy * 0.5 + vec2(0.5), vec2(0.0), vec2(1.0));
	projectCoord.xy = clamp(projectCoord.xy * 0.5 + texOffset[indexLevel], texOffset[indexLevel], texOffset[indexLevel] + vec2(0.5));

	highp float resolution[4] = float[](2.0, 2.0, 2.0, 2.0);
	highp float curDepth = (mainShadowProjectionViewMatrix(indexLevel) * vec4(worldPosition, 1.0)).z;

//	return FilterPCF(texShadow, projectCoord.xy, curDepth, resolution[indexLevel] * 2048.0);
	return LinearFilterPCF(texShadow, projectCoord.xy, curDepth, resolution[indexLevel] * 2048.0);
}

#endif
