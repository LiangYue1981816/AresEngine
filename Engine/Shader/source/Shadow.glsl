#version 310 es

#ifdef VERTEX_SHADER
precision mediump float;
#include "engine.inc"


// VERTEX_ATTRIBUTE_POSITION;
// INSTANCE_ATTRIBUTE_TRANSFORM;

// Output
layout (location = 0) out highp vec3 outPosition;


void main()
{
	highp mat4 worldMatrix = mat4(inInstanceTransformMatrixCol0, inInstanceTransformMatrixCol1, inInstanceTransformMatrixCol2, inInstanceTransformMatrixCol3);
	outPosition = (cameraViewMatrix * worldMatrix * vec4(inPosition.xyz, 1.0)).xyz;
	gl_Position = (cameraProjectionViewMatrix * worldMatrix * vec4(inPosition.xyz, 1.0));
}
#endif

#ifdef FRAGMENT_SHADER
precision mediump float;
#include "engine.inc"
#include "shadow.inc"


// Input
layout (location = 0) in highp vec3 inPosition;

// Output
layout (location = 0) out mediump vec4 outFragColor;


void main()
{
	outFragColor = EncodeFloatRGB(length(inPosition) / (cameraZFar - cameraZNear));
}
#endif
