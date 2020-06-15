#version 310 es

#ifdef VERTEX_SHADER
precision mediump float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM
USE_ENGINE_UNIFORM
USE_SCENE_DATA_STORAGE

// Output
layout (location = 0) out highp   vec3 outPosition;
layout (location = 1) out mediump vec2 outTexcoord;
#ifdef NORMAL_MAP
layout (location = 2) out mediump vec3 outTangent;
layout (location = 3) out mediump vec3 outBinormal;
layout (location = 4) out mediump vec3 outNormal;
#else
layout (location = 2) out mediump vec3 outNormal;
#endif

// Descriptor
// ...

void main()
{
	highp mat4 worldMatrix = GetInstance(int(inInstanceIndex)).transformMatrix;
	highp vec3 worldPosition = (worldMatrix * vec4(inPosition.xyz, 1.0)).xyz;

#ifdef NORMAL_MAP
	mediump vec3 worldNormal = normalize((worldMatrix * vec4(inNormal.xyz, 0.0f)).xyz);
	mediump vec3 worldBinormal = normalize((worldMatrix * vec4(inBinormal.xyz, 0.0f)).xyz);
	mediump vec3 worldTangent = cross(worldBinormal, worldNormal) * inBinormal.w;
	worldBinormal = cross(worldNormal, worldTangent) * inBinormal.w;

	outTangent = worldTangent;
	outBinormal = worldBinormal;
	outNormal = worldNormal;
#else
	mediump vec3 worldNormal = normalize((worldMatrix * vec4(inNormal.xyz, 0.0f)).xyz);
	outNormal = worldNormal;
#endif

	outPosition = worldPosition;
	outTexcoord = inTexcoord0;

	gl_Position = cameraProjectionViewMatrix * vec4(worldPosition, 1.0);
}
#endif

#ifdef FRAGMENT_SHADER
precision mediump float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM
USE_ENGINE_UNIFORM
USE_SSAO_TEXTURE_UNIFORM
USE_SHADOW_TEXTURE_UNIFORM

#include "lighting.inc"
#include "shadow.inc"

// Input
layout (location = 0) in highp   vec3 inPosition;
layout (location = 1) in mediump vec2 inTexcoord;
#ifdef NORMAL_MAP
layout (location = 2) in mediump vec3 inTangent;
layout (location = 3) in mediump vec3 inBinormal;
layout (location = 4) in mediump vec3 inNormal;
#else
layout (location = 2) in mediump vec3 inNormal;
#endif

// Output
layout (location = 0) out mediump vec4 outFragColor;

// Descriptor
DESCRIPTOR_SET_MATPASS(8) mediump uniform sampler2D texAlbedo;
#ifdef NORMAL_MAP
DESCRIPTOR_SET_MATPASS(9) mediump uniform sampler2D texNormal;
#endif
#ifdef ROUGHNESS_METALLIC_SPECULAR_AO_MAP
DESCRIPTOR_SET_MATPASS(10) mediump uniform sampler2D texRoughnessMetallicSpecularAO;
#endif
#ifdef ENV_MAP
DESCRIPTOR_SET_MATPASS(11) mediump uniform sampler2D texEnv;
#endif

void main()
{
	mediump vec4 albedo = texture(texAlbedo, inTexcoord);

	if (albedo.a < 0.5)
		discard;

	mediump vec3 albedoColor = Gamma2Linear(albedo.rgb);

	highp vec3 worldCameraPosition = (cameraViewInverseMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	mediump vec3 worldViewDirection = normalize(worldCameraPosition - inPosition);

#ifdef NORMAL_MAP
	mediump vec3 worldNormal = normalize(mat3(inTangent, inBinormal, inNormal) * (texture(texNormal, inTexcoord).rgb * 2.0 - 1.0));
#else
	mediump vec3 worldNormal = inNormal;
#endif

#ifdef ROUGHNESS_METALLIC_SPECULAR_AO_MAP
	mediump vec4 roughness_metallic_specular_ao = texture(texRoughnessMetallicSpecularAO, inTexcoord);
	mediump float roughness = roughness_metallic_specular_ao.r;
	mediump float metallic = roughness_metallic_specular_ao.g;
	mediump float specular = roughness_metallic_specular_ao.b;
	mediump float ao = roughness_metallic_specular_ao.a;
#else
	mediump float roughness = 0.5;
	mediump float metallic = 0.5;
	mediump float specular = 0.5;
	mediump float ao = 1.0;
#endif

	highp vec4 projectCoord = cameraProjectionViewMatrix * vec4(inPosition, 1.0);
	projectCoord.xy = projectCoord.xy / projectCoord.w;
 	projectCoord.xy = projectCoord.xy * 0.5 + 0.5;
	mediump float ssao = texture(texSSAO, projectCoord.xy).r;

	mediump float shadow = ShadowValue(inPosition, inNormal, texShadow);
//	mediump float shadow = ShadowValueIrregular(inPosition, texShadow);

	mediump vec3 pointLightDirection = mainPointLightPosition - inPosition;
	mediump vec3 pointLightColor = mainPointLightColor * LightingAttenuation(length(pointLightDirection));
	pointLightDirection = normalize(pointLightDirection);

	mediump vec3 ambientLighting = AmbientSH9(worldNormal, albedoColor, metallic) * ambientLightFactor;
	mediump vec3 pointLighting = ClothLighting(worldNormal, worldViewDirection, pointLightDirection, pointLightColor, albedoColor, specular, roughness) * pointLightFactor;
	mediump vec3 directLighting = ClothLighting(worldNormal, worldViewDirection, mainDirectLightDirection, mainDirectLightColor, albedoColor, specular, roughness) * directLightFactor;
#ifdef ENV_MAP
	mediump vec3 envLighting = EnvLighting(worldNormal, worldViewDirection, vec3(1.0), specular, metallic, roughness, texEnv, 8.0) * envLightFactor;
#else
	mediump vec3 envLighting = vec3(0.0);
#endif
	mediump vec3 finalLighting = ao * ssao * (ambientLighting * (vec3(1.0) + envLighting) + pointLighting + directLighting * shadow);

//	Debug Shadow
//	highp float factor = length(worldCameraPosition - inPosition) / (cameraZFar - cameraZNear);
//	if (factor < mainShadowLevelFactor.w) finalLighting = vec3(1.0, 1.0, 1.0) * vec3(shadow);
//	if (factor < mainShadowLevelFactor.z) finalLighting = vec3(0.0, 0.0, 1.0) * vec3(shadow);
//	if (factor < mainShadowLevelFactor.y) finalLighting = vec3(0.0, 1.0, 0.0) * vec3(shadow);
//	if (factor < mainShadowLevelFactor.x) finalLighting = vec3(1.0, 0.0, 0.0) * vec3(shadow);
//	finalLighting = vec3(shadow);

	outFragColor = PackHDR(finalLighting);
}
#endif
