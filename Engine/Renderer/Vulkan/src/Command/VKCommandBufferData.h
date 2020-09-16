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
