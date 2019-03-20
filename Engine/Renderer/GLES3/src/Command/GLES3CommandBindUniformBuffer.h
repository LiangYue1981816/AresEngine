#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindUniformBuffer : public CGfxCommandBase
{
public:
	CGLES3CommandBindUniformBuffer(const CGfxUniformBufferPtr ptrUniformBuffer, uint32_t nameUniform)
		: m_ptrUniformBuffer(ptrUniformBuffer)
		, m_nameUniform(nameUniform)
	{

	}
	virtual ~CGLES3CommandBindUniformBuffer(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_UNIFORMBUFFER, "CommandBindUniformBuffer");
		{
			if (m_ptrUniformBuffer.IsValid()) {
				GLES3Renderer()->BindUniformBuffer(m_ptrUniformBuffer, m_nameUniform);
			}
		}
	}


private:
	uint32_t m_nameUniform;
	CGfxUniformBufferPtr m_ptrUniformBuffer;
};
