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
	highp vec3 worldPosition = (worldMatrix * vec4(inPosition.xyz, 1.0)).xyz;

	outTexcoord = inTexcoord0;
	gl_Position = cameraProjectionViewMatrix * vec4(worldPosition, 1.0);
}
#endif

#ifdef FRAGMENT_SHADER
precision mediump float;
#include "engine.inc"


// Input
layout (location = 0) in mediump vec2 inTexcoord;

// Output
layout (location = 0) out vec4 outFragColor;

// Descriptor
DESCRIPTOR_SET_MATPASS(8) uniform sampler2D texAlbedo;


void main()
{
	vec4 color = texture(texAlbedo, inTexcoord);

#ifdef ALPHA_TEST
	if (color.a < 0.5)
		discard;
#endif

	outFragColor.rgb = color.rgb;
	outFragColor.a = 1.0;
}
#endif
