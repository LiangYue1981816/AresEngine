#pragma once
#include "GLES3Renderer.h"


class CGLES3PipelineCompute : public CGLES3Pipeline, public CGfxPipelineCompute
{
	friend class CGLES3PipelineComputeManager;


private:
	CGLES3PipelineCompute(uint32_t name);
	virtual ~CGLES3PipelineCompute(void);


public:
	HANDLE GetPipeline(void) const;

public:
	bool Create(const CGfxShader *pComputeShader);
	void Destroy(void);

public:
	void Bind(void) const;
};
