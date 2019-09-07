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


void main()
{
	highp vec3 sample_sphere[16] = vec3[] (
		vec3( 0.5381, 0.1856,-0.4319), vec3( 0.1379, 0.2486, 0.4430),
		vec3( 0.3371, 0.5679,-0.0057), vec3(-0.6999,-0.0451,-0.0019),
		vec3( 0.0689,-0.1598,-0.8547), vec3( 0.0560, 0.0069,-0.1843),
		vec3(-0.0146, 0.1402, 0.0762), vec3( 0.0100,-0.1924,-0.0344),
		vec3(-0.3577,-0.5301,-0.4358), vec3(-0.3169, 0.1063, 0.0158),
		vec3( 0.0103,-0.5869, 0.0046), vec3(-0.0897,-0.4940, 0.3287),
		vec3( 0.7119,-0.0154,-0.0918), vec3(-0.0533, 0.0596,-0.5411),
		vec3( 0.0352,-0.0631, 0.5460), vec3(-0.4776, 0.2847,-0.0271));

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
