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
#ifdef _VULKAN_
	highp mat4 projectionViewMatrix = mat4(1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0,-0.5, 0.0, 0.0, 0.0, 1.0, 1.0);
#else
	highp mat4 projectionViewMatrix = mat4(1.0, 0.0, 0.0, 0.0, 0.0,  1.0, 0.0, 0.0, 0.0, 0.0,-0.5, 0.0, 0.0, 0.0, 1.0, 1.0);
#endif
	highp mat4 worldMatrix = mat4(inInstanceTransformMatrixCol0, inInstanceTransformMatrixCol1, inInstanceTransformMatrixCol2, inInstanceTransformMatrixCol3);
	gl_Position = projectionViewMatrix * worldMatrix * vec4(inPosition.xyz, 1.0);
	outTexcoord = inTexcoord0;
}
#endif

#ifdef FRAGMENT_SHADER
precision mediump float;
#include "engine.inc"


// Input
layout (location = 0) in mediump vec2 inTexcoord;

// Output
layout (location = 0) out mediump vec4 outFragColor;

// Descriptor
USE_COLOR_TEXTURE_UNIFORM;

layout(push_constant) uniform PushConstantParam {
	float range;
} Param;


void main()
{
	highp vec2 range = vec2(Param.range);

	highp float offsets[5] = float[](0.0, 1.0, 2.0, 3.0, 4.0);
	highp float weights[5] = float[](0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

	highp vec2 texelSize = range / vec2(textureSize(texColor, 0));
	highp vec3 result = texture(texColor, inTexcoord).rgb * weights[0];

	for (int i = 1; i < 5; i++) {
		highp vec2 offset = vec2(0.0, offsets[i]) * texelSize;
		result += texture(texColor, inTexcoord + offset).rgb * weights[i];
		result += texture(texColor, inTexcoord - offset).rgb * weights[i];
	}

	outFragColor.rgb = result;
	outFragColor.a = 1.0;
}
#endif
