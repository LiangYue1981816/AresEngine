#include "VKRenderer.h"


CVKTexture::CVKTexture(CVKDevice *pDevice, CVKTextureManager *pManager)
	: m_pDevice(pDevice)
	, m_pManager(pManager)

	, m_bExtern(false)
	, m_vkImageView(VK_NULL_HANDLE)
{

}

CVKTexture::~CVKTexture(void)
{

}

bool CVKTexture::Create(VkImageView vkImageView)
{
	Destroy();

	m_bExtern = true;
	m_vkImageView = vkImageView;

	return true;
}

void CVKTexture::Destroy(void)
{
	if (m_bExtern == false) {
		if (m_vkImageView) {
			vkDestroyImageView(m_pDevice->GetDevice(), m_vkImageView, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
		}
	}

	m_bExtern = false;
	m_vkImageView = VK_NULL_HANDLE;
	m_ptrImage.Release();
}

VkImageView CVKTexture::GetImageView(void) const
{
	return m_vkImageView;
}
