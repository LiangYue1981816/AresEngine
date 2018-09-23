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

layout (location = 0) out highp   vec3 outPosition;
layout (location = 1) out mediump vec2 outTexcoord;
layout (location = 2) out mediump vec3 outHalfDirection;
layout (location = 3) out mediump vec3 outViewDirection;
#ifdef NORMAL_MAP
layout (location = 4) out mediump mat3 outTBN;
#else
layout (location = 4) out mediump vec3 outNormal;
#endif

void main()
{
	highp mat4 worldMatrix = mat4(inInstanceTransformMatrixCol0, inInstanceTransformMatrixCol1, inInstanceTransformMatrixCol2, inInstanceTransformMatrixCol3);
	highp vec3 worldPosition = (worldMatrix * vec4(inPosition.xyz, 1.0)).xyz;
	highp vec3 worldCameraPosition = (cameraViewInverseMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;

	mediump vec3 worldViewDirection = worldCameraPosition - worldPosition;
	worldViewDirection = normalize(worldViewDirection);

	mediump vec3 worldHalfDirection = mainDirectLightDirection + worldViewDirection;
	worldHalfDirection = normalize(worldHalfDirection);

	mediump vec3 worldNormal = (worldMatrix * vec4(inNormal, 0.0f)).xyz;
	worldNormal = normalize(worldNormal);

#ifdef NORMAL_MAP
	mediump vec3 worldBinormal = (worldMatrix * vec4(inBinormal, 0.0f)).xyz;
	worldBinormal = normalize(worldBinormal);

	mediump vec3 t = cross(worldBinormal, worldNormal);
	mediump vec3 b = cross(worldNormal, t);
	mediump mat3 tbn = mat3(t, b, worldNormal);
#endif

	outTexcoord = inTexcoord0;
	outPosition = worldPosition;
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

DESCRIPTOR_SET_PASS(8)  uniform sampler2D texAlbedo;
#ifdef AO_MAP
DESCRIPTOR_SET_PASS(9)  uniform sampler2D texAO;
#endif
#ifdef NORMAL_MAP
DESCRIPTOR_SET_PASS(10) uniform sampler2D texNormal;
#endif
#ifdef SPECULAR_MAP
DESCRIPTOR_SET_PASS(11) uniform sampler2D texSpecular;
#endif

layout (location = 0) in highp   vec3 inPosition;
layout (location = 1) in mediump vec2 inTexcoord;
layout (location = 2) in mediump vec3 inHalfDirection;
layout (location = 3) in mediump vec3 inViewDirection;
#ifdef NORMAL_MAP
layout (location = 4) in mediump mat3 inTBN;
#else
layout (location = 4) in mediump vec3 inNormal;
#endif

layout (location = 0) out vec4 outFragColor;

void main()
{
	lowp vec3 albedoColor = Gamma2Linear(texture(texAlbedo, inTexcoord).rgb);
#ifdef AO_MAP
	lowp vec3 ao = texture(texAO, inTexcoord).rgb;
#else
	lowp vec3 ao = vec3(1.0);
#endif
#ifdef SPECULAR_MAP
	lowp vec3 specularColor = texture(texSpecular, inTexcoord).rgb;
#else
	lowp vec3 specularColor = vec3(1.0);
#endif
	lowp float metallic = 0.7;
	lowp float roughness = 0.3;

#ifdef NORMAL_MAP
	mediump vec3 pixelNormal = texture(texNormal, inTexcoord).rgb * 2.0 - 1.0;
	pixelNormal = normalize(inTBN * pixelNormal);
#else
	mediump vec3 pixelNormal = inNormal;
#endif

	mediump vec3 pointLightColor = mainPointLightColor;
	mediump vec3 pointLightDirection = mainPointLightPosition - inPosition;
	pointLightColor = pointLightColor * PointLightAttenuation(length(pointLightDirection));
	pointLightDirection = normalize(pointLightDirection);

	mediump vec3 ambientLightingColor = AmbientLightingSH9(albedoColor, 0.0, pixelNormal) * ambientLightFactor;
	mediump vec3 pointLightingColor = SimpleLighting(pointLightColor, pointLightDirection, pixelNormal, albedoColor) * pointLightFactor;
	mediump vec3 directLightingColor = BlinnLighting(mainDirectLightColor, mainDirectLightDirection, inHalfDirection, inViewDirection, pixelNormal, albedoColor, specularColor, metallic, roughness) * directLightFactor;
	mediump vec3 finalLighting = FinalLighting(ao, ambientLightingColor, pointLightingColor, directLightingColor, vec3(0.0), 1.0);

	finalLighting = ToneMapping(finalLighting);
	finalLighting = Linear2Gamma(finalLighting);

	outFragColor.rgb = finalLighting;
	outFragColor.a = 1.0;
}
#endif