#include "VKRenderer.h"


CVKPipelineGraphics::CVKPipelineGraphics(CVKDevice *pDevice, CVKPipelineGraphicsManager *pManager, uint32_t name)
	: CVKPipeline(pDevice)
	, CGfxPipelineGraphics(name)
	, m_name(name)
	, m_pManager(pManager)
{

}

CVKPipelineGraphics::~CVKPipelineGraphics(void)
{

}

uint32_t CVKPipelineGraphics::GetName(void) const
{
	return m_name;
}

bool CVKPipelineGraphics::Create(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state)
{
	return true;
}

void CVKPipelineGraphics::Destroy(void)
{

}

bool CVKPipelineGraphics::IsTextureValid(uint32_t name) const
{
	return true;
}

bool CVKPipelineGraphics::IsUniformValid(uint32_t name) const
{
	return true;
}

bool CVKPipelineGraphics::IsUniformBlockValid(uint32_t name) const
{
	return true;
}
