#pragma once
#include "VKRenderer.h"


class CVKPipelineCompute : public CGfxPipelineCompute
{
	friend class CVKPipelineComputeManager;


private:
	CVKPipelineCompute(CVKDevice* pDevice, uint32_t name);
	virtual ~CVKPipelineCompute(void);


public:
	const CGfxDescriptorLayoutPtr GetDescriptorLayout(int indexDescriptorSet) const;

public:
	bool Create(const CGfxShader* pComputeShader);
	void Destroy(void);

public:
	void Bind(VkCommandBuffer vkCommandBuffer);


private:
	CVKPipeline* m_pPipeline;

private:
	CVKDevice* m_pDevice;
};
