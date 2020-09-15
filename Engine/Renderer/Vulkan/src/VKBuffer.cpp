#include "VKRenderer.h"


CVKBuffer::CVKBuffer(CVKDevice* pDevice, VkDeviceSize size, VkBufferUsageFlags bufferUsageFlags, VkMemoryPropertyFlags memoryPropertyFlags)
	: m_pDevice(pDevice)
	, m_pMemory(nullptr)

	, m_vkBuffer(VK_NULL_HANDLE)
	, m_vkSize(0)
	, m_vkBufferUsageFlags(0)
{
	Create(size, bufferUsageFlags, memoryPropertyFlags);
}

CVKBuffer::~CVKBuffer(void)
{
	Destroy();
}

bool CVKBuffer::Create(VkDeviceSize size, VkBufferUsageFlags bufferUsageFlags, VkMemoryPropertyFlags memoryPropertyFlags)
{
	ASSERT(size);
	ASSERT(bufferUsageFlags);
	ASSERT(memoryPropertyFlags);

	m_vkSize = size;
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
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateBuffer(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkBuffer));

	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), m_vkBuffer, &requirements);

	m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(requirements.size, requirements.alignment, memoryPropertyFlags, VK_RESOURCE_TYPE_BUFFER);
	if (m_pMemory == nullptr) { ASSERT(false); return false; }
	if (m_pMemory->BindBuffer(m_vkBuffer) == false) { ASSERT(false); return false; }

	return true;
}

void CVKBuffer::Destroy(void)
{
	ASSERT(m_vkBuffer);
	ASSERT(m_pMemory);

	vkDestroyBuffer(m_pDevice->GetDevice(), m_vkBuffer, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	m_pDevice->GetMemoryManager()->FreeMemory(m_pMemory);

	m_pMemory = nullptr;
	m_vkBuffer = VK_NULL_HANDLE;
	m_vkBufferUsageFlags = 0;
}

VkBuffer CVKBuffer::GetBuffer(void) const
{
	ASSERT(m_vkBuffer);
	return m_vkBuffer;
}

VkDeviceSize CVKBuffer::GetSize(void) const
{
	ASSERT(m_vkSize);
	return m_vkSize;
}

VkDeviceSize CVKBuffer::GetMemorySize(void) const
{
	ASSERT(m_pMemory);
	return m_pMemory->GetSize();
}

VkBufferUsageFlags CVKBuffer::GetBufferUsageFlags(void) const
{
	ASSERT(m_vkBufferUsageFlags);
	return m_vkBufferUsageFlags;
}

bool CVKBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	ASSERT(m_vkBuffer);
	ASSERT(m_vkSize >= offset + size);

	if (size && data) {
		if (m_pMemory->IsHostVisible()) {
			CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->BeginMap());
			CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->CopyData(offset, size, data));
			CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->EndMap());
		}
		/*
		else {
			VkAccessFlags dstAccessFlags = CVKHelper::GetAccessMaskByBufferUsage(m_vkBufferUsageFlags);
			VkPipelineStageFlags dstPipelineStageFlags = CVKHelper::GetPipelineStageFlagsByBufferUsage(m_vkBufferUsageFlags);
			CALL_BOOL_FUNCTION_RETURN_BOOL(m_pDevice->GetTransferManager()->TransferBufferData(this, dstAccessFlags, dstPipelineStageFlags, offset, size, data));
		}
		*/
	}

	return true;
}
/*
bool CVKBuffer::PipelineBarrier(VkCommandBuffer vkCommandBuffer, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkDeviceSize offset, VkDeviceSize size)
{
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkBuffer);
	ASSERT(m_vkSize >= offset + size);

	CALL_VK_FUNCTION_ASSERT(vkCmdBufferMemoryBarrier(vkCommandBuffer, m_vkBuffer, srcAccessFlags, dstAccessFlags, offset, size));
	return true;
}

bool CVKBuffer::PipelineBarrier(VkCommandBuffer vkCommandBuffer, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkPipelineStageFlags srcPipelineStageFlags, VkPipelineStageFlags dstPipelineStageFlags, VkDeviceSize offset, VkDeviceSize size)
{
	ASSERT(vkCommandBuffer);
	ASSERT(m_vkBuffer);
	ASSERT(m_vkSize >= offset + size);

	CALL_VK_FUNCTION_ASSERT(vkCmdBufferMemoryBarrier(vkCommandBuffer, m_vkBuffer, srcAccessFlags, dstAccessFlags, srcPipelineStageFlags, dstPipelineStageFlags, offset, size));
	return true;
}
*/
bool CVKBuffer::IsDeviceLocal(void) const
{
	ASSERT(m_pMemory);
	return m_pMemory->IsDeviceLocal();
}

bool CVKBuffer::IsHostVisible(void) const
{
	ASSERT(m_pMemory);
	return m_pMemory->IsHostVisible();
}

bool CVKBuffer::IsHostCoherent(void) const
{
	ASSERT(m_pMemory);
	return m_pMemory->IsHostCoherent();
}

bool CVKBuffer::IsHostCached(void) const
{
	ASSERT(m_pMemory);
	return m_pMemory->IsHostCached();
}

bool CVKBuffer::IsLazilyAllocated(void) const
{
	ASSERT(m_pMemory);
	return m_pMemory->IsLazilyAllocated();
}
