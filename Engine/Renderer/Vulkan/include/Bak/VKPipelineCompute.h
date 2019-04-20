#pragma once
#include "VKRenderer.h"


class CVKPipelineCompute : public CVKPipeline, public CGfxPipelineCompute
{
	friend class CVKPipelineComputeManager;


private:
	CVKPipelineCompute(CVKDevice *pDevice, CVKPipelineComputeManager *pManager, uint32_t name);
	virtual ~CVKPipelineCompute(void);


public:
	uint32_t GetName(void) const;

public:
	HANDLE GetPipeline(void) const;

public:
	bool Create(const CGfxShader *pComputeShader);
	void Destroy(void);

public:
	bool IsTextureValid(uint32_t name) const;
	bool IsUniformValid(uint32_t name) const;
	bool IsUniformBlockValid(uint32_t name) const;

public:
	void Bind(VkCommandBuffer vkCommandBuffer) const;


private:
	uint32_t m_name;

private:
	CVKPipelineComputeManager *m_pManager;
};
