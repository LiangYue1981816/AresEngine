#ifndef _SHADOW_INC_
#define _SHADOW_INC_

#include "engine.inc"
#include "common.inc"


highp vec3 EncodeFloatRGB(highp float value)
{
	highp vec3 encode;
	value = sqrt(value) * 8388608.0;
	encode.x = floor(value * 0.00001526);
	value -= encode.x * 65536.0;
	encode.y = floor(value * 0.00390625);
	value -= encode.y * 256.0;
	encode.z = floor(value);
	encode = vec3(127.0, 255.0, 255.0) - encode;
	return encode * 0.003922;
}

highp float DecodeFloatRGB(highp vec3 encode) 
{
	highp vec3 factor = vec3(1.9921875, 0.0077820, 0.000030398);
	highp float value = 1.0 - dot(encode, factor);
	return value * value;
}

highp float SimpleShadowDepth(highp sampler2D shadowmap, highp vec2 texCoord)
{
	return DecodeFloatRGB(texture(shadowmap, texCoord.xy).rgb);
}

highp float LinearShadowDepth(lowp int indexLevel, highp sampler2D shadowmap, highp vec2 texCoord)
{
	highp vec2 unnormalized = texCoord * mainShadowResolution(indexLevel);
	highp vec2 fractional = fract(unnormalized);
	unnormalized = floor(unnormalized);

	highp float v0 = DecodeFloatRGB(texture(shadowmap, (unnormalized + vec2(0.0f, 0.0f)) / mainShadowResolution(indexLevel)).rgb);
	highp float v1 = DecodeFloatRGB(texture(shadowmap, (unnormalized + vec2(1.0f, 0.0f)) / mainShadowResolution(indexLevel)).rgb);
	highp float v2 = DecodeFloatRGB(texture(shadowmap, (unnormalized + vec2(1.0f, 1.0f)) / mainShadowResolution(indexLevel)).rgb);
	highp float v3 = DecodeFloatRGB(texture(shadowmap, (unnormalized + vec2(0.0f, 1.0f)) / mainShadowResolution(indexLevel)).rgb);

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

	highp vec3 shadowViewPosition = (mainShadowViewMatrix(indexLevel) * vec4(position.xyz, 1.0)).xyz;
	highp float curDepth = length(shadowViewPosition) - 0.001;

	highp vec4 projectCoord = mainShadowProjectionViewMatrix(indexLevel) * vec4(position.xyz, 1.0);
	projectCoord.xy /= projectCoord.w;
 	projectCoord.xy = projectCoord.xy * 0.5 + vec2(0.5, 0.5);
	projectCoord.xy = projectCoord.xy * 0.5 + offset[indexLevel];

	highp float shadowDepth = SimpleShadowDepth(shadowmap, projectCoord.xy) * mainShadowClipPlane(indexLevel).x;

	highp vec3 cameraPosition = (cameraViewInverseMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	highp float shadowAtten = (mainShadowDistance(indexLevel) - length(position - cameraPosition)) / mainShadowDistance(indexLevel);
	shadowAtten = pow2(shadowAtten);
	shadowAtten = clamp(shadowAtten, 0.0, 1.0);

	highp float shadowValue = exp(shadowDepth - curDepth);
	shadowValue = 1.0 - (1.0 - shadowValue) * shadowAtten;
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

	highp vec3 shadowViewPosition = (mainShadowViewMatrix(indexLevel) * vec4(position.xyz, 1.0)).xyz;
	highp float curDepth = length(shadowViewPosition) - 0.001;

	highp vec4 projectCoord = mainShadowProjectionViewMatrix(indexLevel) * vec4(position.xyz, 1.0);
	projectCoord.xy /= projectCoord.w;
 	projectCoord.xy = projectCoord.xy * 0.5 + vec2(0.5, 0.5);
	projectCoord.xy = projectCoord.xy * 0.5 + offset[indexLevel];

	highp float shadowDepth = LinearShadowDepth(indexLevel, shadowmap, projectCoord.xy) * mainShadowClipPlane(indexLevel).x;

	highp vec3 cameraPosition = (cameraViewInverseMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	highp float shadowAtten = (mainShadowDistance(indexLevel) - length(position - cameraPosition)) / mainShadowDistance(indexLevel);
	shadowAtten = pow2(shadowAtten);
	shadowAtten = clamp(shadowAtten, 0.0, 1.0);

	highp float shadowValue = exp(shadowDepth - curDepth);
	shadowValue = 1.0 - (1.0 - shadowValue) * shadowAtten;
	shadowValue = clamp(shadowValue, 0.0, 1.0);

	return shadowValue;
}

#endif
