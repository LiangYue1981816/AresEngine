#version 310 es

#ifdef VERTEX_SHADER
precision mediump float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM;
USE_ENGINE_UNIFORM;

// Output
layout (location = 0) out mediump vec2 outTexcoord;

// Descriptor
// ...

void main()
{
#ifdef _VULKAN_
	highp mat4 projectionViewMatrix = mat4(1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, 1.0);
	highp mat4 worldMatrix = mat4(1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
#else
	highp mat4 projectionViewMatrix = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, 1.0);
	highp mat4 worldMatrix = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
#endif
	gl_Position = projectionViewMatrix * worldMatrix * vec4(inPosition.xyz, 1.0);
	outTexcoord = inTexcoord0;
}
#endif

#ifdef FRAGMENT_SHADER
precision mediump float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM;
USE_ENGINE_UNIFORM;
USE_DEPTH_TEXTURE_UNIFORM;

// Input
layout (location = 0) in mediump vec2 inTexcoord;

// Output
layout (location = 0) out mediump vec4 outFragColor;

// Descriptor
DESCRIPTOR_SET_MATPASS(8) mediump uniform sampler2D texNoise;

layout(push_constant) uniform PushConstantParam {
	int samples;
	float minSampleRadius;
	float maxSampleRadius;
	float minDepthRange;
	float maxDepthRange;
} Param;

