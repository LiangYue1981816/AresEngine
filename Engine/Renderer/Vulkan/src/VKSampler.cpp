#include "VKRenderer.h"


CVKSampler::CVKSampler(CVKDevice* pDevice, GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode)
	: CGfxSampler(minFilter, magFilter, mipmapMode, addressMode)
	, m_pDevice(pDevice)

	, m_vkSampler(VK_NULL_HANDLE)
{
	ASSERT(m_pDevice);

	Create(minFilter, magFilter, mipmapMode, addressMode);
}

CVKSampler::~CVKSampler(void)
{
	Destroy();
}

bool CVKSampler::Create(GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode)
{
	Destroy();
	{
		do {
			VkSamplerCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			createInfo.magFilter = CVKHelper::TranslateFilter(magFilter);
			createInfo.minFilter = CVKHelper::TranslateFilter(minFilter);
			createInfo.mipmapMode = CVKHelper::TranslateSamplerMipmapMode(mipmapMode);
			createInfo.addressModeU = CVKHelper::TranslateSamplerAddressMode(addressMode);
			createInfo.addressModeV = CVKHelper::TranslateSamplerAddressMode(addressMode);
			createInfo.addressModeW = CVKHelper::TranslateSamplerAddressMode(addressMode);
			createInfo.mipLodBias = 0.0f;
			createInfo.anisotropyEnable = VK_FALSE;
			createInfo.maxAnisotropy = 1.0f;
			createInfo.compareEnable = VK_FALSE;
			createInfo.compareOp = VK_COMPARE_OP_NEVER;
			createInfo.minLod = 0.0f;
			createInfo.maxLod = FLT_MAX;
			createInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
			createInfo.unnormalizedCoordinates = VK_FALSE;
			CALL_VK_FUNCTION_BREAK(vkCreateSampler(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkSampler));

			return true;
		} while (false);
	}
	Destroy();
	return false;
}

void CVKSampler::Destroy(void)
{
	if (m_vkSampler) {
		vkDestroySampler(m_pDevice->GetDevice(), m_vkSampler, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	m_vkSampler = VK_NULL_HANDLE;
}

VkSampler CVKSampler::GetSampler(void) const
{
	return m_vkSampler;
}
