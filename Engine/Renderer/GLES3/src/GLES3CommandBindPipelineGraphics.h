#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindPipelineGraphics : public CGfxCommandBase
{
public:
	CGLES3CommandBindPipelineGraphics(const CGfxPipelineGraphics *pPipelineGraphics)
		: m_pPipelineGraphics((CGfxPipelineGraphics *)pPipelineGraphics)
	{

	}
	virtual ~CGLES3CommandBindPipelineGraphics(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_PIPELINEGRAPHICS, "CommandBindPipelineGraphics");
		{
			if (m_pPipelineGraphics) {
				GLES3Renderer()->BindPipelineGraphics(m_pPipelineGraphics);
			}
			else {
#if GLES_VER == 310
				GLBindProgramPipeline(0);
#else
				GLUseProgram(0);
#endif
			}
		}
	}


private:
	CGfxPipelineGraphics *m_pPipelineGraphics;
};
