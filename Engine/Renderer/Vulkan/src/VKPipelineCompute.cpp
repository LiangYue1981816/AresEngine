#include "VKRenderer.h"


CVKPipelineCompute::CVKPipelineCompute(CVKDevice *pDevice, CVKPipelineComputeManager *pManager, uint32_t name)
	: CVKPipeline(pDevice)
	, CGfxPipelineCompute(name)
	, m_name(name)
	, m_pManager(pManager)
{

}

CVKPipelineCompute::~CVKPipelineCompute(void)
{

}

uint32_t CVKPipelineCompute::GetName(void) const
{
	return m_name;
}

bool CVKPipelineCompute::Create(const CGfxShader *pComputeShader)
{
	return true;
}

void CVKPipelineCompute::Destroy(void)
{

}

bool CVKPipelineCompute::IsTextureValid(uint32_t name) const
{
	return true;
}

bool CVKPipelineCompute::IsUniformValid(uint32_t name) const
{
	return true;
}

bool CVKPipelineCompute::IsUniformBlockValid(uint32_t name) const
{
	return true;
}
