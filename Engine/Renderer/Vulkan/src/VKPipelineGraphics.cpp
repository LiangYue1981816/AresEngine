#include "VKRenderer.h"


CVKPipelineGraphics::CVKPipelineGraphics(CVKDevice* pDevice, uint32_t name, VkPipelineCache vkPipelineCache, const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding)
	: CGfxPipelineGraphics(name)
	, m_pDevice(pDevice)
	, m_pPipeline(nullptr)
{
	m_pPipeline = new CVKPipeline(pDevice, vkPipelineCache, pRenderPass, pVertexShader, pFragmentShader, state, indexSubpass, vertexBinding, instanceBinding);
}

CVKPipelineGraphics::~CVKPipelineGraphics(void)
{
	delete m_pPipeline;
}

const VkPipelineLayout CVKPipelineGraphics::GetPipelineLayout(void) const
{
	return m_pPipeline->GetPipelineLayout();
}

const CGfxDescriptorLayoutPtr CVKPipelineGraphics::GetDescriptorLayout(int indexDescriptorSet) const
{
	return m_pPipeline->GetDescriptorLayout(indexDescriptorSet);
}

const uint32_t CVKPipelineGraphics::GetInputAttachmentName(int indexInputAttachment) const
{
	return m_pPipeline->GetInputAttachmentName(indexInputAttachment);
}

bool CVKPipelineGraphics::IsCompatibleVertexFormat(uint32_t binding, uint32_t format) const
{
	return m_pPipeline->IsCompatibleVertexFormat(binding, format);
}

bool CVKPipelineGraphics::IsTransparency(void) const
{
	return m_state.bEnableBlend;
}
