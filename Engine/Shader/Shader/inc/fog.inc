#ifndef _FOG_INC_
#define _FOG_INC_

#include "engine.inc"
#include "common.inc"


float FogDensityLinear(vec3 cameraPosition, vec3 position)
{
	float distance = length(position - cameraPosition);

	float heightDensity = clamp((mainFogHeightEnd - position.y) / (mainFogHeightEnd - mainFogHeightStart), 0.0, 1.0) * mainFogHeightDensity;
	float distanceDensity = clamp((distance - mainFogDistanceStart) / (mainFogDistanceEnd - mainFogDistanceStart), 0.0, 1.0) * mainFogDistanceDensity;

	return clamp(heightDensity + distanceDensity, 0.0, 1.0);
}

float FogDensityExp(vec3 cameraPosition, vec3 position)
{
	float distance = length(position - cameraPosition);

	float heightDensity = (mainFogHeightEnd - position.y) * mainFogHeightDensity;
	float distanceDensity = (distance - mainFogDistanceStart) * mainFogDistanceDensity;

	heightDensity = exp(heightDensity);
	distanceDensity = exp(distanceDensity);

	heightDensity = clamp(heightDensity, 0.0, 1.0);
	distanceDensity = clamp(distanceDensity, 0.0, 1.0);

	return clamp(heightDensity + distanceDensity, 0.0, 1.0);
}

float FogDensityExp2(vec3 cameraPosition, vec3 position)
{
	float distance = length(position - cameraPosition);

	float heightDensity = (mainFogHeightEnd - position.y) * mainFogHeightDensity;
	float distanceDensity = (distance - mainFogDistanceStart) * mainFogDistanceDensity;

	heightDensity = exp(heightDensity * heightDensity);
	distanceDensity = exp(distanceDensity * distanceDensity);

	heightDensity = clamp(heightDensity, 0.0, 1.0);
	distanceDensity = clamp(distanceDensity, 0.0, 1.0);

	return clamp(heightDensity + distanceDensity, 0.0, 1.0);
}

vec3 FogColor(vec3 color, float density)
{
	return mix(color, mainFogColor, density);
}

#endif
