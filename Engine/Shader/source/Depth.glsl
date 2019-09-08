#version 310 es

#ifdef VERTEX_SHADER
precision mediump float;
#include "engine.inc"


// VERTEX_ATTRIBUTE_POSITION;
// INSTANCE_ATTRIBUTE_TRANSFORM;


void main()
{
	highp mat4 worldMatrix = mat4(inInstanceTransformMatrixCol0, inInstanceTransformMatrixCol1, inInstanceTransformMatrixCol2, inInstanceTransformMatrixCol3);
	gl_Position = cameraProjectionViewMatrix * worldMatrix * vec4(inPosition.xyz, 1.0);
}
#endif

#ifdef FRAGMENT_SHADER
precision mediump float;
#include "engine.inc"


// Descriptor
DESCRIPTOR_SET_MATPASS(8) mediump uniform sampler2D texAlbedo;


void main()
{
	mediump vec4 color = texture(texAlbedo, inTexcoord);

	if (color.a < 0.5)
		discard;
}
#endif
