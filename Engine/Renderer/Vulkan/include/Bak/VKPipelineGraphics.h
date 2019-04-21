#pragma once
#include "VKRenderer.h"


class CVKPipelineGraphics : public CVKPipeline, public CGfxPipelineGraphics
{
	friend class CVKPipelineGraphicsManager;


private:
	CVKPipelineGraphics(CVKDevice* pDevice, CVKPipelineGraphicsManager* pManager, uint32_t name);
	virtual ~CVKPipelineGraphics(void);


public:
	uint32_t GetName(void) const;

public:
	HANDLE GetPipeline(void) const;

public:
	bool Create(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, uint32_t indexSubpass, uint32_t vertexBinding, uint32_t instanceBinding);
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
	CVKPipelineGraphicsManager* m_pManager;
};
