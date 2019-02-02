#include "VKRenderer.h"


const uint32_t numMaxSets = 256;
const uint32_t numUniformBuffers = 1024;
const uint32_t numCombinedImageSamplers = 512;
const uint32_t numStorageImages = 32;
const uint32_t numStorageBuffers = 32;
const uint32_t numStorageTexelBuffers = 32;
const uint32_t numUniformTexelBuffers = 32;
const uint32_t numInputAttachments = 32;

CVKDescriptorPool::CVKDescriptorPool(CVKDevice *pDevice)
	: m_pDevice(pDevice)
	, m_vkDescriptorPool(VK_NULL_HANDLE)

	, m_numSets(0)
	, m_numDescriptors{ 0 }
{
	VkDescriptorPoolSize poolSizes[] = {
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, numUniformBuffers },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, numUniformBuffers },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, numCombinedImageSamplers },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, numStorageImages },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, numStorageBuffers },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, numStorageTexelBuffers },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, numUniformTexelBuffers },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, numInputAttachments },
	};

	VkDescriptorPoolCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.maxSets = numMaxSets;
	createInfo.poolSizeCount = sizeof(poolSizes) / sizeof(VkDescriptorPoolSize);
	createInfo.pPoolSizes = poolSizes;
	CALL_VK_FUNCTION_RETURN(vkCreateDescriptorPool(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkDescriptorPool));

	m_numSets = numMaxSets;
	m_numDescriptors[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER] = numUniformBuffers;
	m_numDescriptors[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC] = numUniformBuffers;
	m_numDescriptors[VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER] = numCombinedImageSamplers;
	m_numDescriptors[VK_DESCRIPTOR_TYPE_STORAGE_IMAGE] = numStorageImages;
	m_numDescriptors[VK_DESCRIPTOR_TYPE_STORAGE_BUFFER] = numStorageBuffers;
	m_numDescriptors[VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER] = numStorageTexelBuffers;
	m_numDescriptors[VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER] = numUniformTexelBuffers;
	m_numDescriptors[VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT] = numInputAttachments;
}

CVKDescriptorPool::~CVKDescriptorPool(void)
{
	if (m_vkDescriptorPool) {
		vkDestroyDescriptorPool(m_pDevice->GetDevice(), m_vkDescriptorPool, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	m_vkDescriptorPool = VK_NULL_HANDLE;
}
