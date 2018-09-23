#version 310 es

#ifdef VERTEX_SHADER
precision mediump float;
#include "engine.inc"

USE_VERTEX_ATTRIBUTE_POSITION;
USE_VERTEX_ATTRIBUTE_TEXCOORD0;

USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL0;
USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL1;
USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL2;
USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL3;

layout (location = 0) out mediump vec2 outTexcoord;

void main()
{
	highp mat4 worldMatrix = mat4(inInstanceTransformMatrixCol0, inInstanceTransformMatrixCol1, inInstanceTransformMatrixCol2, inInstanceTransformMatrixCol3);
	highp vec3 worldPosition = (worldMatrix * vec4(inPosition.xyz, 1.0)).xyz;

	outTexcoord = inTexcoord0;
	gl_Position = cameraProjectionViewMatrix * vec4(worldPosition, 1.0);
}
#endif

#ifdef FRAGMENT_SHADER
precision mediump float;
#include "engine.inc"

uniform sampler2D texDiffuse;

layout (location = 0) in mediump vec2 inTexcoord;
layout (location = 0) out vec4 outFragColor;

void main()
{
	outFragColor = texture(texDiffuse, inTexcoord);
}
#endif