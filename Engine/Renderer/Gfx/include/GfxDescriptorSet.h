#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxDescriptorSet : public CGfxResource
{
public:
	typedef struct DescriptorImageInfo {
		CGfxSampler *pSampler;
		CGfxTexture2DPtr ptrTexture2D;
		CGfxTexture2DArrayPtr ptrTexture2DArray;
		CGfxTextureCubeMapPtr ptrTextureCubeMap;
		CGfxRenderTexturePtr ptrRenderTexture;
	} DescriptorImageInfo;

	typedef struct DescriptorBufferInfo {
		uint32_t offset;
		uint32_t range;
		CGfxUniformBufferPtr ptrUniformBuffer;
	} DescriptorBufferInfo;


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
	virtual bool SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture, const CGfxSampler *pSampler) = 0;
	virtual bool SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture, const CGfxSampler *pSampler) = 0;
	virtual bool SetTextureCubeMap(uint32_t name, const CGfxTextureCubeMapPtr ptrTexture, const CGfxSampler *pSampler) = 0;
	virtual bool SetRenderTexture(uint32_t name, const CGfxRenderTexturePtr ptrRenderTexture, const CGfxSampler *pSampler) = 0;
	virtual bool SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrUniformBuffer, uint32_t offset, uint32_t range) = 0;

public:
	virtual const DescriptorImageInfo* GetDescriptorImageInfo(uint32_t name) const = 0;
	virtual const DescriptorBufferInfo* GetDescriptorBufferInfo(uint32_t name) const = 0;
};
