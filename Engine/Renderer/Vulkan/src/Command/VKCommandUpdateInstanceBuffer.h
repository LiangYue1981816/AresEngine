#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandUpdateInstanceBuffer : public CGfxCommandBase
{
public:
	CVKCommandUpdateInstanceBuffer(VkCommandBuffer vkCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size)
		: m_ptrMeshDraw(ptrMeshDraw)
	{
		if (pInstanceBuffer && size) {
			m_buffer.assign(pInstanceBuffer, pInstanceBuffer + size);
			Execute(vkCommandBuffer);
		}
	}
	virtual ~CVKCommandUpdateInstanceBuffer(void)
	{

	}

public:
	virtual void Execute(VkCommandBuffer vkCommandBuffer) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UPDATE_INSTANCEBUFFER, "CommandUpdateInstanceBuffer");
		{
			if (m_ptrMeshDraw && m_buffer.empty() == false) {
				m_ptrMeshDraw->InstanceBufferData(m_buffer.size(), m_buffer.data());
			}
		}
	}


private:
	CGfxMeshDrawPtr m_ptrMeshDraw;
	eastl::vector<uint8_t> m_buffer;
};