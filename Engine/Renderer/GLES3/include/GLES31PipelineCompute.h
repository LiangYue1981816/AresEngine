#if GLES_VER == 310

#pragma once
#include "GLES3Renderer.h"


class CGLES3PipelineCompute : public CGLES3PipelineBase, public CGfxPipelineCompute
{
	friend class CGLES3PipelineManager;


private:
	CGLES3PipelineCompute(uint32_t name);
	virtual ~CGLES3PipelineCompute(void);


private:
	bool Create(const CGfxShader *pComputeShader);
	void Destroy(void);

public:
	void Bind(void *pParam);
};

#endif
