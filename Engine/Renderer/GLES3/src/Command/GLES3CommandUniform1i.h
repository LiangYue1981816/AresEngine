#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniform1i : public CGfxCommandBase
{
public:
	CGLES3CommandUniform1i(const char *szName, int v0)
		: m_name(HashValue(szName))
		, m_v0(v0)
	{

	}
	virtual ~CGLES3CommandUniform1i(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM1I, "CommandUniform1i");
		{
			GLES3Renderer()->GetCurrentPipelineGraphics()->Uniform1i(m_name, m_v0);
		}
	}


private:
	uint32_t m_name;
	int m_v0;
};
