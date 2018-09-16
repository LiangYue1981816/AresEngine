#version 310 es

#ifdef VERTEX_SHADER
precision mediump float;
#include "engine.inc"
#include "light.inc"

USE_VERTEX_ATTRIBUTE_POSITION;
USE_VERTEX_ATTRIBUTE_NORMAL;
USE_VERTEX_ATTRIBUTE_BINORMAL;
USE_VERTEX_ATTRIBUTE_TEXCOORD0;

USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL0;
USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL1;
USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL2;
USE_INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL3;

layout (location = 0) out vec3 outPosition;
layout (location = 1) out vec2 outTexcoord;
layout (location = 2) out vec3 outHalfDirection;
layout (location = 3) out vec3 outViewDirection;
layout (location = 4) out mat3 outTBN;

void main()
{
	mat4 worldMatrix = mat4(inInstanceTransformMatrixCol0, inInstanceTransformMatrixCol1, inInstanceTransformMatrixCol2, inInstanceTransformMatrixCol3);

	vec3 worldPosition = (worldMatrix * vec4(inPosition.xyz, 1.0)).xyz;
	vec3 worldCameraPosition = (cameraViewInverseMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	vec3 worldViewDirection = worldCameraPosition - worldPosition;
	worldViewDirection = normalize(worldViewDirection);

	vec3 worldHalfDirection = mainDirectLightDirection + worldViewDirection;
	worldHalfDirection = normalize(worldHalfDirection);

	vec3 worldNormal = (worldMatrix * vec4(inNormal, 0.0f)).xyz;
	worldNormal = normalize(worldNormal);

	vec3 worldBinormal = (worldMatrix * vec4(inBinormal, 0.0f)).xyz;
	worldBinormal = normalize(worldBinormal);

	vec3 t = cross(worldBinormal, worldNormal);
	vec3 b = cross(worldNormal, t);
	mat3 tbn = mat3(t, b, worldNormal);

	outTexcoord = inTexcoord0;
	outPosition = worldPosition;
	outHalfDirection = worldHalfDirection;
	outViewDirection = worldViewDirection;
	outTBN = tbn;

	gl_Position = cameraProjectionViewMatrix * vec4(worldPosition, 1.0);
}
#endif

#ifdef FRAGMENT_SHADER
precision mediump float;
#include "engine.inc"
#include "light.inc"

uniform sampler2D texAlbedo;
uniform sampler2D texNormal;
uniform sampler2D texSpecular;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexcoord;
layout (location = 2) in vec3 inHalfDirection;
layout (location = 3) in vec3 inViewDirection;
layout (location = 4) in mat3 inTBN;

layout (location = 0) out vec4 outFragColor;

void main()
{
	vec3 albedoColor = Gamma2Linear(texture(texAlbedo, inTexcoord).rgb);
	vec3 specularColor = texture(texSpecular, inTexcoord).rgb;

	float metallic = 0.7;
	float roughness = 0.3;

	vec3 pixelNormal = texture(texNormal, inTexcoord).rgb * 2.0 - 1.0;
	pixelNormal = normalize(inTBN * pixelNormal);

	vec3 pointLightColor = mainPointLightColor;
	vec3 pointLightDirection = mainPointLightPosition - inPosition;
	pointLightColor = pointLightColor * PointLightAttenuation(length(pointLightDirection));
	pointLightDirection = normalize(pointLightDirection);

	vec3 ambientLightingColor = AmbientLightingSH9(albedoColor, 0.0, pixelNormal) * ambientLightFactor;
	vec3 pointLightingColor = SimpleLighting(pointLightColor, pointLightDirection, pixelNormal, albedoColor) * pointLightFactor;
	vec3 directLightingColor = BlinnLighting(mainDirectLightColor, mainDirectLightDirection, inHalfDirection, inViewDirection, pixelNormal, albedoColor, specularColor, metallic, roughness) * directLightFactor;
	vec3 finalLighting = FinalLighting(vec3(1.0), ambientLightingColor, pointLightingColor, directLightingColor, vec3(0.0), 1.0);

	finalLighting = ToneMapping(finalLighting);
	finalLighting = Linear2Gamma(finalLighting);

	outFragColor.rgb = finalLighting;
	outFragColor.a = 1.0;
}
#endif