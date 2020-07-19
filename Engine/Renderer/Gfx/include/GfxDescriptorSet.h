#pragma once
#include "PreHeader.h"


typedef struct DescriptorBufferInfo {
	DescriptorBufferInfo(void)
	{
		bDirty = false;
		binding = INVALID_VALUE;
	}

	void SetUniformBuffer(uint32_t _binding, CGfxUniformBufferPtr _ptrBuffer, uint32_t _offset, uint32_t _range)
	{
		if (binding != _binding || ptrUniformBuffer != _ptrBuffer) {
			bDirty = true;
			binding = _binding;
			ptrUniformBuffer = _ptrBuffer;
		}

		offset = _offset;
		range = _range;
		
		ptrStorageBuffer.Release();
	}

	void SetStorageBuffer(uint32_t _binding, CGfxStorageBufferPtr _ptrBuffer, uint32_t _offset, uint32_t _range)
	{
		if (binding != _binding || ptrStorageBuffer != _ptrBuffer) {
			bDirty = true;
			binding = _binding;
			ptrStorageBuffer = _ptrBuffer;
		}

		offset = _offset;
		range = _range;

		ptrUniformBuffer.Release();
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
		binding = INVALID_VALUE;
	}

	void SetImage2D(uint32_t _binding, CGfxTexture2DPtr _ptrImage, uint32_t _level)
	{
		if (binding != _binding || ptrImage2D != _ptrImage || level != _level) {
			bDirty = true;
			binding = _binding;
			level = _level;
			ptrImage2D = _ptrImage;
		}

		pSampler = nullptr;
		ptrImage2DArray.Release();
		ptrImageCubemap.Release();
		ptrImageRenderTexture.Release();
		ptrTexture2D.Release();
		ptrTexture2DArray.Release();
		ptrTextureCubemap.Release();
		ptrRenderTexture.Release();
		ptrInputAttachmentTexture.Release();
	}

	void SetImage2DArray(uint32_t _binding, CGfxTexture2DArrayPtr _ptrImage, uint32_t _level, uint32_t _layer)
	{
		if (binding != _binding || ptrImage2DArray != _ptrImage || level != _level || layer != _layer) {
			bDirty = true;
			binding = _binding;
			level = _level;
			layer = _layer;
			ptrImage2DArray = _ptrImage;
		}

		pSampler = nullptr;
		ptrImage2D.Release();
		ptrImageCubemap.Release();
		ptrImageRenderTexture.Release();
		ptrTexture2D.Release();
		ptrTexture2DArray.Release();
		ptrTextureCubemap.Release();
		ptrRenderTexture.Release();
		ptrInputAttachmentTexture.Release();
	}

	void SetImageCubemap(uint32_t _binding, CGfxTextureCubemapPtr _ptrImage, uint32_t _level, uint32_t _layer)
	{
		if (binding != _binding || ptrImageCubemap != _ptrImage || level != _level || layer != _layer) {
			bDirty = true;
			binding = _binding;
			level = _level;
			layer = _layer;
			ptrImageCubemap = _ptrImage;
		}

		pSampler = nullptr;
		ptrImage2D.Release();
		ptrImage2DArray.Release();
		ptrImageRenderTexture.Release();
		ptrTexture2D.Release();
		ptrTexture2DArray.Release();
		ptrTextureCubemap.Release();
		ptrRenderTexture.Release();
		ptrInputAttachmentTexture.Release();
	}

	void SetImageRenderTexture(uint32_t _binding, CGfxRenderTexturePtr _ptrImage)
	{
		if (binding != _binding || ptrImageRenderTexture != _ptrImage) {
			bDirty = true;
			binding = _binding;
			ptrImageRenderTexture = _ptrImage;
		}

		pSampler = nullptr;
		ptrImage2D.Release();
		ptrImage2DArray.Release();
		ptrImageCubemap.Release();
		ptrTexture2D.Release();
		ptrTexture2DArray.Release();
		ptrTextureCubemap.Release();
		ptrRenderTexture.Release();
		ptrInputAttachmentTexture.Release();
	}

	void SetTexture2D(uint32_t _binding, CGfxTexture2DPtr _ptrTexture, CGfxSampler* _pSampler)
	{
		if (binding != _binding || ptrTexture2D != _ptrTexture || pSampler != _pSampler) {
			bDirty = true;
			binding = _binding;
			pSampler = _pSampler;
			ptrTexture2D = _ptrTexture;
		}

		ptrImage2D.Release();
		ptrImage2DArray.Release();
		ptrImageCubemap.Release();
		ptrImageRenderTexture.Release();
		ptrTexture2DArray.Release();
		ptrTextureCubemap.Release();
		ptrRenderTexture.Release();
		ptrInputAttachmentTexture.Release();
	}

	void SetTexture2DArray(uint32_t _binding, CGfxTexture2DArrayPtr _ptrTexture, CGfxSampler* _pSampler)
	{
		if (binding != _binding || ptrTexture2DArray != _ptrTexture || pSampler != _pSampler) {
			bDirty = true;
			binding = _binding;
			pSampler = _pSampler;
			ptrTexture2DArray = _ptrTexture;
		}

		ptrImage2D.Release();
		ptrImage2DArray.Release();
		ptrImageCubemap.Release();
		ptrImageRenderTexture.Release();
		ptrTexture2D.Release();
		ptrTextureCubemap.Release();
		ptrRenderTexture.Release();
		ptrInputAttachmentTexture.Release();
	}

	void SetTextureCubemap(uint32_t _binding, CGfxTextureCubemapPtr _ptrTexture, CGfxSampler* _pSampler)
	{
		if (binding != _binding || ptrTextureCubemap != _ptrTexture || pSampler != _pSampler) {
			bDirty = true;
			binding = _binding;
			pSampler = _pSampler;
			ptrTextureCubemap = _ptrTexture;
		}

		ptrImage2D.Release();
		ptrImage2DArray.Release();
		ptrImageCubemap.Release();
		ptrImageRenderTexture.Release();
		ptrTexture2D.Release();
		ptrTexture2DArray.Release();
		ptrRenderTexture.Release();
		ptrInputAttachmentTexture.Release();
	}

	void SetRenderTexture(uint32_t _binding, CGfxRenderTexturePtr _ptrTexture, CGfxSampler* _pSampler)
	{
		if (binding != _binding || ptrRenderTexture != _ptrTexture || pSampler != _pSampler) {
			bDirty = true;
			binding = _binding;
			pSampler = _pSampler;
			ptrRenderTexture = _ptrTexture;
		}

		ptrImage2D.Release();
		ptrImage2DArray.Release();
		ptrImageCubemap.Release();
		ptrImageRenderTexture.Release();
		ptrTexture2D.Release();
		ptrTexture2DArray.Release();
		ptrTextureCubemap.Release();
		ptrInputAttachmentTexture.Release();
	}

	void SetInputAttachmentTexture(uint32_t _binding, CGfxRenderTexturePtr _ptrTexture, CGfxSampler* _pSampler)
	{
		if (binding != _binding || ptrInputAttachmentTexture != _ptrTexture || pSampler != _pSampler) {
			bDirty = true;
			binding = _binding;
			pSampler = _pSampler;
			ptrInputAttachmentTexture = _ptrTexture;
		}

		ptrImage2D.Release();
		ptrImage2DArray.Release();
		ptrImageCubemap.Release();
		ptrImageRenderTexture.Release();
		ptrTexture2D.Release();
		ptrTexture2DArray.Release();
		ptrTextureCubemap.Release();
		ptrRenderTexture.Release();
	}

	bool bDirty;
	uint32_t binding;
	uint32_t level;
	uint32_t layer;
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
	virtual bool SetImage2D(uint32_t name, const CGfxTexture2DPtr ptrImage, uint32_t level) = 0;
	virtual bool SetImage2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrImage, uint32_t level, uint32_t layer) = 0;
	virtual bool SetImageCubemap(uint32_t name, const CGfxTextureCubemapPtr ptrImage, uint32_t level, uint32_t layer) = 0;
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
