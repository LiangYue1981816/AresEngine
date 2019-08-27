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
	highp mat4 worldMatrix = mat4(inInstanceTransformMatrixCol0, inInstanceTransformMatrixCol1, inInstanceTransformMatrixCol2, inInstanceTransformMatrixCol3);
	gl_Position = cameraProjectionViewMatrix * worldMatrix * vec4(inPosition.xyz, 1.0);
	outTexcoord = inTexcoord0;
}
#endif

#ifdef FRAGMENT_SHADER
precision mediump float;
#include "engine.inc"
#include "shadow.inc"


// Input
layout (location = 0) in mediump vec2 inTexcoord;

// Output
layout (location = 0) out mediump vec4 outFragColor;

// Descriptor
USE_SHADOWMAP_UNIFORM;


void main()
{
	highp float resolution = 2048.0;
	highp float depth = 0.0;
//	depth = LinearShadowDepth(texShadowMap, inTexcoord, resolution);
	depth += UnpackFloat(texture(texShadowMap, inTexcoord + vec2(-1.0f, -1.0f) / resolution)) * 1.0 / 16.0;
	depth += UnpackFloat(texture(texShadowMap, inTexcoord + vec2( 0.0f, -1.0f) / resolution)) * 2.0 / 16.0;
	depth += UnpackFloat(texture(texShadowMap, inTexcoord + vec2( 1.0f, -1.0f) / resolution)) * 1.0 / 16.0;
	depth += UnpackFloat(texture(texShadowMap, inTexcoord + vec2(-1.0f,  0.0f) / resolution)) * 2.0 / 16.0;
	depth += UnpackFloat(texture(texShadowMap, inTexcoord + vec2( 0.0f,  0.0f) / resolution)) * 4.0 / 16.0;
	depth += UnpackFloat(texture(texShadowMap, inTexcoord + vec2( 1.0f,  0.0f) / resolution)) * 2.0 / 16.0;
	depth += UnpackFloat(texture(texShadowMap, inTexcoord + vec2(-1.0f,  1.0f) / resolution)) * 1.0 / 16.0;
	depth += UnpackFloat(texture(texShadowMap, inTexcoord + vec2( 0.0f,  1.0f) / resolution)) * 2.0 / 16.0;
	depth += UnpackFloat(texture(texShadowMap, inTexcoord + vec2( 1.0f,  1.0f) / resolution)) * 1.0 / 16.0;
	outFragColor = PackFloat(depth);
}
#endif
