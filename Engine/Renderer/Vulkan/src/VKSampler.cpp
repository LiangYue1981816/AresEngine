#include "VKRenderer.h"


CVKSampler::CVKSampler(CVKDevice *pDevice, GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode)
	: CGfxSampler(minFilter, magFilter, mipmapMode, addressMode)
	, m_pDevice(pDevice)

	, m_vkSampler(VK_NULL_HANDLE)
{
	VkSamplerCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.magFilter = (VkFilter)magFilter;
	createInfo.minFilter = (VkFilter)minFilter;
	createInfo.mipmapMode = (VkSamplerMipmapMode)mipmapMode;
	createInfo.addressModeU = (VkSamplerAddressMode)addressMode;
	createInfo.addressModeV = (VkSamplerAddressMode)addressMode;
	createInfo.addressModeW = (VkSamplerAddressMode)addressMode;
	createInfo.mipLodBias = 0.0f;
	createInfo.anisotropyEnable = VK_FALSE;
	createInfo.maxAnisotropy = 1.0f;
	createInfo.compareEnable = VK_FALSE;
	createInfo.compareOp = VK_COMPARE_OP_NEVER;
	createInfo.minLod = 0.0f;
	createInfo.maxLod = FLT_MAX;
	createInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
	createInfo.unnormalizedCoordinates = VK_FALSE;
	CALL_VK_FUNCTION_RETURN(vkCreateSampler(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkSampler));
}

CVKSampler::~CVKSampler(void)
{
	if (m_vkSampler) {
		vkDestroySampler(m_pDevice->GetDevice(), m_vkSampler, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}
}

HANDLE CVKSampler::GetSampler(void) const
{
	return (HANDLE)m_vkSampler;
}
