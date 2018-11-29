#include "GfxHeader.h"


CGfxResourceLoader* CGfxResourceLoader::pInstance = nullptr;
CGfxResourceLoader* CGfxResourceLoader::GetInstance(void)
{
	return pInstance;
}

CGfxResourceLoader::CGfxResourceLoader(void)
{
	pInstance = this;
}

CGfxResourceLoader::~CGfxResourceLoader(void)
{
	pInstance = nullptr;
}
