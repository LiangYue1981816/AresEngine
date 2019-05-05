#include "VKRenderer.h"


const uint32_t numMaxSets = 256;
const uint32_t numUniformBuffers = 1024;
const uint32_t numCombinedImageSamplers = 512;
const uint32_t numStorageImages = 32;
const uint32_t numStorageBuffers = 32;
const uint32_t numStorageTexelBuffers = 32;
const uint32_t numUniformTexelBuffers = 32;
const uint32_t numInputAttachments = 32;

CVKDescriptorPool::CVKDescriptorPool(CVKDevice* pDevice)
	: m_pDevice(pDevice)
	, m_vkDescriptorPool(VK_NULL_HANDLE)

	, m_numSets(0)
	, m_numDescriptors{ 0 }

	, pNext(nullptr)
	, pPrev(nullptr)
{
	m_numSets = numMaxSets;
	m_numDescriptors[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER] = numUniformBuffers;
	m_numDescriptors[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC] = numUniformBuffers;
	m_numDescriptors[VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER] = numCombinedImageSamplers;
	m_numDescriptors[VK_DESCRIPTOR_TYPE_STORAGE_IMAGE] = numStorageImages;
	m_numDescriptors[VK_DESCRIPTOR_TYPE_STORAGE_BUFFER] = numStorageBuffers;
	m_numDescriptors[VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER] = numStorageTexelBuffers;
	m_numDescriptors[VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER] = numUniformTexelBuffers;
	m_numDescriptors[VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT] = numInputAttachments;

	VkDescriptorPoolSize poolSizes[] = {
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, m_numDescriptors[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER] },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, m_numDescriptors[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC] },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, m_numDescriptors[VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER] },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, m_numDescriptors[VK_DESCRIPTOR_TYPE_STORAGE_IMAGE] },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, m_numDescriptors[VK_DESCRIPTOR_TYPE_STORAGE_BUFFER] },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, m_numDescriptors[VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER] },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, m_numDescriptors[VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER] },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, m_numDescriptors[VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT] },
	};

	VkDescriptorPoolCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.maxSets = numMaxSets;
	createInfo.poolSizeCount = sizeof(poolSizes) / sizeof(VkDescriptorPoolSize);
	createInfo.pPoolSizes = poolSizes;
	vkCreateDescriptorPool(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkDescriptorPool);
}

CVKDescriptorPool::~CVKDescriptorPool(void)
{
	for (const auto& itDescriptorSet : m_pDescriptorSets) {
		delete itDescriptorSet.second;
	}

	vkResetDescriptorPool(m_pDevice->GetDevice(), m_vkDescriptorPool, 0);
	vkDestroyDescriptorPool(m_pDevice->GetDevice(), m_vkDescriptorPool, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
}

VkDescriptorPool CVKDescriptorPool::GetDescriptorPool(void) const
{
	return m_vkDescriptorPool;
}

CVKDescriptorSet* CVKDescriptorPool::AllocDescriptorSet(const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
{
	if (m_numSets == 0) {
		return nullptr;
	}

	for (int index = 0; index < VK_DESCRIPTOR_TYPE_RANGE_SIZE; index++) {
		if (m_numDescriptors[index] < ptrDescriptorLayout->GetNumDescriptors()[index]) {
			return nullptr;
		}
	}

	CVKDescriptorSet* pDescriptorSet = new CVKDescriptorSet(m_pDevice, this, ptrDescriptorLayout);
	m_pDescriptorSets[pDescriptorSet] = pDescriptorSet;

	m_numSets -= 1;

	for (int index = 0; index < VK_DESCRIPTOR_TYPE_RANGE_SIZE; index++) {
		m_numDescriptors[index] -= ptrDescriptorLayout->GetNumDescriptors()[index];
	}

	return pDescriptorSet;
}

bool CVKDescriptorPool::FreeDescriptorSet(CVKDescriptorSet* pDescriptorSet)
{
	m_pDescriptorSets.erase(pDescriptorSet);
	delete pDescriptorSet;

	if (m_pDescriptorSets.empty()) {
		m_numSets = numMaxSets;
		m_numDescriptors[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER] = numUniformBuffers;
		m_numDescriptors[VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC] = numUniformBuffers;
		m_numDescriptors[VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER] = numCombinedImageSamplers;
		m_numDescriptors[VK_DESCRIPTOR_TYPE_STORAGE_IMAGE] = numStorageImages;
		m_numDescriptors[VK_DESCRIPTOR_TYPE_STORAGE_BUFFER] = numStorageBuffers;
		m_numDescriptors[VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER] = numStorageTexelBuffers;
		m_numDescriptors[VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER] = numUniformTexelBuffers;
		m_numDescriptors[VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT] = numInputAttachments;
		vkResetDescriptorPool(m_pDevice->GetDevice(), m_vkDescriptorPool, 0);
		return true;
	}
	else {
		return false;
	}
}
