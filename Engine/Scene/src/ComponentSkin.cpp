#include "SceneHeader.h"
#include "RenderHeader.h"


CComponentSkin::CComponentSkin(uint32_t name)
	: CComponent(name)
{

}

CComponentSkin::CComponentSkin(const CComponentSkin& component)
	: CComponent(component)
{

}

CComponentSkin::~CComponentSkin(void)
{

}

void CComponentSkin::TaskUpdate(float gameTime, float deltaTime)
{

}

void CComponentSkin::TaskUpdateCamera(CGfxCamera* pCamera, CGfxRenderQueue* pRenderQueue, uint32_t mask, int indexThread)
{

}
