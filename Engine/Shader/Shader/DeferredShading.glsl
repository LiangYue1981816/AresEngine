#version 310 es

#ifdef VERTEX_SHADER
precision highp float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM
USE_ENGINE_UNIFORM
USE_SCENE_DATA_STORAGE

// Output
layout (location = 0) out vec4 outPosition;
layout (location = 1) out flat int outInstanceIndex;

// Descriptor
// ...

void main()
{
	outInstanceIndex = int(inInstanceIndex);

	mat4 worldMatrix = GetInstance(int(inInstanceIndex)).transformMatrix;
	vec3 worldPosition = (worldMatrix * vec4(inPosition.xyz, 1.0)).xyz;
	gl_Position = outPosition = cameraProjectionViewMatrix * vec4(worldPosition, 1.0);
}
#endif

#ifdef FRAGMENT_SHADER
precision highp float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM
USE_ENGINE_UNIFORM
USE_SCENE_DATA_STORAGE
USE_DEPTH_TEXTURE_UNIFORM

#include "lighting.inc"

// Input
layout (location = 0) in vec4 inPosition;
layout (location = 1) in flat int inInstanceIndex;

// Output
layout (location = 0) out vec4 outFragColor;

// Descriptor
DESCRIPTOR_SET_INPUTATTACHMENT(1, 6) uniform highp subpassInput texGBuffer0;
DESCRIPTOR_SET_INPUTATTACHMENT(2, 7) uniform highp subpassInput texGBuffer1;

void main()
{
	vec4 pixelColorGBuffer0 = subpassLoad(texGBuffer0);
	vec4 pixelColorGBuffer1 = subpassLoad(texGBuffer1);

	vec4 projectCoord = inPosition;
	projectCoord.xy = projectCoord.xy / projectCoord.w;
 	projectCoord.xy = projectCoord.xy * 0.5 + 0.5;

	float depth = texture(texDepth, projectCoord.xy).r;
	vec3 worldPosition = ScreenToWorldPosition(projectCoord.xy, depth, cameraProjectionInverseMatrix, cameraViewInverseMatrix).xyz;
	vec3 worldCameraPosition = (cameraViewInverseMatrix * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	vec3 worldViewDirection = normalize(worldCameraPosition - worldPosition);
	vec3 worldNormal = NormalDecode(pixelColorGBuffer1.rg);

	vec3 albedo = pixelColorGBuffer0.rgb;
	float ao = pixelColorGBuffer0.a;
	float roughness = pixelColorGBuffer1.b;
	float metallic = pixelColorGBuffer1.a;

	vec3 pointLightPosition = GetInstance(inInstanceIndex).center.xyz;
	float pointLightRange = GetInstance(inInstanceIndex).lightAttenuation.w;
	float distance = length(pointLightPosition - worldPosition);

	vec3 pointLighting = vec3(0.0);

	if(pointLightRange > distance)
	{
		vec3 pointLightDirection = normalize(pointLightPosition - worldPosition);
		vec3 pointLightAttenuation = GetInstance(inInstanceIndex).lightAttenuation.xyz;
		vec3 pointLightColor = GetInstance(inInstanceIndex).lightColor.rgb * Attenuation(distance, pointLightAttenuation.x, pointLightAttenuation.y, pointLightAttenuation.z);
		vec3 lighting = PBRLighting(worldNormal, worldViewDirection, pointLightDirection, pointLightColor, albedo, 0.5, metallic, roughness) * pointLightFactor;
		pointLighting = ao * lighting;
	}

	outFragColor.rgb = pointLighting;
	outFragColor.a = 1.0;
}
#endif
