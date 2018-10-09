#include "Engine.h"
#include "ComponentPointLight.h"


CComponentPointLight::CComponentPointLight(uint32_t name)
	: CComponent(name)
{

}

CComponentPointLight::CComponentPointLight(const CComponentPointLight &component)
	: CComponent(component)
{

}

CComponentPointLight::~CComponentPointLight(void)
{

}

void CComponentPointLight::TaskUpdate(float gameTime, float deltaTime)
{

}

void CComponentPointLight::TaskUpdateCamera(CGfxCamera *pCamera, int indexThread, int indexQueue)
{

}
