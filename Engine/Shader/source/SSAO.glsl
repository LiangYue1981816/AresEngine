#version 310 es

#ifdef VERTEX_SHADER
precision mediump float;
#include "engine.inc"


// VERTEX_ATTRIBUTE_POSITION;
// VERTEX_ATTRIBUTE_TEXCOORD0;
// INSTANCE_ATTRIBUTE_TRANSFORM;

// Output
layout (location = 0) out mediump vec2 outTexcoord;


void main()
{
	highp mat4 projectionViewMatrix = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,-0.5, 0.0, 0.0, 0.0, 1.0, 1.0);
	highp mat4 worldMatrix = mat4(inInstanceTransformMatrixCol0, inInstanceTransformMatrixCol1, inInstanceTransformMatrixCol2, inInstanceTransformMatrixCol3);
	gl_Position = projectionViewMatrix * worldMatrix * vec4(inPosition.xyz, 1.0);
	outTexcoord = inTexcoord0;
}
#endif

#ifdef FRAGMENT_SHADER
precision mediump float;
#include "engine.inc"
#include "common.inc"


// Input
layout (location = 0) in mediump vec2 inTexcoord;

// Output
layout (location = 0) out mediump vec4 outFragColor;

// Descriptor
USE_DEPTH_TEXTURE_UNIFORM;
USE_COLOR_TEXTURE_UNIFORM;

DESCRIPTOR_SET_MATPASS(8) highp uniform sampler2D texNoise;


void main()
{
	highp vec3 sample_sphere[16] = vec3[] (
		vec3( 0.89000,  0.72200,  0.40800), 
		vec3( 0.19400, -0.08400, -0.57400), 
		vec3(-0.14600, -0.29200,  0.55200), 
		vec3(-0.84200,  0.63400, -0.83400), 
		vec3( 0.21200, -0.67800, -0.37800), 
		vec3( 0.52200, -0.51000, -0.31400), 
		vec3(-0.94200, -0.46400, -0.99400), 
		vec3( 0.62000, -0.02000,  0.63200), 
		vec3(-0.16600,  0.38200, -0.95200), 
		vec3( 0.43400, -0.49600, -0.11800), 
		vec3(-0.47200,  0.40600, -0.64200), 
		vec3( 0.93400,  0.09000, -0.87600), 
		vec3(-0.97800,  0.95400, -0.62000), 
		vec3(-0.95600, -0.20000,  0.71200), 
		vec3( 0.80400, -0.45400,  0.70800), 
		vec3( 0.83000,  0.15200,  0.78200));

	highp float curDepth = texture(texDepth, inTexcoord).r;
	highp vec3 curPosition = ScreenToViewPosition(inTexcoord, curDepth);

	highp float radius = 0.1;
	highp float occlusion = 0.0;

	for (int index = 0; index < 8; index++) {
		highp vec3 normal = normalize(vec3(PoissonDisk(curPosition, index), 0.0));
		highp vec3 samplePosition = curPosition + reflect(sample_sphere[index], normal) * radius;

		highp vec4 offset = vec4(samplePosition, 1.0);
        offset = cameraProjectionMatrix * offset;
        offset.xy = offset.xy / offset.w;
        offset.xy = offset.xy * 0.5 + 0.5;

		highp float sampleDepth = texture(texDepth, offset.xy).r;
		occlusion += step(curDepth, sampleDepth);
	}

	outFragColor.rgb = vec3(occlusion / 8.0);
	outFragColor.a = 1.0;
}

#endif
