#pragma once
#include "VKRenderer.h"


class CVKDescriptorSet
{
	friend class CVKDescriptorPool;


private:
	typedef struct DescriptorImageInfo {
		VkDescriptorImageInfo imageInfo;
		CVKSampler *pSampler;
		CGfxTexture2DPtr ptrTexture2D;
		CGfxTexture2DArrayPtr ptrTexture2DArray;
		CGfxTextureCubeMapPtr ptrTextureCubeMap;
		CGfxRenderTexturePtr ptrRenderTexture;
	} DescriptorImageInfo;

	typedef struct DescriptorBufferInfo {
		VkDescriptorBufferInfo bufferInfo;
		CGfxUniformBufferPtr ptrUniformBuffer;
	} DescriptorBufferInfo;


private:
	CVKDescriptorSet(CVKDevice *pDevice, CVKDescriptorPool *pDescriptorPool, CVKDescriptorLayout *pDescriptorLayout);
	virtual ~CVKDescriptorSet(void);


public:
	uint32_t GetSetIndex(void) const;
	VkDescriptorSet GetDescriptorSet(void) const;
	CVKDescriptorPool* GetDescriptorPool(void) const;
	CVKDescriptorLayout* GetDescriptorLayout(void) const;

public:
	bool SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture, const CGfxSampler *pSampler);
	bool SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture, const CGfxSampler *pSampler);
	bool SetTextureCubeMap(uint32_t name, const CGfxTextureCubeMapPtr ptrTexture, const CGfxSampler *pSampler);
	bool SetRenderTexture(uint32_t name, const CGfxRenderTexturePtr ptrRenderTexture, const CGfxSampler *pSampler);
	bool SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrUniformBuffer);

public:
	CGfxSampler* GetSampler(uint32_t name) const;
	CGfxTexture2DPtr GetTexture2D(uint32_t name) const;
	CGfxTexture2DArrayPtr GetTexture2DArray(uint32_t name) const;
	CGfxTextureCubeMapPtr GetTextureCubeMap(uint32_t name) const;
	CGfxRenderTexturePtr GetRenderTexture(uint32_t name) const;

public:
	void Update(void);
	void Bind(VkCommandBuffer vkCommandBuffer, VkPipelineBindPoint vkPipelineBindPoint, VkPipelineLayout vkPipelineLayout) const;


private:
	bool m_bDirty;
	eastl::unordered_map<uint32_t, DescriptorImageInfo> m_imageDescriptors;
	eastl::unordered_map<uint32_t, DescriptorBufferInfo> m_bufferDescriptors;

private:
	VkDescriptorSet m_vkDescriptorSet;
	CVKDescriptorPool *m_pDescriptorPool;
	CVKDescriptorLayout *m_pDescriptorLayout;

private:
	CVKDevice *m_pDevice;
};
