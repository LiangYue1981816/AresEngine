#pragma once
#include "PreHeader.h"


typedef struct DescriptorBufferInfo {
	DescriptorBufferInfo(void)
	{
		bDirty = false;
	}

	void Set(uint32_t _binding, uint32_t _offset, uint32_t _range, CGfxUniformBufferPtr _ptrUniformBuffer, CGfxStorageBufferPtr _ptrStorageBuffer)
	{
		bDirty = true;
		binding = _binding;
		offset = _offset;
		range = _range;
		ptrUniformBuffer = _ptrUniformBuffer;
		ptrStorageBuffer = _ptrStorageBuffer;
	}

	bool bDirty;
	uint32_t binding;
	uint32_t offset;
	uint32_t range;
	CGfxUniformBufferPtr ptrUniformBuffer;
	CGfxStorageBufferPtr ptrStorageBuffer;
} DescriptorBufferInfo;

typedef struct DescriptorImageInfo {
	DescriptorImageInfo(void)
	{
		bDirty = false;
	}

	void Set(uint32_t _binding, CGfxSampler* _pSampler, CGfxTexture2DPtr _ptrTexture2D, CGfxTexture2DArrayPtr _ptrTexture2DArray, CGfxTextureCubemapPtr _ptrTextureCubemap, CGfxRenderTexturePtr _ptrRenderTexture, CGfxRenderTexturePtr _ptrInputAttachmentTexture)
	{
		bDirty = true;
		binding = _binding;
		pSampler = _pSampler;
		ptrTexture2D = _ptrTexture2D;
		ptrTexture2DArray = _ptrTexture2DArray;
		ptrTextureCubemap = _ptrTextureCubemap;
		ptrRenderTexture = _ptrRenderTexture;
		ptrInputAttachmentTexture = _ptrInputAttachmentTexture;
	}

	bool bDirty;
	uint32_t binding;
	CGfxSampler* pSampler;
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
	virtual bool SetStorageBuffer(uint32_t name, const CGfxStorageBufferPtr ptrStorageBuffer, uint32_t offset, uint32_t range) = 0;

public:
	virtual const CGfxDescriptorLayoutPtr GetDescriptorLayout(void) const = 0;
	virtual const DescriptorImageInfo* GetDescriptorImageInfo(uint32_t name) const = 0;
	virtual const DescriptorBufferInfo* GetDescriptorBufferInfo(uint32_t name) const = 0;


private:
	uint32_t m_name;
};
