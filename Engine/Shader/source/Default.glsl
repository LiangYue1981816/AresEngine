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

layout (location = 0) out vec2 outTexcoord;

void main()
{
	mat4 worldMatrix = mat4(inInstanceTransformMatrixCol0, inInstanceTransformMatrixCol1, inInstanceTransformMatrixCol2, inInstanceTransformMatrixCol3);
	vec3 worldPosition = (worldMatrix * vec4(inPosition.xyz, 1.0)).xyz;

	outTexcoord = inTexcoord0;
	gl_Position = cameraProjectionViewMatrix * vec4(worldPosition, 1.0);
}
#endif

#ifdef FRAGMENT_SHADER
precision mediump float;
#include "engine.inc"

uniform sampler2D texDiffuse;

layout (location = 0) in vec2 inTexcoord;
layout (location = 0) out vec4 outFragColor;

void main()
{
	outFragColor = texture(texDiffuse, inTexcoord);
}
#endif