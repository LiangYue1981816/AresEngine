#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindEngine : public CGfxCommandBase
{
public:
	CGLES3CommandBindEngine(CGfxUniformEnginePtr &ptrUniformEngine)
		: m_ptrUniformEngine(ptrUniformEngine)
	{

	}
	virtual ~CGLES3CommandBindEngine(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_ENGINE, "CommandBindEngine");
		{
			if (m_ptrUniformEngine.IsValid()) {
				GLES3Renderer()->BindUniformEngine(m_ptrUniformEngine);
			}
		}
	}


private:
	CGfxUniformEnginePtr m_ptrUniformEngine;
};
