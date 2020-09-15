#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBufferData : public CGfxCommandBase
{
public:
	CVKCommandBufferData(VkCommandBuffer vkCommandBuffer, const CGfxIndexBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrIndexBuffer(ptrBuffer)
		, m_offset(offset)
		, m_size(size)
		, m_data(data)
	{
		Execute();
	}
	CVKCommandBufferData(VkCommandBuffer vkCommandBuffer, const CGfxVertexBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrVertexBuffer(ptrBuffer)
		, m_offset(offset)
		, m_size(size)
		, m_data(data)
	{
		Execute();
	}
	CVKCommandBufferData(VkCommandBuffer vkCommandBuffer, const CGfxInstanceBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrInstanceBuffer(ptrBuffer)
		, m_offset(offset)
		, m_size(size)
		, m_data(data)
	{
		Execute();
	}
	CVKCommandBufferData(VkCommandBuffer vkCommandBuffer, const CGfxIndirectBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrIndirectBuffer(ptrBuffer)
		, m_offset(offset)
		, m_size(size)
		, m_data(data)
	{
		Execute();
	}
	CVKCommandBufferData(VkCommandBuffer vkCommandBuffer, const CGfxUniformBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrUniformBuffer(ptrBuffer)
		, m_offset(offset)
		, m_size(size)
		, m_data(data)
	{
		Execute();
	}
	CVKCommandBufferData(VkCommandBuffer vkCommandBuffer, const CGfxStorageBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrStorageBuffer(ptrBuffer)
		, m_offset(offset)
		, m_size(size)
		, m_data(data)
	{
		Execute();
	}
	virtual ~CVKCommandBufferData(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);

		if (m_ptrIndexBuffer) {

		}

		if (m_ptrVertexBuffer) {

		}

		if (m_ptrInstanceBuffer) {

		}

		if (m_ptrIndirectBuffer) {

		}

		if (m_ptrUniformBuffer) {

		}

		if (m_ptrStorageBuffer) {

		}
	}


private:
	size_t m_offset;
	size_t m_size;
	const void* m_data;

private:
	CGfxTransferBufferPtr m_ptrTransferBuffer;

	CGfxIndexBufferPtr m_ptrIndexBuffer;
	CGfxVertexBufferPtr m_ptrVertexBuffer;
	CGfxInstanceBufferPtr m_ptrInstanceBuffer;
	CGfxIndirectBufferPtr m_ptrIndirectBuffer;
	CGfxUniformBufferPtr m_ptrUniformBuffer;
	CGfxStorageBufferPtr m_ptrStorageBuffer;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
