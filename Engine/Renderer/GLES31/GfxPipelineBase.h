#pragma once
#include "GfxRenderer.h"


class CGfxPipelineBase
{
	friend class CGfxRenderer;
	friend class CGfxPipelineManager;


protected:
	CGfxPipelineBase(uint32_t name);
	virtual ~CGfxPipelineBase(void);

public:
	uint32_t GetName(void) const;


public:
	virtual bool IsUniformValid(uint32_t name) const;
	virtual bool IsTextureValid(uint32_t name) const;

public:
	virtual void BindPipeline(void) const = 0;
	virtual bool BindTexture2D(uint32_t name, uint32_t texture, uint32_t sampler, uint32_t unit) const;
	virtual bool BindTexture2DArray(uint32_t name, uint32_t texture, uint32_t sampler, uint32_t unit) const;
	virtual bool BindTextureCubeMap(uint32_t name, uint32_t texture, uint32_t sampler, uint32_t unit) const;
	virtual bool BindUniformBuffer(uint32_t name, uint32_t buffer, uint32_t size, int offset = 0) const;


protected:
	uint32_t m_name;

protected:
	uint32_t m_pipeline;
	CGfxShader *m_pShaders[shaderc_compute_shader - shaderc_vertex_shader + 1];
};