void main()
{
	highp vec3 sample_sphere[64] = vec3[] (
		vec3(0.93400, 0.36000, -0.79600), 
		vec3(0.79200, -0.31600, -0.30000), 
		vec3(0.50000, -0.32400, 0.88400), 
		vec3(-0.98400, -0.53600, 0.02000), 
		vec3(0.41800, 0.64600, 0.79400), 
		vec3(-0.67200, 0.99600, -0.39400), 
		vec3(-0.26200, 0.39200, -0.90400), 
		vec3(0.47600, -0.87000, -0.70200), 
		vec3(-0.97600, -0.59600, -0.73800), 
		vec3(-0.78600, -0.41800, -0.57200), 
		vec3(-0.15400, 0.72200, -0.77600), 
		vec3(-0.10200, 0.04600, 0.23200), 
		vec3(-0.15000, -0.52000, 0.14200), 
		vec3(0.68400, -0.56200, -0.78600), 
		vec3(-0.69400, 0.11800, -0.07600), 
		vec3(0.16200, -0.97000, 0.07600), 
		vec3(-0.21800, -0.02600, 0.81200), 
		vec3(0.50800, 0.50000, -0.32400), 
		vec3(-0.90200, 0.01400, -0.21800), 
		vec3(0.25600, -0.61800, 0.86800), 
		vec3(-0.27200, 0.01600, 0.96200), 
		vec3(0.22200, 0.83000, -0.63000), 
		vec3(0.92600, 0.78800, 0.83200), 
		vec3(0.33000, 0.61600, 0.44000), 
		vec3(0.64400, 0.30800, 0.87400), 
		vec3(-0.77600, 0.06000, -0.04600), 
		vec3(0.88400, -0.63000, -0.21800), 
		vec3(0.94000, -0.48200, -0.07600), 
		vec3(0.36000, -0.62000, -0.20400), 
		vec3(-0.17800, 0.35000, 0.55600), 
		vec3(0.97600, -0.76600, 0.65600), 
		vec3(0.92200, 0.69800, -0.70200), 
		vec3(0.42800, -0.68000, 0.31000), 
		vec3(0.98600, -0.71400, 0.52200), 
		vec3(-0.04400, -0.01600, -0.38200), 
		vec3(-0.53200, -0.66800, 0.36000), 
		vec3(-0.65000, 0.96600, 0.37800), 
		vec3(0.47200, 0.80400, 0.39000), 
		vec3(0.11200, 0.86000, 0.85200), 
		vec3(0.51400, 0.82200, -0.15800), 
		vec3(0.21800, 0.37000, 0.23800), 
		vec3(0.72800, -0.96600, 0.97400), 
		vec3(0.94400, 0.38200, 0.49400), 
		vec3(0.98000, -0.82600, -0.18000), 
		vec3(-0.62800, -0.35600, -0.67000), 
		vec3(0.05400, -0.38800, 0.72400), 
		vec3(0.82400, -0.66800, 0.15000), 
		vec3(-0.69000, -0.12400, -0.40800), 
		vec3(0.32200, -0.31400, 0.28800), 
		vec3(0.93600, -0.95000, 0.71400), 
		vec3(-0.94200, -0.18000, -0.07400), 
		vec3(-0.76600, 0.33400, -0.38200), 
		vec3(-0.49400, 0.17200, 0.54600), 
		vec3(0.06800, 0.10400, 0.44600), 
		vec3(0.53800, -0.93800, -0.92400), 
		vec3(0.44600, 0.72000, 0.93600), 
		vec3(0.60800, -0.41600, 0.61600), 
		vec3(-0.19000, 0.31400, -0.51800), 
		vec3(0.56200, 0.16600, 0.07600), 
		vec3(0.73200, 0.86400, -0.37600), 
		vec3(0.45400, 0.92000, 0.66200), 
		vec3(0.12200, 0.75600, -0.30800), 
		vec3(0.01800, 0.96400, -0.39400), 
		vec3(-0.16800, -0.51000, -0.43000));

	highp vec2 noiseSize = vec2(textureSize(texNoise, 0));
	highp vec2 depthSize = vec2(textureSize(texDepth, 0));
	highp vec2 noiseTexcoord = 10.0 * (inTexcoord + PoissonDisk(vec3(inTexcoord, 0.0), 0)) * depthSize / noiseSize;

	highp float curDepth = texture(texDepth, inTexcoord).r;
	highp vec3 curPosition = ScreenToViewPosition(inTexcoord, curDepth, cameraProjectionInverseMatrix).xyz;
#ifdef _VULKAN_
	highp vec3 curNormal = normalize(cross(dFdy(curPosition), dFdx(curPosition)));
#else
	highp vec3 curNormal = normalize(cross(dFdx(curPosition), dFdy(curPosition)));
#endif
	highp vec3 curReflect = normalize(texture(texNoise, noiseTexcoord).xyz * 2.0 - 1.0);

	highp int samples = clamp(Param.samples, 4, 64);
	highp float minSampleRadius = Param.minSampleRadius;
	highp float maxSampleRadius = Param.maxSampleRadius;
	highp float minDepthRange = Param.minDepthRange;
	highp float maxDepthRange = Param.maxDepthRange;

	highp float radius = mix(minSampleRadius, maxSampleRadius, smoothstep(minDepthRange, maxDepthRange, LinearDepth(curDepth, cameraZNear, cameraZFar) / (cameraZFar - cameraZNear)));
	highp float occlusion = 0.0;

	for (int index = 0; index < samples; index++) {
		highp vec3 sampleNormal = reflect(sample_sphere[index], curReflect);
		highp vec3 samplePosition = curPosition + sign(dot(curNormal, sampleNormal)) * sampleNormal * radius;

		highp vec4 offset = vec4(samplePosition, 1.0);
		offset = cameraProjectionMatrix * offset;
		offset.xy = offset.xy / offset.w;
		offset.xy = offset.xy * 0.5 + 0.5;

		highp float sampleDepth = texture(texDepth, offset.xy).r;
		highp vec3 checkSamplePosition = ScreenToViewPosition(offset.xy, sampleDepth, cameraProjectionInverseMatrix).xyz;
		float checkRange = smoothstep(0.0, 0.1, radius / abs(checkSamplePosition.z - curPosition.z));
		occlusion += step(samplePosition.z, checkSamplePosition.z) * checkRange;
	}

	occlusion = 1.0 - clamp(occlusion / float(samples), 0.0, 1.0);

	outFragColor.rgb = vec3(occlusion);
	outFragColor.a = 1.0;
}

#endif
