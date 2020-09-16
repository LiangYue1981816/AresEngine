#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBufferData : public CGfxCommandBase
{
public:
	CVKCommandBufferData(CVKDevice* pDevice, VkCommandBuffer vkCommandBuffer, const CGfxIndexBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
		: m_pDevice(pDevice)
		, m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrIndexBuffer(ptrBuffer)
		, m_offset(offset)
		, m_size(size)
		, m_data(data)
	{
		m_ptrTransferBuffer = new CVKTransferBuffer(pDevice, size);
		m_ptrTransferBuffer->BufferData(0, size, data);

		Execute();
	}
	CVKCommandBufferData(CVKDevice* pDevice, VkCommandBuffer vkCommandBuffer, const CGfxVertexBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
		: m_pDevice(pDevice)
		, m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrVertexBuffer(ptrBuffer)
		, m_offset(offset)
		, m_size(size)
		, m_data(data)
	{
		m_ptrTransferBuffer = new CVKTransferBuffer(pDevice, size);
		m_ptrTransferBuffer->BufferData(0, size, data);

		Execute();
	}
	CVKCommandBufferData(CVKDevice* pDevice, VkCommandBuffer vkCommandBuffer, const CGfxInstanceBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
		: m_pDevice(pDevice)
		, m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrInstanceBuffer(ptrBuffer)
		, m_offset(offset)
		, m_size(size)
		, m_data(data)
	{
		m_ptrTransferBuffer = new CVKTransferBuffer(pDevice, size);
		m_ptrTransferBuffer->BufferData(0, size, data);

		Execute();
	}
	CVKCommandBufferData(CVKDevice* pDevice, VkCommandBuffer vkCommandBuffer, const CGfxIndirectBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
		: m_pDevice(pDevice)
		, m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrIndirectBuffer(ptrBuffer)
		, m_offset(offset)
		, m_size(size)
		, m_data(data)
	{
		m_ptrTransferBuffer = new CVKTransferBuffer(pDevice, size);
		m_ptrTransferBuffer->BufferData(0, size, data);

		Execute();
	}
	CVKCommandBufferData(CVKDevice* pDevice, VkCommandBuffer vkCommandBuffer, const CGfxUniformBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
		: m_pDevice(pDevice)
		, m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrUniformBuffer(ptrBuffer)
		, m_offset(offset)
		, m_size(size)
		, m_data(data)
	{
		m_ptrTransferBuffer = new CVKTransferBuffer(pDevice, size);
		m_ptrTransferBuffer->BufferData(0, size, data);

		Execute();
	}
	CVKCommandBufferData(CVKDevice* pDevice, VkCommandBuffer vkCommandBuffer, const CGfxStorageBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
		: m_pDevice(pDevice)
		, m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrStorageBuffer(ptrBuffer)
		, m_offset(offset)
		, m_size(size)
		, m_data(data)
	{
		m_ptrTransferBuffer = new CVKTransferBuffer(pDevice, size);
		m_ptrTransferBuffer->BufferData(0, size, data);

		Execute();
	}
	virtual ~CVKCommandBufferData(void)
	{

	}

public:
	virtual void Execute(void)
	{
		ASSERT(m_vkCommandBuffer);

		VkBuffer vkSrcBuffer;
		VkBuffer vkDstBuffer;
		VkBufferUsageFlags vkBufferUsageFlags;

		if (m_ptrIndexBuffer) {
			vkSrcBuffer = ((CVKTransferBuffer*)m_ptrTransferBuffer.GetPointer())->GetBuffer();
			vkDstBuffer = ((CVKIndexBuffer*)m_ptrIndexBuffer.GetPointer())->GetBuffer();
			vkBufferUsageFlags = ((CVKIndexBuffer*)m_ptrIndexBuffer.GetPointer())->GetBufferUsageFlags();
		}
		else if (m_ptrVertexBuffer) {
			vkSrcBuffer = ((CVKTransferBuffer*)m_ptrTransferBuffer.GetPointer())->GetBuffer();
			vkDstBuffer = ((CVKIndexBuffer*)m_ptrVertexBuffer.GetPointer())->GetBuffer();
			vkBufferUsageFlags = ((CVKIndexBuffer*)m_ptrVertexBuffer.GetPointer())->GetBufferUsageFlags();
		}
		else if (m_ptrInstanceBuffer) {
			vkSrcBuffer = ((CVKTransferBuffer*)m_ptrTransferBuffer.GetPointer())->GetBuffer();
			vkDstBuffer = ((CVKIndexBuffer*)m_ptrInstanceBuffer.GetPointer())->GetBuffer();
			vkBufferUsageFlags = ((CVKIndexBuffer*)m_ptrInstanceBuffer.GetPointer())->GetBufferUsageFlags();
		}
		else if (m_ptrIndirectBuffer) {
			vkSrcBuffer = ((CVKTransferBuffer*)m_ptrTransferBuffer.GetPointer())->GetBuffer();
			vkDstBuffer = ((CVKIndexBuffer*)m_ptrIndirectBuffer.GetPointer())->GetBuffer();
			vkBufferUsageFlags = ((CVKIndexBuffer*)m_ptrIndirectBuffer.GetPointer())->GetBufferUsageFlags();
		}
		else if (m_ptrUniformBuffer) {
			vkSrcBuffer = ((CVKTransferBuffer*)m_ptrTransferBuffer.GetPointer())->GetBuffer();
			vkDstBuffer = ((CVKIndexBuffer*)m_ptrUniformBuffer.GetPointer())->GetBuffer();
			vkBufferUsageFlags = ((CVKIndexBuffer*)m_ptrUniformBuffer.GetPointer())->GetBufferUsageFlags();
		}
		else if (m_ptrStorageBuffer) {
			vkSrcBuffer = ((CVKTransferBuffer*)m_ptrTransferBuffer.GetPointer())->GetBuffer();
			vkDstBuffer = ((CVKIndexBuffer*)m_ptrStorageBuffer.GetPointer())->GetBuffer();
			vkBufferUsageFlags = ((CVKIndexBuffer*)m_ptrStorageBuffer.GetPointer())->GetBufferUsageFlags();
		}
		else {
			ASSERT(0);
		}

		vkCmdBufferData(m_vkCommandBuffer, vkSrcBuffer, 0, vkDstBuffer, m_offset, m_size);
		vkCmdBufferMemoryBarrier(m_vkCommandBuffer, vkDstBuffer, VK_ACCESS_TRANSFER_WRITE_BIT, CVKHelper::GetAccessMaskByBufferUsage(vkBufferUsageFlags), VK_PIPELINE_STAGE_TRANSFER_BIT, CVKHelper::GetPipelineStageFlagsByBufferUsage(vkBufferUsageFlags), m_offset, m_size);
	}


private:
	size_t m_offset;
	size_t m_size;
	const void* m_data;

	CGfxTransferBufferPtr m_ptrTransferBuffer;

private:
	CGfxIndexBufferPtr m_ptrIndexBuffer;
	CGfxVertexBufferPtr m_ptrVertexBuffer;
	CGfxInstanceBufferPtr m_ptrInstanceBuffer;
	CGfxIndirectBufferPtr m_ptrIndirectBuffer;
	CGfxUniformBufferPtr m_ptrUniformBuffer;
	CGfxStorageBufferPtr m_ptrStorageBuffer;

private:
	VkCommandBuffer m_vkCommandBuffer;

private:
	CVKDevice* m_pDevice;
};
