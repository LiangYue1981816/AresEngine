#include "VKRenderer.h"


CVKPipelineGraphics::CVKPipelineGraphics(CVKDevice* pDevice, uint32_t name)
	: CGfxPipelineGraphics(name)
	, m_pDevice(pDevice)
	, m_pPipeline(nullptr)
{
	ASSERT(m_pDevice);

	m_pPipeline = new CVKPipeline(m_pPipeline);
}

CVKPipelineGraphics::~CVKPipelineGraphics(void)
{
	delete m_pPipeline;
}

const CGfxDescriptorLayoutPtr CVKPipelineGraphics::GetDescriptorLayout(int indexDescriptorSet) const
{
	return m_pPipeline->GetDescriptorLayout(indexDescriptorSet);
}

bool CVKPipelineGraphics::Create(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding)
{
	return m_pPipeline->Create(pRenderPass, pVertexShader, pFragmentShader, state, indexSubpass, vertexBinding, instanceBinding);
}

void CVKPipelineGraphics::Destroy(void)
{
	m_pPipeline->Destroy();
}

void CVKPipelineGraphics::Bind(VkCommandBuffer vkCommandBuffer)
{
	m_pPipeline->Bind(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS);
}
