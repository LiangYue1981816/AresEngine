#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandUniform4f : public CGfxCommandBase
{
public:
	CGLES3CommandUniform4f(const CGfxPipelineCompute* pPipelineCompute, const CGfxPipelineGraphics* pPipelineGraphics, uint32_t name, float v0, float v1, float v2, float v3)
		: m_pPipelineCompute((CGLES3PipelineCompute*)pPipelineCompute)
		, m_pPipelineGraphics((CGLES3PipelineGraphics*)pPipelineGraphics)
		, m_name(name)
		, m_v0(v0)
		, m_v1(v1)
		, m_v2(v2)
		, m_v3(v3)
	{

	}
	virtual ~CGLES3CommandUniform4f(void)
	{

	}

public:
	virtual void Execute(void)
	{
		ASSERT(m_pPipelineCompute || m_pPipelineGraphics);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_UNIFORM4F);
		{
			if (m_pPipelineCompute) {
				m_pPipelineCompute->Uniform4f(m_name, m_v0, m_v1, m_v2, m_v3);
			}

			if (m_pPipelineGraphics) {
				m_pPipelineGraphics->Uniform4f(m_name, m_v0, m_v1, m_v2, m_v3);
			}
		}
	}


private:
	uint32_t m_name;
	float m_v0;
	float m_v1;
	float m_v2;
	float m_v3;

private:
	CGLES3PipelineCompute* m_pPipelineCompute;
	CGLES3PipelineGraphics* m_pPipelineGraphics;
};
