#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniform4i : public CGfxCommandBase
{
public:
	CGLES3CommandUniform4i(const char *szName, int v0, int v1, int v2, int v3)
		: m_name(HashValue(szName))
		, m_v0(v0)
		, m_v1(v1)
		, m_v2(v2)
		, m_v3(v3)
	{

	}
	virtual ~CGLES3CommandUniform4i(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM4I, "CommandUniform4i");
		{
			GLES3Renderer()->GetCurrentPipelineGraphics()->Uniform4i(m_name, m_v0, m_v1, m_v2, m_v3);
		}
	}


private:
	uint32_t m_name;
	int m_v0;
	int m_v1;
	int m_v2;
	int m_v3;
};
