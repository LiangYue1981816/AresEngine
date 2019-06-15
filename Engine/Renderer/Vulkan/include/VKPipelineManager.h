#pragma once
#include "VKRenderer.h"


class CVKPipelineComputeManager : public CGfxResourceManager
{
	friend class CVKRenderer;


private:
	CVKPipelineComputeManager(CVKDevice* pDevice);
	virtual ~CVKPipelineComputeManager(void);


private:
	CVKPipelineCompute* Create(const CGfxShader* pComputeShader);


private:
	VkPipelineCache m_vkPipelineCache;
	eastl::unordered_map<uint32_t, CVKPipelineCompute*> m_pPipelines;

private:
	CVKDevice* m_pDevice;
};


class CVKPipelineGraphicsManager : public CGfxResourceManager
{
	friend class CVKRenderer;


private:
	CVKPipelineGraphicsManager(CVKDevice* pDevice);
	virtual ~CVKPipelineGraphicsManager(void);


private:
	CVKPipelineGraphics* Create(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, uint32_t indexSubpass, uint32_t vertexBinding, uint32_t instanceBinding);


private:
	VkPipelineCache m_vkPipelineCache;
	eastl::unordered_map<uint32_t, CVKPipelineGraphics*> m_pPipelines;

private:
	CVKDevice* m_pDevice;
};
