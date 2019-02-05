#pragma once
#include "VKRenderer.h"


class CVKDescriptorSet
{
private:
	typedef struct DescriptorImageInfo {
		VkDescriptorImageInfo imageInfo;
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
	CVKDescriptorSet(CVKDevice *pDevice, CVKDescriptorLayout *pDescriptorLayout, VkDescriptorSet vkDescriptorSet);
	virtual ~CVKDescriptorSet(void);


public:
	VkDescriptorSet GetDescriptorSet(void) const;
	CVKDescriptorLayout* GetDescriptorLayout(void) const;

public:
	bool SetTexture2D(uint32_t name, const CGfxTexture2DPtr &ptrTexture, const CGfxSampler *pSampler);
	bool SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr &ptrTexture, const CGfxSampler *pSampler);
	bool SetTextureCubeMap(uint32_t name, const CGfxTextureCubeMapPtr &ptrTexture, const CGfxSampler *pSampler);
	bool SetRenderTexture(uint32_t name, const CGfxRenderTexturePtr &ptrRenderTexture, const CGfxSampler *pSampler);
	bool SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr &ptrUniformBuffer);
	void Update(void);


private:
	eastl::unordered_map<uint32_t, DescriptorImageInfo> m_imageDescriptors;
	eastl::unordered_map<uint32_t, DescriptorBufferInfo> m_bufferDescriptors;

private:
	VkDescriptorSet m_vkDescriptorSet;
	CVKDescriptorLayout *m_pDescriptorLayout;

private:
	CVKDevice *m_pDevice;
};
