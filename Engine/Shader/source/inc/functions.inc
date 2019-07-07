#ifndef _FUNCTIONS_INC_
#define _FUNCTIONS_INC_

#include "engine.inc"
#include "common.inc"


vec3 BlendNormal(vec3 baseNormal, vec3 additionalNormal)
{
	vec3 newBaseNormal = vec3(baseNormal.rg, baseNormal.b + 1.0);
	vec3 newAdditionalNormal = vec3(additionalNormal.rg * (-1.0), additionalNormal.b);
	
	newBaseNormal = newBaseNormal * dot(newBaseNormal, newAdditionalNormal);
	newAdditionalNormal = newAdditionalNormal * (baseNormal.b + 1.0);
	
	return newBaseNormal - newAdditionalNormal;
}

#endif
