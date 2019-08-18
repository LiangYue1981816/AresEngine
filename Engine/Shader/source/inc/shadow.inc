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

highp vec2 RandomOffset(highp vec3 seed, highp int index)
{
	highp vec2 offset[16] = vec2[]( 
		vec2(-0.94201624, -0.39906216), 
		vec2( 0.94558609, -0.76890725), 
		vec2(-0.094184101,-0.92938870), 
		vec2( 0.34495938,  0.29387760), 
		vec2(-0.91588581,  0.45771432), 
		vec2(-0.81544232, -0.87912464), 
		vec2(-0.38277543,  0.27676845), 
		vec2( 0.97484398,  0.75648379), 
		vec2( 0.44323325, -0.97511554), 
		vec2( 0.53742981, -0.47373420), 
		vec2(-0.26496911, -0.41893023), 
		vec2( 0.79197514,  0.19090188), 
		vec2(-0.24188840,  0.99706507), 
		vec2(-0.81409955,  0.91437590), 
		vec2( 0.19984126,  0.78641367), 
		vec2( 0.14383161, -0.14100790) 
	);

	highp vec4 seed4 = vec4(floor(seed.xyz * 1000.0), index);
	highp float dotProduct = dot(seed4, vec4(12.9898, 78.233, 45.164, 94.673));
	return offset[int(16.0 * fract(sin(dotProduct) * 43758.5453)) % 16];
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

	highp vec2 texOffset[4] = vec2[](vec2(0.0, 0.0), vec2(0.5, 0.0), vec2(0.0, 0.5), vec2(0.5, 0.5));
	highp vec4 projectCoord = mainShadowProjectionViewMatrix(indexLevel) * vec4(worldPosition.xyz, 1.0);
	projectCoord.xy /= projectCoord.w;
 	projectCoord.xy = clamp(projectCoord.xy * 0.5 + vec2(0.5, 0.5), vec2(0.0), vec2(1.0));
	projectCoord.xy = clamp(projectCoord.xy * 0.5 + texOffset[indexLevel], texOffset[indexLevel], texOffset[indexLevel] + vec2(0.5));

	highp vec4 biasOffset = vec4(0.001, 0.0015, 0.003, 0.006);
	highp float curDepth = ((mainShadowViewMatrix(indexLevel) * vec4(worldPosition, 1.0)).z - mainShadowZNear(indexLevel)) / (mainShadowZFar(indexLevel) - mainShadowZNear(indexLevel)) + biasOffset[indexLevel];

	highp float visibility = 1.0;
	highp vec4 resolution = vec4(1024.0, 2048.0, 2048.0, 4096.0);
	for (int i = 0; i < 4; i++){
		highp vec2 randomOffset = RandomOffset(worldPosition.xyz, i) / resolution[indexLevel];
		highp float shadowDepth = ShadowDepth(shadowmap, projectCoord.xy + randomOffset);
		visibility -= curDepth > shadowDepth ? 0.0 : 0.25;
	}

	return clamp(visibility, 0.0, 1.0);
}

#endif
