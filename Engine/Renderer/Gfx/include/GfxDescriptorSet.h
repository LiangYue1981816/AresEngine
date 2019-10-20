#pragma once
#include "PreHeader.h"


typedef struct DescriptorBufferInfo {
	bool bDirty = false;
	uint32_t offset = 0;
	uint32_t range = 0;
	CGfxStorageBufferPtr ptrStorageBuffer;
	CGfxUniformBufferPtr ptrUniformBuffer;
} DescriptorBufferInfo;

typedef struct DescriptorImageInfo {
	bool bDirty = false;
	CGfxSampler* pSampler = nullptr;
	CGfxTexture2DPtr ptrTexture2D;
	CGfxTexture2DArrayPtr ptrTexture2DArray;
	CGfxTextureCubemapPtr ptrTextureCubemap;
	CGfxRenderTexturePtr ptrRenderTexture;
	CGfxRenderTexturePtr ptrInputAttachmentTexture;
} DescriptorImageInfo;

class CALL_API CGfxDescriptorSet : public CGfxResource
{
public:
	CGfxDescriptorSet(uint32_t name, const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
		: m_name(name)
	{

	}
	virtual ~CGfxDescriptorSet(void)
	{

	}

	uint32_t GetName(void) const
	{
		return m_name;
	}


public:
	virtual bool SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture, const CGfxSampler* pSampler) = 0;
	virtual bool SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture, const CGfxSampler* pSampler) = 0;
	virtual bool SetTextureCubemap(uint32_t name, const CGfxTextureCubemapPtr ptrTexture, const CGfxSampler* pSampler) = 0;
	virtual bool SetRenderTexture(uint32_t name, const CGfxRenderTexturePtr ptrTexture, const CGfxSampler* pSampler) = 0;
	virtual bool SetInputAttachmentTexture(uint32_t name, const CGfxRenderTexturePtr ptrTexture, const CGfxSampler* pSampler) = 0;
	virtual bool SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrUniformBuffer, uint32_t offset, uint32_t range) = 0;

public:
	virtual const CGfxDescriptorLayoutPtr GetDescriptorLayout(void) const = 0;
	virtual const DescriptorImageInfo* GetDescriptorImageInfo(uint32_t name) const = 0;
	virtual const DescriptorBufferInfo* GetDescriptorBufferInfo(uint32_t name) const = 0;

public:
	virtual void Update(void) = 0;


private:
	uint32_t m_name;
};
