#include "VKRenderer.h"


CVKBuffer::CVKBuffer(CVKDevice* pDevice, VkDeviceSize size, VkBufferUsageFlags bufferUsageFlags, VkMemoryPropertyFlags memoryPropertyFlags)
	: m_pDevice(pDevice)
	, m_pMemory(nullptr)

	, m_vkBuffer(VK_NULL_HANDLE)
	, m_vkBufferUsageFlags(0)
{
	ASSERT(m_pDevice);

	Create(size, bufferUsageFlags, memoryPropertyFlags);
}

CVKBuffer::~CVKBuffer(void)
{
	Destroy();
}

void CVKBuffer::Release(void)
{
	delete this;
}

bool CVKBuffer::Create(VkDeviceSize size, VkBufferUsageFlags bufferUsageFlags, VkMemoryPropertyFlags memoryPropertyFlags)
{
	Destroy();
	{
		do {
			ASSERT(size);
			ASSERT(bufferUsageFlags);
			ASSERT(memoryPropertyFlags);

			m_vkBufferUsageFlags = bufferUsageFlags;

			VkBufferCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			createInfo.size = size;
			createInfo.usage = bufferUsageFlags;
			createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
			CALL_VK_FUNCTION_BREAK(vkCreateBuffer(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkBuffer));

			VkMemoryRequirements requirements;
			vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), m_vkBuffer, &requirements);

			m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(requirements.size, requirements.alignment, requirements.memoryTypeBits, memoryPropertyFlags);
			if (m_pMemory == nullptr) { ASSERT(false); break; }
			if (m_pMemory->BindBuffer(m_vkBuffer) == false) { ASSERT(false); break; }

			return true;
		} while (false);
	}
	Destroy();
	return false;
}

void CVKBuffer::Destroy(void)
{
	if (m_vkBuffer) {
		vkDestroyBuffer(m_pDevice->GetDevice(), m_vkBuffer, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (m_pMemory) {
		m_pDevice->GetMemoryManager()->FreeMemory(m_pMemory);
	}

	m_pMemory = nullptr;
	m_vkBuffer = VK_NULL_HANDLE;
	m_vkBufferUsageFlags = 0;
}

VkBuffer CVKBuffer::GetBuffer(void) const
{
	return m_vkBuffer;
}

VkDeviceSize CVKBuffer::GetSize(void) const
{
	return m_pMemory->GetSize();
}

bool CVKBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	ASSERT(data);
	ASSERT(size);
	ASSERT(m_pMemory->GetSize() >= offset + size);

	if (m_pMemory->IsHostVisible()) {
		CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->BeginMap(offset, size));
		CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->CopyData(0, size, data));
		CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->EndMap());
		return true;
	}
	else {
		VkAccessFlags dstAccessFlags = CVKHelper::GetAccessMaskByBufferUsage(m_vkBufferUsageFlags);
		VkPipelineStageFlags dstPipelineStageFlags = CVKHelper::GetPipelineStageFlagsByBufferUsage(m_vkBufferUsageFlags);
		return m_pDevice->GetTransferManager()->TransferBufferData(this, dstAccessFlags, dstPipelineStageFlags, offset, size, data);
	}
}

bool CVKBuffer::PipelineBarrier(VkCommandBuffer vkCommandBuffer, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkPipelineStageFlags srcPipelineStageFlags, VkPipelineStageFlags dstPipelineStageFlags, VkDeviceSize offset, VkDeviceSize size)
{
	ASSERT(vkCommandBuffer);
	ASSERT(m_pMemory->GetSize() >= offset + size);

	CALL_VK_FUNCTION_ASSERT(vkCmdBufferMemoryBarrier(vkCommandBuffer, m_vkBuffer, srcAccessFlags, dstAccessFlags, srcPipelineStageFlags, dstPipelineStageFlags, offset, size));
	return true;
}

bool CVKBuffer::IsDeviceLocal(void) const
{
	return m_pMemory->IsDeviceLocal();
}

bool CVKBuffer::IsHostVisible(void) const
{
	return m_pMemory->IsHostVisible();
}

bool CVKBuffer::IsHostCoherent(void) const
{
	return m_pMemory->IsHostCoherent();
}

bool CVKBuffer::IsHostCached(void) const
{
	return m_pMemory->IsHostCached();
}

bool CVKBuffer::IsLazilyAllocated(void) const
{
	return m_pMemory->IsLazilyAllocated();
}
