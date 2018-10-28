#version 310 es

#ifdef VERTEX_SHADER
precision mediump float;
#include "engine.inc"

USE_VERTEX_ATTRIBUTE_POSITION;

USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL0;
USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL1;
USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL2;
USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL3;

void main()
{
	highp mat4 worldMatrix = mat4(inInstanceTransformMatrixCol0, inInstanceTransformMatrixCol1, inInstanceTransformMatrixCol2, inInstanceTransformMatrixCol3);
	gl_Position = cameraProjectionViewMatrix * worldMatrix * vec4(inPosition.xyz, 1.0);
}
#endif

#ifdef FRAGMENT_SHADER
precision mediump float;

void main()
{

}
#endif