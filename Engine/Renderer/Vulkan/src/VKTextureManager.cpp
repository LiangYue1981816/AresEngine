#include "VKRenderer.h"
#include "ResourceLoader.h"


CVKTextureManager::CVKTextureManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
{

}

CVKTextureManager::~CVKTextureManager(void)
{
	for (const auto &itTexture : m_pTextures) {
		delete itTexture.second;
	}

	m_pTextures.clear();
}

CVKDevice* CVKTextureManager::GetDevice(void) const
{
	return m_pDevice;
}
