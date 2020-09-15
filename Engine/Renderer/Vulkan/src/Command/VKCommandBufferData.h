#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandBufferData : public CGfxCommandBase
{
public:
	CVKCommandBufferData(VkCommandBuffer vkCommandBuffer, const CGfxIndexBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrIndexBuffer(ptrBuffer)
	{
		Execute();
	}
	CVKCommandBufferData(VkCommandBuffer vkCommandBuffer, const CGfxVertexBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrVertexBuffer(ptrBuffer)
	{
		Execute();
	}
	CVKCommandBufferData(VkCommandBuffer vkCommandBuffer, const CGfxInstanceBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrInstanceBuffer(ptrBuffer)
	{
		Execute();
	}
	CVKCommandBufferData(VkCommandBuffer vkCommandBuffer, const CGfxIndirectBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrIndirectBuffer(ptrBuffer)
	{
		Execute();
	}
	CVKCommandBufferData(VkCommandBuffer vkCommandBuffer, const CGfxUniformBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrUniformBuffer(ptrBuffer)
	{
		Execute();
	}
	CVKCommandBufferData(VkCommandBuffer vkCommandBuffer, const CGfxStorageBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrStorageBuffer(ptrBuffer)
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
	}


private:
	CGfxIndexBufferPtr m_ptrIndexBuffer;
	CGfxVertexBufferPtr m_ptrVertexBuffer;
	CGfxInstanceBufferPtr m_ptrInstanceBuffer;
	CGfxIndirectBufferPtr m_ptrIndirectBuffer;
	CGfxUniformBufferPtr m_ptrUniformBuffer;
	CGfxStorageBufferPtr m_ptrStorageBuffer;
	CGfxTransferBufferPtr m_ptrTransferBuffer;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
