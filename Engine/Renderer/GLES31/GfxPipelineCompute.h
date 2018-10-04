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
	bool CreateProgram(const CGfxShader *pComputeShader);

public:
	void BindPipeline(void) const;
};
