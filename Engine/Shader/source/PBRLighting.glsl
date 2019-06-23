#version 310 es

#ifdef VERTEX_SHADER
precision mediump float;
#include "engine.inc"
#include "light.inc"


// VERTEX_ATTRIBUTE_POSITION;
// VERTEX_ATTRIBUTE_NORMAL;
// VERTEX_ATTRIBUTE_BINORMAL;
// VERTEX_ATTRIBUTE_TEXCOORD0;
// INSTANCE_ATTRIBUTE_TRANSFORM;

// Output
layout (location = 0) out highp   vec3 outPosition;
layout (location = 1) out mediump vec2 outTexcoord;
#ifdef NORMAL_MAP
layout (location = 2) out mediump mat3 outTBN;
#else
layout (location = 2) out mediump vec3 outNormal;
#endif


void main()
{
	highp mat4 worldMatrix = mat4(inInstanceTransformMatrixCol0, inInstanceTransformMatrixCol1, inInstanceTransformMatrixCol2, inInstanceTransformMatrixCol3);
	highp vec3 worldPosition = (worldMatrix * vec4(inPosition.xyz, 1.0)).xyz;

	mediump vec3 worldNormal = (worldMatrix * vec4(inNormal, 0.0f)).xyz;
	worldNormal = normalize(worldNormal);

#ifdef NORMAL_MAP
	mediump vec3 worldBinormal = (worldMatrix * vec4(inBinormal, 0.0f)).xyz;
	worldBinormal = normalize(worldBinormal);

	mediump vec3 t = cross(worldBinormal, worldNormal);
	mediump vec3 b = cross(worldNormal, t);
	mediump mat3 tbn = mat3(t, b, worldNormal);
#endif

	outPosition = worldPosition;
	outTexcoord = inTexcoord0;
	outHalfDirection = worldHalfDirection;
	outViewDirection = worldViewDirection;

#ifdef NORMAL_MAP
	outTBN = tbn;
#else
	outNormal = worldNormal;
#endif

	gl_Position = cameraProjectionViewMatrix * vec4(worldPosition, 1.0);
}
#endif

#ifdef FRAGMENT_SHADER
precision mediump float;
#include "engine.inc"
#include "light.inc"


// Output
layout (location = 0) out mediump vec4 outFragColor;

// Input
layout (location = 0) in highp   vec3 inPosition;
layout (location = 1) in mediump vec2 inTexcoord;
#ifdef NORMAL_MAP
layout (location = 2) in mediump mat3 inTBN;
#else
layout (location = 2) in mediump vec3 inNormal;
#endif

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
	mediump vec4 albedo = texture(texAlbedo, inTexcoord);

#ifdef ALPHA_TEST
	if (albedo.a < 0.5)
		discard;
#endif

	highp vec3 worldCameraPosition = (cameraViewInverseMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	mediump vec3 worldViewDirection = normalize((worldCameraPosition - inPosition);
	mediump vec3 worldHalfDirection = normalize(mainDirectLightDirection + worldViewDirection);

#ifdef NORMAL_MAP
	mediump vec3 worldNormal = normalize(inTBN * (texture(texNormal, inTexcoord).rgb * vec3(2.0) - vec3(1.0)));
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
	mediump float specular = 1.0;
	mediump float ao = 1.0;
#endif
	mediump float shadow = 1.0;

	mediump vec3 pointLightDirection = mainPointLightPosition - inPosition;
	mediump vec3 pointLightColor = mainPointLightColor * LightingAttenuation(length(pointLightDirection));
	pointLightDirection = normalize(pointLightDirection);

	mediump vec3 aoColor = vec3(ao);
	mediump vec3 albedoColor = Gamma2Linear(albedo.rgb);
	mediump vec3 ambientLightingColor = AmbientSH9(worldNormal, albedoColor, metallic) * ambientLightFactor;
	mediump vec3 pointLightingColor = PbrLighting(worldNormal, worldViewDirection, worldHalfDirection, pointLightDirection, pointLightColor, albedoColor, metallic, roughness) * pointLightFactor;
	mediump vec3 directLightingColor = PbrLighting(worldNormal, worldViewDirection, worldHalfDirection, mainDirectLightDirection, mainDirectLightColor, albedoColor, metallic, roughness) * directLightFactor;
#ifdef ENV_MAP
	mediump vec3 envLightingColor = EnvLighting(worldNormal, worldViewDirection, albedoColor, texEnv, 8.0, metallic, roughness) * envLightFactor;
#else
	mediump vec3 envLightingColor = EnvLighting(worldNormal, worldViewDirection, albedoColor, ambientLightingColor, metallic, roughness) * envLightFactor;
#endif
	mediump vec3 finalLighting = aoColor * (ambientLightingColor + pointLightingColor + directLightingColor * shadow + envLightingColor);

	finalLighting = ToneMapping(finalLighting);
	finalLighting = Linear2Gamma(finalLighting);

	outFragColor.rgb = finalLighting;
	outFragColor.a = 1.0;
}
#endif
