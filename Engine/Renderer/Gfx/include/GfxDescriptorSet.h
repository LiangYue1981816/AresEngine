#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxDescriptorSet : public CGfxResource
{
public:
	CGfxDescriptorSet(const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
	{

	}
	virtual ~CGfxDescriptorSet(void)
	{

	}


public:
	virtual HANDLE GetDescriptorSet(void) const = 0;
	virtual CGfxDescriptorLayoutPtr GetDescriptorLayout(void) const = 0;

public:
	virtual bool SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrUniformBuffer) = 0;
	virtual bool SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture, const CGfxSampler *pSampler) = 0;
	virtual bool SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture, const CGfxSampler *pSampler) = 0;
	virtual bool SetTextureCubeMap(uint32_t name, const CGfxTextureCubeMapPtr ptrTexture, const CGfxSampler *pSampler) = 0;
	virtual bool SetRenderTexture(uint32_t name, const CGfxRenderTexturePtr ptrRenderTexture, const CGfxSampler *pSampler) = 0;

public:
	virtual CGfxSampler* GetSampler(uint32_t name) const = 0;
	virtual CGfxTexture2DPtr GetTexture2D(uint32_t name) const = 0;
	virtual CGfxTexture2DArrayPtr GetTexture2DArray(uint32_t name) const = 0;
	virtual CGfxTextureCubeMapPtr GetTextureCubeMap(uint32_t name) const = 0;
	virtual CGfxRenderTexturePtr GetRenderTexture(uint32_t name) const = 0;
};
