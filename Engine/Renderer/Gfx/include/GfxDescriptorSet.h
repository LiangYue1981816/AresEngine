#pragma once
#include "PreHeader.h"


typedef struct DescriptorBufferInfo {
	DescriptorBufferInfo(void)
	{
		bDirty = false;
	}

	void SetUniformBuffer(uint32_t _binding, CGfxUniformBufferPtr _ptrBuffer, uint32_t _offset, uint32_t _range)
	{
		bDirty = true;
		binding = _binding;
		offset = _offset;
		range = _range;
		ptrUniformBuffer = _ptrBuffer;
		ptrStorageBuffer.Release();
	}

	void SetStorageBuffer(uint32_t _binding, CGfxStorageBufferPtr _ptrBuffer, uint32_t _offset, uint32_t _range)
	{
		bDirty = true;
		binding = _binding;
		offset = _offset;
		range = _range;
		ptrUniformBuffer.Release();
		ptrStorageBuffer = _ptrBuffer;
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

	void SetImage2D(uint32_t _binding, CGfxTexture2DPtr _ptrImage)
	{
		bDirty = true;
		binding = _binding;
		pSampler = nullptr;
		ptrImage2D = _ptrImage;
		ptrImage2DArray.Release();
		ptrImageCubemap.Release();
		ptrImageRenderTexture.Release();
		ptrTexture2D.Release();
		ptrTexture2DArray.Release();
		ptrTextureCubemap.Release();
		ptrRenderTexture.Release();
		ptrInputAttachmentTexture.Release();
	}

	void SetImage2DArray(uint32_t _binding, CGfxTexture2DArrayPtr _ptrImage)
	{
		bDirty = true;
		binding = _binding;
		pSampler = nullptr;
		ptrImage2D.Release();
		ptrImage2DArray = _ptrImage;
		ptrImageCubemap.Release();
		ptrImageRenderTexture.Release();
		ptrTexture2D.Release();
		ptrTexture2DArray.Release();
		ptrTextureCubemap.Release();
		ptrRenderTexture.Release();
		ptrInputAttachmentTexture.Release();
	}

	void SetImageCubemap(uint32_t _binding, CGfxTextureCubemapPtr _ptrImage)
	{
		bDirty = true;
		binding = _binding;
		pSampler = nullptr;
		ptrImage2D.Release();
		ptrImage2DArray.Release();
		ptrImageCubemap = _ptrImage;
		ptrImageRenderTexture.Release();
		ptrTexture2D.Release();
		ptrTexture2DArray.Release();
		ptrTextureCubemap.Release();
		ptrRenderTexture.Release();
		ptrInputAttachmentTexture.Release();
	}

	void SetImageRenderTexture(uint32_t _binding, CGfxRenderTexturePtr _ptrImage)
	{
		bDirty = true;
		binding = _binding;
		pSampler = nullptr;
		ptrImage2D.Release();
		ptrImage2DArray.Release();
		ptrImageCubemap.Release();
		ptrImageRenderTexture = _ptrImage;
		ptrTexture2D.Release();
		ptrTexture2DArray.Release();
		ptrTextureCubemap.Release();
		ptrRenderTexture.Release();
		ptrInputAttachmentTexture.Release();
	}

	void SetTexture2D(uint32_t _binding, CGfxTexture2DPtr _ptrTexture, CGfxSampler* _pSampler)
	{
		bDirty = true;
		binding = _binding;
		pSampler = _pSampler;
		ptrImage2D.Release();
		ptrImage2DArray.Release();
		ptrImageCubemap.Release();
		ptrImageRenderTexture.Release();
		ptrTexture2D = _ptrTexture;
		ptrTexture2DArray.Release();
		ptrTextureCubemap.Release();
		ptrRenderTexture.Release();
		ptrInputAttachmentTexture.Release();
	}

	void SetTexture2DArray(uint32_t _binding, CGfxTexture2DArrayPtr _ptrTexture, CGfxSampler* _pSampler)
	{
		bDirty = true;
		binding = _binding;
		pSampler = _pSampler;
		ptrImage2D.Release();
		ptrImage2DArray.Release();
		ptrImageCubemap.Release();
		ptrImageRenderTexture.Release();
		ptrTexture2D.Release();
		ptrTexture2DArray = _ptrTexture;
		ptrTextureCubemap.Release();
		ptrRenderTexture.Release();
		ptrInputAttachmentTexture.Release();
	}

	void SetTextureCubemap(uint32_t _binding, CGfxTextureCubemapPtr _ptrTexture, CGfxSampler* _pSampler)
	{
		bDirty = true;
		binding = _binding;
		pSampler = _pSampler;
		ptrImage2D.Release();
		ptrImage2DArray.Release();
		ptrImageCubemap.Release();
		ptrImageRenderTexture.Release();
		ptrTexture2D.Release();
		ptrTexture2DArray.Release();
		ptrTextureCubemap = _ptrTexture;
		ptrRenderTexture.Release();
		ptrInputAttachmentTexture.Release();
	}

	void SetRenderTexture(uint32_t _binding, CGfxRenderTexturePtr _ptrTexture, CGfxSampler* _pSampler)
	{
		bDirty = true;
		binding = _binding;
		pSampler = _pSampler;
		ptrImage2D.Release();
		ptrImage2DArray.Release();
		ptrImageCubemap.Release();
		ptrImageRenderTexture.Release();
		ptrTexture2D.Release();
		ptrTexture2DArray.Release();
		ptrTextureCubemap.Release();
		ptrRenderTexture = _ptrTexture;
		ptrInputAttachmentTexture.Release();
	}

	void SetInputAttachmentTexture(uint32_t _binding, CGfxRenderTexturePtr _ptrTexture, CGfxSampler* _pSampler)
	{
		bDirty = true;
		binding = _binding;
		pSampler = _pSampler;
		ptrImage2D.Release();
		ptrImage2DArray.Release();
		ptrImageCubemap.Release();
		ptrImageRenderTexture.Release();
		ptrTexture2D.Release();
		ptrTexture2DArray.Release();
		ptrTextureCubemap.Release();
		ptrRenderTexture.Release();
		ptrInputAttachmentTexture = _ptrTexture;
	}


	bool bDirty;
	uint32_t binding;
	CGfxSampler* pSampler;
	CGfxTexture2DPtr ptrImage2D;
	CGfxTexture2DArrayPtr ptrImage2DArray;
	CGfxTextureCubemapPtr ptrImageCubemap;
	CGfxRenderTexturePtr ptrImageRenderTexture;
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
	virtual bool SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrBuffer, uint32_t offset, uint32_t range) = 0;
	virtual bool SetStorageBuffer(uint32_t name, const CGfxStorageBufferPtr ptrBuffer, uint32_t offset, uint32_t range) = 0;
	virtual bool SetImage2D(uint32_t name, const CGfxTexture2DPtr ptrImage) = 0;
	virtual bool SetImage2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrImage) = 0;
	virtual bool SetImageCubemap(uint32_t name, const CGfxTextureCubemapPtr ptrImage) = 0;
	virtual bool SetImageRenderTexture(uint32_t name, const CGfxRenderTexturePtr ptrImage) = 0;
	virtual bool SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture, const CGfxSampler* pSampler) = 0;
	virtual bool SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture, const CGfxSampler* pSampler) = 0;
	virtual bool SetTextureCubemap(uint32_t name, const CGfxTextureCubemapPtr ptrTexture, const CGfxSampler* pSampler) = 0;
	virtual bool SetRenderTexture(uint32_t name, const CGfxRenderTexturePtr ptrTexture, const CGfxSampler* pSampler) = 0;
	virtual bool SetInputAttachmentTexture(uint32_t name, const CGfxRenderTexturePtr ptrTexture, const CGfxSampler* pSampler) = 0;

public:
	virtual const CGfxDescriptorLayoutPtr GetDescriptorLayout(void) const = 0;
	virtual const DescriptorImageInfo* GetDescriptorImageInfo(uint32_t name) const = 0;
	virtual const DescriptorBufferInfo* GetDescriptorBufferInfo(uint32_t name) const = 0;


private:
	uint32_t m_name;
};
