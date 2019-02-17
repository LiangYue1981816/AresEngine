#if GLES_VER == 300

#pragma once
#include "GLES3Renderer.h"


class CGLES3PipelineCompute : public CGLES3Pipeline, public CGfxPipelineCompute
{
	friend class CGLES3PipelineComputeManager;


private:
	CGLES3PipelineCompute(uint32_t name);
	virtual ~CGLES3PipelineCompute(void);


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
	void Bind(void);


private:
	uint32_t m_name;
};

#endif
