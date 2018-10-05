#pragma once
#include "GfxRenderer.h"
#include "GfxPipelineBase.h"


class CGfxPipelineCompute : public CGfxPipelineBase
{
	friend class CGfxRenderer;
	friend class CGfxPipelineManager;


private:
	CGfxPipelineCompute(uint32_t name);
	virtual ~CGfxPipelineCompute(void);


private:
	bool Create(const CGfxShader *pComputeShader);
	void CreateProgram(const CGfxShader *pComputeShader);
	void Destroy(void);

public:
	void BindPipeline(void) const;
};
