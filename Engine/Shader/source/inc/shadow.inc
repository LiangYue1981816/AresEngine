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

highp float SimpleShadowDepth(highp sampler2D shadowmap, highp vec2 texCoord)
{
	return DecodeFloat(texture(shadowmap, texCoord.xy));
}

highp float LinearShadowDepth(lowp int indexLevel, highp sampler2D shadowmap, highp vec2 texCoord)
{
	highp float resolution = 512.0;
	highp vec2 unnormalized = texCoord * resolution;
	highp vec2 fractional = fract(unnormalized);
	unnormalized = floor(unnormalized);

	highp float v0 = DecodeFloat(texture(shadowmap, (unnormalized + vec2(0.0f, 0.0f)) / resolution));
	highp float v1 = DecodeFloat(texture(shadowmap, (unnormalized + vec2(1.0f, 0.0f)) / resolution));
	highp float v2 = DecodeFloat(texture(shadowmap, (unnormalized + vec2(1.0f, 1.0f)) / resolution));
	highp float v3 = DecodeFloat(texture(shadowmap, (unnormalized + vec2(0.0f, 1.0f)) / resolution));

	highp float v = 0.0;
	v = (v0 + (v3 - v0) * fractional.y);
	v = v + ((v1 + (v2 - v1) * fractional.y) - v) * fractional.x;

	return v;
}

highp float SimpleShadowValue(lowp int indexLevel, highp vec3 position, highp sampler2D shadowmap)
{
	highp vec2 offset[4];
	offset[0] = vec2(0.0, 0.0);
	offset[1] = vec2(0.5, 0.0);
	offset[2] = vec2(0.0, 0.5);
	offset[3] = vec2(0.5, 0.5);

	highp vec4 projectCoord = mainShadowProjectionViewMatrix(indexLevel) * vec4(position.xyz, 1.0);
	projectCoord.xy /= projectCoord.w;
 	projectCoord.xy = clamp(projectCoord.xy * 0.5 + vec2(0.5, 0.5), vec2(0.0), vec2(1.0));
	projectCoord.xy = clamp(projectCoord.xy * 0.5 + offset[indexLevel], offset[indexLevel], offset[indexLevel] + vec2(0.5));

	highp float shadowDepth = SimpleShadowDepth(shadowmap, projectCoord.xy);
	highp float shadowRange = mainShadowZFar(indexLevel) - mainShadowZNear(indexLevel);

	highp vec3 worldShadowPosition = (mainShadowViewInverseMatrix(indexLevel) * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	highp float curDepth = length(worldShadowPosition - position) / shadowRange + 0.01;

	/*
	highp vec3 cameraPosition = (cameraViewInverseMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	highp float shadowAtten = (mainShadowDistance(indexLevel) - length(position - cameraPosition)) / mainShadowDistance(indexLevel);
	shadowAtten = pow2(shadowAtten);
	shadowAtten = clamp(shadowAtten, 0.0, 1.0);

	highp float shadowValue = exp(shadowDepth - curDepth);
	shadowValue = 1.0 - (1.0 - shadowValue) * shadowAtten;
	shadowValue = clamp(shadowValue, 0.0, 1.0);
	*/

	highp float shadowValue = exp(shadowDepth - curDepth);
	shadowValue = clamp(shadowValue, 0.0, 1.0);

	return shadowValue;
}

highp float LinearShadowValue(lowp int indexLevel, highp vec3 position, highp sampler2D shadowmap)
{
	highp vec2 offset[4];
	offset[0] = vec2(0.0, 0.0);
	offset[1] = vec2(0.5, 0.0);
	offset[2] = vec2(0.0, 0.5);
	offset[3] = vec2(0.5, 0.5);

	highp vec4 projectCoord = mainShadowProjectionViewMatrix(indexLevel) * vec4(position.xyz, 1.0);
	projectCoord.xy /= projectCoord.w;
 	projectCoord.xy = clamp(projectCoord.xy * 0.5 + vec2(0.5, 0.5), vec2(0.0), vec2(1.0));
	projectCoord.xy = clamp(projectCoord.xy * 0.5 + offset[indexLevel], offset[indexLevel], offset[indexLevel] + vec2(0.5));

	highp float shadowDepth = SimpleShadowDepth(shadowmap, projectCoord.xy);
	highp float shadowRange = mainShadowZFar(indexLevel) - mainShadowZNear(indexLevel);
	highp float curDepth = length((mainShadowViewMatrix(indexLevel) * vec4(position.xyz, 1.0)).xyz) / shadowRange + 0.01;

	/*
	highp vec3 cameraPosition = (cameraViewInverseMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	highp float shadowAtten = (mainShadowDistance(indexLevel) - length(position - cameraPosition)) / mainShadowDistance(indexLevel);
	shadowAtten = pow2(shadowAtten);
	shadowAtten = clamp(shadowAtten, 0.0, 1.0);

	highp float shadowValue = exp(shadowDepth - curDepth);
	shadowValue = 1.0 - (1.0 - shadowValue) * shadowAtten;
	shadowValue = clamp(shadowValue, 0.0, 1.0);
	*/

	highp float shadowValue = exp(shadowDepth - curDepth);
	shadowValue = clamp(shadowValue, 0.0, 1.0);

	return shadowValue;
}

#endif
