#pragma once
#include "VKRenderer.h"


class CVKPipelineGraphics : public CGfxPipelineGraphics
{
	friend class CVKPipelineGraphicsManager;


private:
	CVKPipelineGraphics(CVKDevice* pDevice, uint32_t name);
	virtual ~CVKPipelineGraphics(void);


public:
	const CGfxDescriptorLayoutPtr GetDescriptorLayout(int indexDescriptorSet) const;

public:
	bool Create(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding);
	void Destroy(void);

public:
	void Bind(VkCommandBuffer vkCommandBuffer);


private:
	CVKPipeline* m_pPipeline;

private:
	CVKDevice* m_pDevice;
};
