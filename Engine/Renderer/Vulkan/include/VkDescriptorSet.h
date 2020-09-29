#pragma once
#include "VKRenderer.h"


class CVKDescriptorSet : public CGfxDescriptorSet
{
	friend class CVKDescriptorPool;
	friend class CVKDescriptorSetManager;


private:
	CVKDescriptorSet(CVKDevice* pDevice, CVKDescriptorPool* pDescriptorPool, uint32_t name, const CGfxDescriptorLayoutPtr ptrDescriptorLayout);
	CVKDescriptorSet(CVKDevice* pDevice, CVKDescriptorPool* pDescriptorPool, uint32_t name, const CGfxDescriptorSetPtr ptrDescriptorSetCopyFrom);
	virtual ~CVKDescriptorSet(void);
	virtual void Release(void);


private:
	CVKDescriptorPool* GetDescriptorPool(void) const;

public:
	bool SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrBuffer, uint32_t offset, uint32_t range);
	bool SetStorageBuffer(uint32_t name, const CGfxStorageBufferPtr ptrBuffer, uint32_t offset, uint32_t range);
	bool SetImage2D(uint32_t name, const CGfxTexture2DPtr ptrImage, uint32_t level);
	bool SetImage2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrImage, uint32_t level, uint32_t layer);
	bool SetImageCubemap(uint32_t name, const CGfxTextureCubemapPtr ptrImage, uint32_t level, uint32_t layer);
	bool SetImageRenderTexture(uint32_t name, const CGfxRenderTexturePtr ptrImage);
	bool SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture, const CGfxSampler* pSampler);
	bool SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture, const CGfxSampler* pSampler);
	bool SetTextureCubemap(uint32_t name, const CGfxTextureCubemapPtr ptrTexture, const CGfxSampler* pSampler);
	bool SetRenderTexture(uint32_t name, const CGfxRenderTexturePtr ptrTexture, const CGfxSampler* pSampler);
	bool SetInputAttachmentTexture(uint32_t name, const CGfxRenderTexturePtr ptrTexture, const CGfxSampler* pSampler);

public:
	const CGfxDescriptorLayoutPtr GetDescriptorLayout(void) const;
	const DescriptorImageInfo* GetDescriptorImageInfo(uint32_t name) const;
	const DescriptorBufferInfo* GetDescriptorBufferInfo(uint32_t name) const;


private:
	eastl::unordered_map<uint32_t, DescriptorImageInfo> m_imageDescriptorInfos;
	eastl::unordered_map<uint32_t, DescriptorBufferInfo> m_bufferDescriptorInfos;

private:
	VkDescriptorSet m_vkDescriptorSet;
	CGfxDescriptorLayoutPtr m_ptrDescriptorLayout;

private:
	CVKDevice* m_pDevice;

private:
	CVKDescriptorPool* m_pDescriptorPool;
};
