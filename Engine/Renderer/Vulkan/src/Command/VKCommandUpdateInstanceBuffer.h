#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUpdateInstanceBuffer : public CGfxCommandBase
{
public:
	CVKCommandUpdateInstanceBuffer(VkCommandBuffer vkCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_ptrMeshDraw(ptrMeshDraw)
		, m_buffer(pInstanceBuffer, pInstanceBuffer + size)
	{
		Execute();
	}
	virtual ~CVKCommandUpdateInstanceBuffer(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		ASSERT(m_vkCommandBuffer);
		ASSERT(m_ptrMeshDraw);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UPDATE_INSTANCEBUFFER, "CommandUpdateInstanceBuffer");
		{
			if (m_buffer.empty() == false) {
				m_ptrMeshDraw->InstanceBufferData(m_buffer.size(), m_buffer.data());
			}
		}
	}


private:
	CGfxMeshDrawPtr m_ptrMeshDraw;
	eastl::vector<uint8_t> m_buffer;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
