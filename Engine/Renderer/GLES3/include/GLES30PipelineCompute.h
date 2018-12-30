#if GLES_VER == 300

#pragma once
#include "GLES3Renderer.h"


class CGLES3PipelineCompute : public CGLES3Pipeline, public CGfxPipelineCompute
{
	friend class CGLES3PipelineManager;


private:
	CGLES3PipelineCompute(uint32_t name);
	virtual ~CGLES3PipelineCompute(void);


public:
	bool Create(const CGfxShader *pComputeShader);
	void Destroy(void);

public:
	bool IsTextureValid(uint32_t name) const;
	bool IsUniformValid(uint32_t name) const;
	bool IsUniformBlockValid(uint32_t name) const;

public:
	void Bind(void);
};

#endif
