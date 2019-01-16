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

bool CVKPipelineGraphics::Create(const CGfxRenderPassPtr &ptrRenderPass, const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state)
{
	if (pVertexShader == nullptr) {
		return false;
	}

	if (pVertexShader->IsValid() == false) {
		return false;
	}

	if (pVertexShader->GetKind() != vertex_shader) {
		return false;
	}

	if (pFragmentShader == nullptr) {
		return false;
	}

	if (pFragmentShader->IsValid() == false) {
		return false;
	}

	if (pFragmentShader->GetKind() != fragment_shader) {
		return false;
	}

	Destroy();

	VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
	pipelineCreateInfo.sType;
	pipelineCreateInfo.pNext;
	pipelineCreateInfo.flags;
	pipelineCreateInfo.stageCount;
	pipelineCreateInfo.pStages;
	pipelineCreateInfo.pVertexInputState;
	pipelineCreateInfo.pInputAssemblyState;
	pipelineCreateInfo.pTessellationState;
	pipelineCreateInfo.pViewportState;
	pipelineCreateInfo.pRasterizationState;
	pipelineCreateInfo.pMultisampleState;
	pipelineCreateInfo.pDepthStencilState;
	pipelineCreateInfo.pColorBlendState;
	pipelineCreateInfo.pDynamicState;
	pipelineCreateInfo.layout;
	pipelineCreateInfo.renderPass;
	pipelineCreateInfo.subpass;
	pipelineCreateInfo.basePipelineHandle;
	pipelineCreateInfo.basePipelineIndex;

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
