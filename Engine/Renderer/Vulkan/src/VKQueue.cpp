#include "VKRenderer.h"


CVKQueue::CVKQueue(CVKDevice *pDevice, uint32_t queueFamilyIndex, uint32_t queueIndex)
	: m_pDevice(pDevice)

	, m_vkQueue(VK_NULL_HANDLE)
	, m_queueFamilyIndex(queueFamilyIndex)
{
	atomic_spin_init(&m_lock);
	vkGetDeviceQueue(pDevice->GetDevice(), queueFamilyIndex, queueIndex, &m_vkQueue);
}

CVKQueue::~CVKQueue(void)
{
	for (const auto &itCommandPool : m_vkCommandPools) {
		vkDestroyCommandPool(m_pDevice->GetDevice(), itCommandPool.second, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}
}

bool CVKQueue::ResetCommandBufferPool(uint32_t pool)
{
	atomic_spin_autolock autolock(&m_lock);

	if (m_vkCommandPools.find(pool) == m_vkCommandPools.end()) {
		return false;
	}

	CALL_VK_FUNCTION_RETURN_BOOL(vkResetCommandPool(m_pDevice->GetDevice(), m_vkCommandPools[pool], VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT));
	return true;
}

bool CVKQueue::AllocCommandBuffer(uint32_t pool, VkCommandBufferLevel level, VkCommandBuffer *pvkCommandBuffer)
{
	atomic_spin_autolock autolock(&m_lock);

	if (m_vkCommandPools.find(pool) == m_vkCommandPools.end()) {
		VkCommandPool vkCommandPool = VK_NULL_HANDLE;
		VkCommandPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		createInfo.queueFamilyIndex = m_queueFamilyIndex;
		CALL_VK_FUNCTION_RETURN_BOOL(vkCreateCommandPool(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &vkCommandPool));
		m_vkCommandPools[pool] = vkCommandPool;
	}

	VkCommandBufferAllocateInfo commandBufferInfo = {};
	commandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferInfo.pNext = nullptr;
	commandBufferInfo.level = level;
	commandBufferInfo.commandPool = m_vkCommandPools[pool];
	commandBufferInfo.commandBufferCount = 1;
	CALL_VK_FUNCTION_RETURN_BOOL(vkAllocateCommandBuffers(m_pDevice->GetDevice(), &commandBufferInfo, pvkCommandBuffer));
	return true;
}

void CVKQueue::FreeCommandBuffer(uint32_t pool, VkCommandBuffer vkCommandBuffer)
{
	atomic_spin_autolock autolock(&m_lock);

	if (m_vkCommandPools.find(pool) == m_vkCommandPools.end()) {
		return;
	}

	vkDestroyCommandPool(m_pDevice->GetDevice(), m_vkCommandPools[pool], m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	m_vkCommandPools.erase(pool);
}

bool CVKQueue::Submit(const CGfxCommandBufferPtr &ptrCommandBuffer) const
{
	return Submit(ptrCommandBuffer, VK_NULL_HANDLE, 0, VK_NULL_HANDLE);
}

bool CVKQueue::Submit(const CGfxCommandBufferPtr &ptrCommandBuffer, VkSemaphore vkWaitSemaphore, VkPipelineStageFlags waitStageFlags, VkSemaphore vkSignalSemaphore) const
{
	return true;
}

void CVKQueue::WaitIdle(void) const
{
	vkQueueWaitIdle(m_vkQueue);
}

uint32_t CVKQueue::GetQueueFamilyIndex(void) const
{
	return m_queueFamilyIndex;
}
