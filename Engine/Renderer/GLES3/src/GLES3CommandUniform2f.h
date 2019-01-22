#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniform2f : public CGfxCommandBase
{
public:
	CGLES3CommandUniform2f(const char *szName, float v0, float v1)
		: m_name(HashValue(szName))
		, m_v0(v0)
		, m_v1(v1)
	{

	}
	virtual ~CGLES3CommandUniform2f(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM2F, "CommandUniform2f");
		{
			GLES3Renderer()->GetCurrentPipelineGraphics()->Uniform2f(m_name, m_v0, m_v1);
		}
	}


private:
	uint32_t m_name;
	float m_v0;
	float m_v1;
};
