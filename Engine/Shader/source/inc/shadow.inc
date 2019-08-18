#ifndef _SHADOW_INC_
#define _SHADOW_INC_

#include "engine.inc"
#include "common.inc"


highp vec4 EncodeFloat(highp float value)
{
	highp vec4 kEncodeMul = vec4(1.0, 255.0, 65025.0, 16581375.0);
	highp vec4 enc = kEncodeMul * value;
	enc  = fract(enc);
	enc -= enc.yzww / 255.0;
	return enc;
}

highp float DecodeFloat(highp vec4 encode) 
{
	highp vec4 kDecodeDot = vec4(1.0, 1.0 / 255.0, 1.0 / 65025.0, 1.0 / 16581375.0);
	return dot(encode, kDecodeDot);
}

highp float ShadowDepth(highp sampler2D shadowmap, highp vec2 texCoord)
{
	return DecodeFloat(texture(shadowmap, texCoord.xy));
}

highp float ShadowValue(highp vec3 worldPosition, highp sampler2D shadowmap)
{
	highp int indexLevel;
	highp vec3 worldCameraPosition = (cameraViewInverseMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	highp float factor = length(worldCameraPosition - worldPosition) / (cameraZFar - cameraZNear);
	if (factor < mainShadowLevelFactor.w) indexLevel = 3;
	if (factor < mainShadowLevelFactor.z) indexLevel = 2;
	if (factor < mainShadowLevelFactor.y) indexLevel = 1;
	if (factor < mainShadowLevelFactor.x) indexLevel = 0;

	highp vec2 texOffset[4];
	texOffset[0] = vec2(0.0, 0.0);
	texOffset[1] = vec2(0.5, 0.0);
	texOffset[2] = vec2(0.0, 0.5);
	texOffset[3] = vec2(0.5, 0.5);
	highp vec4 projectCoord = mainShadowProjectionViewMatrix(indexLevel) * vec4(worldPosition.xyz, 1.0);
	projectCoord.xy /= projectCoord.w;
 	projectCoord.xy = clamp(projectCoord.xy * 0.5 + vec2(0.5, 0.5), vec2(0.0), vec2(1.0));
	projectCoord.xy = clamp(projectCoord.xy * 0.5 + texOffset[indexLevel], texOffset[indexLevel], texOffset[indexLevel] + vec2(0.5));

	/*
	highp vec4 resolution = vec4(2048.0, 2048.0, 4096.0, 4096.0);
	highp vec2 unnormalized = projectCoord.xy * resolution[indexLevel];
	highp vec2 fractional = fract(unnormalized);
	unnormalized = floor(unnormalized);
	highp float shadowDepth0 = ShadowDepth(shadowmap, (unnormalized + vec2(0.0f, 0.0f)) / resolution[indexLevel]);
	highp float shadowDepth1 = ShadowDepth(shadowmap, (unnormalized + vec2(1.0f, 0.0f)) / resolution[indexLevel]);
	highp float shadowDepth2 = ShadowDepth(shadowmap, (unnormalized + vec2(1.0f, 1.0f)) / resolution[indexLevel]);
	highp float shadowDepth3 = ShadowDepth(shadowmap, (unnormalized + vec2(0.0f, 1.0f)) / resolution[indexLevel]);
	highp float shadowDepth = 0.0;
	shadowDepth = (shadowDepth0 + (shadowDepth3 - shadowDepth0) * fractional.y);
	shadowDepth = shadowDepth + ((shadowDepth1 + (shadowDepth2 - shadowDepth1) * fractional.y) - shadowDepth) * fractional.x;
	/*/
	highp float shadowDepth = ShadowDepth(shadowmap, projectCoord.xy);
	//*/

	highp vec4 biasOffset = vec4(0.000, 0.000, 0.000, 0.000);
	highp float curDepth = ((mainShadowViewMatrix(indexLevel) * vec4(worldPosition, 1.0)).z - mainShadowZNear(indexLevel)) / (mainShadowZFar(indexLevel) - mainShadowZNear(indexLevel)) + biasOffset[indexLevel];	

	highp float shadowValue = curDepth > shadowDepth ? 1.0 : 0.0;
//	highp float shadowValue = exp((mainShadowZFar(indexLevel) - mainShadowZNear(indexLevel)) * (curDepth - shadowDepth));
	shadowValue = clamp(shadowValue, 0.0, 1.0);

	return shadowValue;
}

#endif
