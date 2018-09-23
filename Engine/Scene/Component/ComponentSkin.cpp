#include "Engine.h"
#include "ComponentSkin.h"


CComponentSkin::CComponentSkin(uint32_t name)
	: CComponent(name)
{

}

CComponentSkin::CComponentSkin(const CComponentSkin &component)
	: CComponent(component)
{

}

CComponentSkin::~CComponentSkin(void)
{

}

void CComponentSkin::TaskUpdate(float gameTime, float deltaTime)
{

}

void CComponentSkin::TaskUpdateCamera(CGfxCamera *pCamera, int indexThread)
{

}
