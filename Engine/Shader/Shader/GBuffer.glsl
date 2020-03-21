#version 310 es

#ifdef VERTEX_SHADER
precision mediump float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM;
USE_ENGINE_UNIFORM;
USE_SCENE_DATA_STORAGE;

// Output
layout (location = 0) out mediump vec2 outTexcoord;
#ifdef NORMAL_MAP
layout (location = 1) out mediump vec3 outTangent;
layout (location = 2) out mediump vec3 outBinormal;
layout (location = 3) out mediump vec3 outNormal;
#else
layout (location = 1) out mediump vec3 outNormal;
#endif

// Descriptor
// ...

void main()
{
	highp mat4 worldMatrix = sceneData.data[inInstanceIndex].transformMatrix;
	highp vec3 worldPosition = (worldMatrix * vec4(inPosition.xyz, 1.0)).xyz;

#ifdef NORMAL_MAP
	mediump vec3 worldNormal = normalize((worldMatrix * vec4(inNormal, 0.0f)).xyz);
	mediump vec3 worldBinormal = normalize((worldMatrix * vec4(inBinormal, 0.0f)).xyz);
	mediump vec3 worldTangent = cross(worldBinormal, worldNormal);
	worldBinormal = cross(worldNormal, worldTangent);

	outTangent = worldTangent;
	outBinormal = worldBinormal;
	outNormal = worldNormal;
#else
	mediump vec3 worldNormal = normalize((worldMatrix * vec4(inNormal, 0.0f)).xyz);
	outNormal = worldNormal;
#endif

	outTexcoord = inTexcoord0;

	gl_Position = cameraProjectionViewMatrix * vec4(worldPosition, 1.0);
}
#endif

#ifdef FRAGMENT_SHADER
precision mediump float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM;
USE_ENGINE_UNIFORM;

// Input
layout (location = 0) in mediump vec2 inTexcoord;
#ifdef NORMAL_MAP
layout (location = 1) in mediump vec3 inTangent;
layout (location = 2) in mediump vec3 inBinormal;
layout (location = 3) in mediump vec3 inNormal;
#else
layout (location = 1) in mediump vec3 inNormal;
#endif

// Output
layout (location = 0) out mediump vec4 outFragGBufferA;
layout (location = 0) out mediump vec4 outFragGBufferB;
layout (location = 0) out mediump vec4 outFragGBufferC;

// Descriptor
DESCRIPTOR_SET_MATPASS(8) mediump uniform sampler2D texAlbedo;
#ifdef NORMAL_MAP
DESCRIPTOR_SET_MATPASS(9) mediump uniform sampler2D texNormal;
#endif
#ifdef ROUGHNESS_METALLIC_SPECULAR_AO_MAP
DESCRIPTOR_SET_MATPASS(10) mediump uniform sampler2D texRoughnessMetallicSpecularAO;
#endif

void main()
{
	mediump vec4 color = texture(texAlbedo, inTexcoord);

	if (color.a < 0.5)
		discard;

	outFragGBufferA.rgb = Gamma2Linear(albedo.rgb);
	outFragGBufferA.a = 1.0;

#ifdef NORMAL_MAP
	mediump vec3 worldNormal = normalize(mat3(inTangent, inBinormal, inNormal) * (texture(texNormal, inTexcoord).rgb * vec3(2.0) - vec3(1.0)));
#else
	mediump vec3 worldNormal = inNormal;
#endif

	outFragGBufferB.rgb = worldNormal;
	outFragGBufferB.a = 1.0;

#ifdef ROUGHNESS_METALLIC_SPECULAR_AO_MAP
	mediump vec4 roughness_metallic_specular_ao = texture(texRoughnessMetallicSpecularAO, inTexcoord);
	mediump float roughness = roughness_metallic_specular_ao.r;
	mediump float metallic = roughness_metallic_specular_ao.g;
	mediump float specular = roughness_metallic_specular_ao.b;
	mediump float ao = roughness_metallic_specular_ao.a;
#else
	mediump float roughness = 0.5;
	mediump float metallic = 0.5;
	mediump float specular = 1.0;
	mediump float ao = 1.0;
#endif

	outFragGBufferC = vec4(roughness, metallic, specular, ao);
}
#endif
