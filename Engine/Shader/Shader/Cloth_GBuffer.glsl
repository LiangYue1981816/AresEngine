#version 310 es

#ifdef VERTEX_SHADER
precision highp float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM
USE_ENGINE_UNIFORM
USE_SCENE_DATA_STORAGE

// Output
layout (location = 0) out vec3 outPosition;
layout (location = 1) out vec2 outTexcoord;
#ifdef NORMAL_MAP
layout (location = 2) out vec3 outTangent;
layout (location = 3) out vec3 outBinormal;
layout (location = 4) out vec3 outNormal;
#else
layout (location = 2) out vec3 outNormal;
#endif

// Descriptor
// ...

void main()
{
	mat4 worldMatrix = GetInstance(int(inInstanceIndex)).transformMatrix;
	vec3 worldPosition = (worldMatrix * vec4(inPosition.xyz, 1.0)).xyz;

#ifdef NORMAL_MAP
	vec3 worldNormal = normalize((worldMatrix * vec4(inNormal.xyz, 0.0f)).xyz);
	vec3 worldBinormal = normalize((worldMatrix * vec4(inBinormal.xyz, 0.0f)).xyz);
	vec3 worldTangent = cross(worldBinormal, worldNormal) * inBinormal.w;
	worldBinormal = cross(worldNormal, worldTangent) * inBinormal.w;
	outNormal = worldNormal;
	outTangent = worldTangent;
	outBinormal = worldBinormal;
#else
	vec3 worldNormal = normalize((worldMatrix * vec4(inNormal.xyz, 0.0f)).xyz);
	outNormal = worldNormal;
#endif

	outPosition = worldPosition;
	outTexcoord = inTexcoord0;

	gl_Position = cameraProjectionViewMatrix * vec4(worldPosition, 1.0);
}
#endif

#ifdef FRAGMENT_SHADER
precision highp float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM
USE_ENGINE_UNIFORM
USE_SSAO_TEXTURE_UNIFORM
USE_SHADOW_TEXTURE_UNIFORM

#include "lighting.inc"
#include "shadow.inc"

// Input
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexcoord;
#ifdef NORMAL_MAP
layout (location = 2) in vec3 inTangent;
layout (location = 3) in vec3 inBinormal;
layout (location = 4) in vec3 inNormal;
#else
layout (location = 2) in vec3 inNormal;
#endif

// Output
layout (location = 0) out vec4 outFragColor;
layout (location = 1) out vec4 outFragGBuffer0;
layout (location = 2) out vec4 outFragGBuffer1;

// Descriptor
DESCRIPTOR_SET_MATPASS(8) uniform sampler2D texAlbedo;
#ifdef NORMAL_MAP
DESCRIPTOR_SET_MATPASS(9) uniform sampler2D texNormal;
#endif
#ifdef ROUGHNESS_METALLIC_SPECULAR_AO_MAP
DESCRIPTOR_SET_MATPASS(10) uniform sampler2D texRoughnessMetallicSpecularAO;
#endif
#ifdef ENV_MAP
DESCRIPTOR_SET_MATPASS(11) uniform sampler2D texEnv;
#endif

void main()
{
	vec4 albedo = texture(texAlbedo, inTexcoord);

	if (albedo.a < 0.5)
		discard;

	vec3 albedoColor = Gamma2Linear(albedo.rgb);

	vec3 worldCameraPosition = (cameraViewInverseMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	vec3 worldViewDirection = normalize(worldCameraPosition - inPosition);

#ifdef NORMAL_MAP
	vec3 worldNormal = mat3(inTangent, inBinormal, inNormal) * (texture(texNormal, inTexcoord).rgb * 2.0 - 1.0);
#else
	vec3 worldNormal = inNormal;
#endif
	worldNormal = normalize(worldNormal);

#ifdef ROUGHNESS_METALLIC_SPECULAR_AO_MAP
	vec4 roughness_metallic_specular_ao = texture(texRoughnessMetallicSpecularAO, inTexcoord);
	float roughness = roughness_metallic_specular_ao.r;
	float metallic = roughness_metallic_specular_ao.g;
	float specular = roughness_metallic_specular_ao.b;
	float ao = roughness_metallic_specular_ao.a;
#else
	float roughness = 0.5;
	float metallic = 0.5;
	float specular = 0.5;
	float ao = 1.0;
#endif

	vec4 projectCoord = cameraProjectionViewMatrix * vec4(inPosition, 1.0);
	projectCoord.xy = projectCoord.xy / projectCoord.w;
 	projectCoord.xy = projectCoord.xy * 0.5 + 0.5;
	float ssao = texture(texSSAO, projectCoord.xy).r;

	float shadow = ShadowValue(inPosition, inNormal, texShadow);
//	float shadow = ShadowValueIrregular(inPosition, texShadow);

	vec3 pointLightDirection = mainPointLightPosition - inPosition;
	vec3 pointLightColor = mainPointLightColor * LightingAttenuation(length(pointLightDirection));
	pointLightDirection = normalize(pointLightDirection);

	vec3 ambientLighting = AmbientSH9(worldNormal, albedoColor, metallic) * ambientLightFactor;
	vec3 pointLighting = ClothLighting(worldNormal, worldViewDirection, pointLightDirection, pointLightColor, albedoColor, specular, roughness) * pointLightFactor;
	vec3 directLighting = ClothLighting(worldNormal, worldViewDirection, mainDirectLightDirection, mainDirectLightColor, albedoColor, specular, roughness) * directLightFactor;
#ifdef ENV_MAP
	vec3 envLighting = EnvLighting(worldNormal, worldViewDirection, vec3(1.0), specular, metallic, roughness, texEnv, 8.0) * envLightFactor;
#else
	vec3 envLighting = vec3(0.0);
#endif
	vec3 finalLighting = ao * ssao * (ambientLighting * (vec3(1.0) + envLighting) + pointLighting + directLighting * shadow);

//	Debug Shadow
//	float factor = length(worldCameraPosition - inPosition) / (cameraZFar - cameraZNear);
//	if (factor < mainShadowLevelFactor.w) finalLighting = vec3(1.0, 1.0, 1.0) * vec3(shadow);
//	if (factor < mainShadowLevelFactor.z) finalLighting = vec3(0.0, 0.0, 1.0) * vec3(shadow);
//	if (factor < mainShadowLevelFactor.y) finalLighting = vec3(0.0, 1.0, 0.0) * vec3(shadow);
//	if (factor < mainShadowLevelFactor.x) finalLighting = vec3(1.0, 0.0, 0.0) * vec3(shadow);
//	finalLighting = vec3(shadow);

	outFragColor = PackHDR(finalLighting);

	outFragGBuffer0.rgb = Gamma2Linear(albedo.rgb);
	outFragGBuffer0.a = ao * ssao;

	outFragGBuffer1.rg = NormalEncode(worldNormal);
	outFragGBuffer1.b = roughness;
	outFragGBuffer1.a = metallic;
}
#endif
