#include "VKRenderer.h"


CVKSampler::CVKSampler(CVKDevice* pDevice, GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode)
	: CGfxSampler(minFilter, magFilter, mipmapMode, addressMode)
	, m_pDevice(pDevice)
	, m_vkSampler(VK_NULL_HANDLE)
{
	Create(minFilter, magFilter, mipmapMode, addressMode);
}

CVKSampler::~CVKSampler(void)
{
	Destroy();
}

bool CVKSampler::Create(GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode)
{
	VkSamplerCreateInfo samplerCreateInfo = {};
	samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.pNext = nullptr;
	samplerCreateInfo.flags = 0;
	samplerCreateInfo.magFilter = CVKHelper::TranslateFilter(magFilter);
	samplerCreateInfo.minFilter = CVKHelper::TranslateFilter(minFilter);
	samplerCreateInfo.mipmapMode = CVKHelper::TranslateSamplerMipmapMode(mipmapMode);
	samplerCreateInfo.addressModeU = CVKHelper::TranslateSamplerAddressMode(addressMode);
	samplerCreateInfo.addressModeV = CVKHelper::TranslateSamplerAddressMode(addressMode);
	samplerCreateInfo.addressModeW = CVKHelper::TranslateSamplerAddressMode(addressMode);
	samplerCreateInfo.mipLodBias = 0.0f;
	samplerCreateInfo.anisotropyEnable = VK_FALSE;
	samplerCreateInfo.maxAnisotropy = 1.0f;
	samplerCreateInfo.compareEnable = VK_FALSE;
	samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
	samplerCreateInfo.minLod = 0.0f;
	samplerCreateInfo.maxLod = FLT_MAX;
	samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateSampler(m_pDevice->GetDevice(), &samplerCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkSampler));

	return true;
}

void CVKSampler::Destroy(void)
{
	vkDestroySampler(m_pDevice->GetDevice(), m_vkSampler, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	m_vkSampler = VK_NULL_HANDLE;
}

VkSampler CVKSampler::GetSampler(void) const
{
	ASSERT(m_vkSampler);
	return m_vkSampler;
}
