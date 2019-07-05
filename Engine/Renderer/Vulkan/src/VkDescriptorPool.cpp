#include "VKRenderer.h"


const uint32_t numMaxSets = 256;
const uint32_t numUniformBuffers = 1024;
const uint32_t numCombinedImageSamplers = 512;
const uint32_t numStorageImages = 32;
const uint32_t numStorageBuffers = 32;
const uint32_t numStorageTexelBuffers = 32;
const uint32_t numUniformTexelBuffers = 32;
const uint32_t numInputAttachments = 32;

CVKDescriptorPool::CVKDescriptorPool(CVKDevice* pDevice, CVKDescriptorSetManager* pManager)
	: m_pDevice(pDevice)
	, m_pManager(pManager)

	, m_numSets(0)
	, m_numDescriptors{ 0 }
	, m_vkDescriptorPool(VK_NULL_HANDLE)

	, pNext(nullptr)
	, pPrev(nullptr)
{
	Create();
}

CVKDescriptorPool::~CVKDescriptorPool(void)
{
	Destroy();
}

bool CVKDescriptorPool::Create(void)
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

	VkDescriptorPoolCreateInfo poolCreateInfo = {};
	poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolCreateInfo.pNext = nullptr;
	poolCreateInfo.flags = 0;
	poolCreateInfo.maxSets = numMaxSets;
	poolCreateInfo.poolSizeCount = sizeof(poolSizes) / sizeof(VkDescriptorPoolSize);
	poolCreateInfo.pPoolSizes = poolSizes;
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateDescriptorPool(m_pDevice->GetDevice(), &poolCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkDescriptorPool));

	return true;
}

void CVKDescriptorPool::Destroy(void)
{
	ASSERT(m_vkDescriptorPool);

	for (const auto& itDescriptorSet : m_pDescriptorSets) {
		delete itDescriptorSet.second;
	}

	vkResetDescriptorPool(m_pDevice->GetDevice(), m_vkDescriptorPool, 0);
	vkDestroyDescriptorPool(m_pDevice->GetDevice(), m_vkDescriptorPool, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());

	m_vkDescriptorPool = VK_NULL_HANDLE;
	memset(m_numDescriptors, 0, sizeof(m_numDescriptors));
}

VkDescriptorPool CVKDescriptorPool::GetDescriptorPool(void) const
{
	ASSERT(m_vkDescriptorPool);
	return m_vkDescriptorPool;
}

CVKDescriptorSetManager* CVKDescriptorPool::GetDescriptorSetManager(void) const
{
	ASSERT(m_pManager);
	return m_pManager;
}

CVKDescriptorSet* CVKDescriptorPool::AllocDescriptorSet(uint32_t name, const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
{
	ASSERT(m_vkDescriptorPool);
	ASSERT(ptrDescriptorLayout);

	if (m_numSets == 0) {
		return nullptr;
	}

	for (int indexType = 0; indexType < VK_DESCRIPTOR_TYPE_RANGE_SIZE; indexType++) {
		if (m_numDescriptors[indexType] < ((CVKDescriptorLayout*)ptrDescriptorLayout.GetPointer())->GetNumDescriptors()[indexType]) {
			return nullptr;
		}
	}

	CVKDescriptorSet* pDescriptorSet = new CVKDescriptorSet(m_pDevice, this, name, ptrDescriptorLayout);
	m_pDescriptorSets[pDescriptorSet] = pDescriptorSet;

	m_numSets -= 1;

	for (int indexType = 0; indexType < VK_DESCRIPTOR_TYPE_RANGE_SIZE; indexType++) {
		m_numDescriptors[indexType] -= ((CVKDescriptorLayout*)ptrDescriptorLayout.GetPointer())->GetNumDescriptors()[indexType];
	}

	return pDescriptorSet;
}

CVKDescriptorSet* CVKDescriptorPool::AllocDescriptorSet(uint32_t name, const CGfxDescriptorSetPtr ptrDescriptorSetTemplate)
{
	ASSERT(m_vkDescriptorPool);
	ASSERT(ptrDescriptorSetTemplate);

	if (m_numSets == 0) {
		return nullptr;
	}

	for (int indexType = 0; indexType < VK_DESCRIPTOR_TYPE_RANGE_SIZE; indexType++) {
		if (m_numDescriptors[indexType] < ((CVKDescriptorLayout*)ptrDescriptorSetTemplate->GetDescriptorLayout().GetPointer())->GetNumDescriptors()[indexType]) {
			return nullptr;
		}
	}

	CVKDescriptorSet* pDescriptorSet = new CVKDescriptorSet(m_pDevice, this, name, ptrDescriptorSetTemplate);
	m_pDescriptorSets[pDescriptorSet] = pDescriptorSet;

	m_numSets -= 1;

	for (int indexType = 0; indexType < VK_DESCRIPTOR_TYPE_RANGE_SIZE; indexType++) {
		m_numDescriptors[indexType] -= ((CVKDescriptorLayout*)ptrDescriptorSetTemplate->GetDescriptorLayout().GetPointer())->GetNumDescriptors()[indexType];
	}

	return pDescriptorSet;
}

bool CVKDescriptorPool::FreeDescriptorSet(CVKDescriptorSet* pDescriptorSet)
{
	ASSERT(pDescriptorSet);
	ASSERT(m_vkDescriptorPool);

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
		CALL_VK_FUNCTION_ASSERT(vkResetDescriptorPool(m_pDevice->GetDevice(), m_vkDescriptorPool, 0));
		return true;
	}
	else {
		return false;
	}
}
