#if GLES_VER == 300

#include "GLES3Renderer.h"


CGLES3PipelineCompute::CGLES3PipelineCompute(uint32_t name)
	: CGfxPipelineCompute(name)
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

bool CGLES3PipelineCompute::IsTextureValid(uint32_t name) const
{
	return CGLES3PipelineBase::IsTextureValid(name);
}

bool CGLES3PipelineCompute::IsUniformValid(uint32_t name) const
{
	return CGLES3PipelineBase::IsUniformValid(name);
}

bool CGLES3PipelineCompute::IsUniformBlockValid(uint32_t name) const
{
	return CGLES3PipelineBase::IsUniformBlockValid(name);
}

void CGLES3PipelineCompute::Bind(void)
{

}

#endif
