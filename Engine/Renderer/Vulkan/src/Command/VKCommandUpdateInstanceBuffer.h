#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUpdateInstanceBuffer : public CGfxCommandBase
{
public:
	CVKCommandUpdateInstanceBuffer(VkCommandBuffer vkCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrMeshDraw(ptrMeshDraw)
	{
		if (pInstanceBuffer && size) {
			m_buffer.assign(pInstanceBuffer, pInstanceBuffer + size);
			Execute();
		}
	}
	virtual ~CVKCommandUpdateInstanceBuffer(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);
		ASSERT(m_ptrMeshDraw);
		ASSERT(m_buffer.size());

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UPDATE_INSTANCEBUFFER, "CommandUpdateInstanceBuffer");
		{
			m_ptrMeshDraw->InstanceBufferData(m_buffer.size(), m_buffer.data());
		}
	}


private:
	CGfxMeshDrawPtr m_ptrMeshDraw;
	eastl::vector<uint8_t> m_buffer;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
