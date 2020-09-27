#pragma once
#include "VKRenderer.h"


class CVKPipelineGraphics : public CGfxPipelineGraphics
{
public:
	CVKPipelineGraphics(uint32_t name, CVKDevice* pDevice, VkPipelineCache vkPipelineCache, const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding);
	virtual ~CVKPipelineGraphics(void);


public:
	const VkPipelineLayout GetPipelineLayout(void) const;
	const CGfxDescriptorLayoutPtr GetDescriptorLayout(int indexDescriptorSet) const;
	const uint32_t GetInputAttachmentName(int indexInputAttachment) const;

public:
	bool IsCompatibleVertexFormat(uint32_t binding, uint32_t format) const;
	bool IsTransparency(void) const;


private:
	PipelineState m_state;

private:
	CVKPipeline* m_pPipeline;

private:
	CVKDevice* m_pDevice;
};
