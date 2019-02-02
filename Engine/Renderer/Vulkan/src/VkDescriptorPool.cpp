#include "VKRenderer.h"


CVKDescriptorPool::CVKDescriptorPool(CVKDevice *pDevice)
	: m_pDevice(pDevice)
	, m_vkDescriptorPool(VK_NULL_HANDLE)
{
	VkDescriptorPoolSize poolSizes[] = {
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1024 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1024 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 512 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 32 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 32 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 32 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 32 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 32 },
	};

	VkDescriptorPoolCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.maxSets = 256;
	createInfo.poolSizeCount = sizeof(poolSizes) / sizeof(VkDescriptorPoolSize);
	createInfo.pPoolSizes = poolSizes;
	vkCreateDescriptorPool(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkDescriptorPool);
}

CVKDescriptorPool::~CVKDescriptorPool(void)
{
	if (m_vkDescriptorPool) {
		vkDestroyDescriptorPool(m_pDevice->GetDevice(), m_vkDescriptorPool, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	m_vkDescriptorPool = VK_NULL_HANDLE;
}
