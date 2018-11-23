#if GLES_VER == 300

#include "GfxHeader.h"


CGLES3PipelineCompute::CGLES3PipelineCompute(uint32_t name)
	: CGLES3PipelineBase(name)
{

}

CGLES3PipelineCompute::~CGLES3PipelineCompute(void)
{
	Destroy();
}

bool CGLES3PipelineCompute::Create(const CGfxShader *pComputeShader)
{
	return true;
}

void CGLES3PipelineCompute::Destroy(void)
{

}

void CGLES3PipelineCompute::Bind(void *pParam)
{

}

#endif
