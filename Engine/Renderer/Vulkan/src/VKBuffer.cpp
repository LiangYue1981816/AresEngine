#include "VKRenderer.h"


CVKBuffer::CVKBuffer(CVKDevice* pDevice, VkDeviceSize size, VkBufferUsageFlags bufferUsageFlags, VkMemoryPropertyFlags memoryPropertyFlags)
	: m_pDevice(pDevice)

	, m_vkBuffer(VK_NULL_HANDLE)
	, m_vkBufferUsageFlags(bufferUsageFlags)

	, m_pMemory(nullptr)
{
	VkBufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.size = size;
	createInfo.usage = bufferUsageFlags;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.queueFamilyIndexCount = 0;
	createInfo.pQueueFamilyIndices = nullptr;
	CALL_VK_FUNCTION_RETURN(vkCreateBuffer(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkBuffer));

	VkMemoryRequirements requirements;
	vkGetBufferMemoryRequirements(m_pDevice->GetDevice(), m_vkBuffer, &requirements);

	if (m_pMemory = m_pDevice->GetMemoryManager()->AllocMemory(requirements.size, requirements.alignment, requirements.memoryTypeBits, memoryPropertyFlags)) {
		m_pMemory->BindBuffer(m_vkBuffer);
	}
}

CVKBuffer::~CVKBuffer(void)
{
	if (m_vkBuffer) {
		vkDestroyBuffer(m_pDevice->GetDevice(), m_vkBuffer, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (m_pMemory) {
		m_pDevice->GetMemoryManager()->FreeMemory(m_pMemory);
	}
}

void CVKBuffer::Release(void)
{
	delete this;
}

VkBuffer CVKBuffer::GetBuffer(void) const
{
	return m_vkBuffer;
}

VkDeviceSize CVKBuffer::GetSize(void) const
{
	if (m_pMemory) {
		return m_pMemory->GetSize();
	}
	else {
		return 0;
	}
}

bool CVKBuffer::BufferData(size_t offset, size_t size, const void* data)
{
	if (m_pMemory) {
		if (m_pMemory->IsHostVisible()) {
			CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->BeginMap(offset, size));
			CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->CopyData(0, size, data));
			CALL_BOOL_FUNCTION_RETURN_BOOL(m_pMemory->EndMap());
			return true;
		}
		else {
			VkAccessFlags dstAccessFlags = 0;
			VkPipelineStageFlags dstPipelineStageFlags = 0;

			if (m_vkBufferUsageFlags & VK_BUFFER_USAGE_INDEX_BUFFER_BIT) {
				dstAccessFlags |= VK_ACCESS_INDEX_READ_BIT;
				dstPipelineStageFlags |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
			}

			if (m_vkBufferUsageFlags & VK_BUFFER_USAGE_VERTEX_BUFFER_BIT) {
				dstAccessFlags |= VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
				dstPipelineStageFlags |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
			}

			if (m_vkBufferUsageFlags & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT) {
				dstAccessFlags |= VK_ACCESS_UNIFORM_READ_BIT;
				dstPipelineStageFlags |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
			}

			return m_pDevice->GetTransferBufferManager()->TransferBufferData(this, dstAccessFlags, dstPipelineStageFlags, offset, size, data);
		}
	}
	else {
		return false;
	}
}

bool CVKBuffer::PipelineBarrier(VkCommandBuffer vkCommandBuffer, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkPipelineStageFlags srcPipelineStageFlags, VkPipelineStageFlags dstPipelineStageFlags, VkDeviceSize offset, VkDeviceSize size)
{
	if (m_vkBuffer) {
		CALL_VK_FUNCTION_RETURN_BOOL(vkCmdBufferMemoryBarrier(vkCommandBuffer, m_vkBuffer, srcAccessFlags, dstAccessFlags, srcPipelineStageFlags, dstPipelineStageFlags, offset, size));
		return true;
	}
	else {
		return false;
	}
}
