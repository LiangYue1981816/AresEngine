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
	bool Create(const CGfxDescriptorLayoutPtr ptrDescriptorLayout);
	void Destroy(void);

private:
	CVKDescriptorPool* GetDescriptorPool(void) const;

public:
	bool SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture, const CGfxSampler* pSampler);
	bool SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture, const CGfxSampler* pSampler);
	bool SetTextureCubemap(uint32_t name, const CGfxTextureCubemapPtr ptrTexture, const CGfxSampler* pSampler);
	bool SetRenderTexture(uint32_t name, const CGfxRenderTexturePtr ptrTexture, const CGfxSampler* pSampler);
	bool SetInputAttachmentTexture(uint32_t name, const CGfxRenderTexturePtr ptrTexture, const CGfxSampler* pSampler);
	bool SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrUniformBuffer, uint32_t offset, uint32_t range);
	bool SetStorageBuffer(uint32_t name, const CGfxStorageBufferPtr ptrStorageBuffer, uint32_t offset, uint32_t range);

public:
	const CGfxDescriptorLayoutPtr GetDescriptorLayout(void) const;
	const DescriptorImageInfo* GetDescriptorImageInfo(uint32_t name) const;
	const DescriptorBufferInfo* GetDescriptorBufferInfo(uint32_t name) const;

public:
	void Bind(VkCommandBuffer vkCommandBuffer, VkPipelineBindPoint vkPipelineBindPoint, VkPipelineLayout vkPipelineLayout);


private:
	VkDescriptorSet m_vkDescriptorSet;

private:
	CGfxDescriptorLayoutPtr m_ptrDescriptorLayout;
	eastl::unordered_map<uint32_t, DescriptorImageInfo> m_imageDescriptorInfos;
	eastl::unordered_map<uint32_t, DescriptorBufferInfo> m_bufferDescriptorInfos;

private:
	CVKDevice* m_pDevice;

private:
	CVKDescriptorPool* m_pDescriptorPool;
};
