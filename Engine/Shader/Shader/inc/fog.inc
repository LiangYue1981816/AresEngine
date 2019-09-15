#ifndef _FOG_INC_
#define _FOG_INC_

#include "engine.inc"
#include "common.inc"


highp float FogDensityLinear(highp vec3 cameraPosition, highp vec3 position)
{
	highp float distance = length(position - cameraPosition);

	highp float heightDensity = clamp((mainFogHeightEnd - position.y) / (mainFogHeightEnd - mainFogHeightStart), 0.0, 1.0) * mainFogHeightDensity;
	highp float distanceDensity = clamp((distance - mainFogDistanceStart) / (mainFogDistanceEnd - mainFogDistanceStart), 0.0, 1.0) * mainFogDistanceDensity;

	return clamp(heightDensity + distanceDensity, 0.0, 1.0);
}

highp float FogDensityExp(highp vec3 cameraPosition, highp vec3 position)
{
	highp float distance = length(position - cameraPosition);

	highp float heightDensity = (mainFogHeightEnd - position.y) * mainFogHeightDensity;
	highp float distanceDensity = (distance - mainFogDistanceStart) * mainFogDistanceDensity;

	heightDensity = exp(heightDensity);
	distanceDensity = exp(distanceDensity);

	heightDensity = clamp(heightDensity, 0.0, 1.0);
	distanceDensity = clamp(distanceDensity, 0.0, 1.0);

	return clamp(heightDensity + distanceDensity, 0.0, 1.0);
}

highp float FogDensityExp2(highp vec3 cameraPosition, highp vec3 position)
{
	highp float distance = length(position - cameraPosition);

	highp float heightDensity = (mainFogHeightEnd - position.y) * mainFogHeightDensity;
	highp float distanceDensity = (distance - mainFogDistanceStart) * mainFogDistanceDensity;

	heightDensity = exp(heightDensity * heightDensity);
	distanceDensity = exp(distanceDensity * distanceDensity);

	heightDensity = clamp(heightDensity, 0.0, 1.0);
	distanceDensity = clamp(distanceDensity, 0.0, 1.0);

	return clamp(heightDensity + distanceDensity, 0.0, 1.0);
}

mediump vec3 FogColor(mediump vec3 color, mediump float density)
{
	return mix(color, mainFogColor, density);
}

#endif
