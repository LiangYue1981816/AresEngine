#pragma once
#include "VKRenderer.h"


class CVKDescriptorSet : public CGfxDescriptorSet
{
	friend class CVKDescriptorPool;
	friend class CVKDescriptorSetManager;


private:
	CVKDescriptorSet(CVKDevice* pDevice, CVKDescriptorPool* pDescriptorPool, const CGfxDescriptorLayoutPtr ptrDescriptorLayout);
	virtual ~CVKDescriptorSet(void);
	virtual void Release(void);


public:
	bool SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture, const CGfxSampler* pSampler);
	bool SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture, const CGfxSampler* pSampler);
	bool SetTextureCubemap(uint32_t name, const CGfxTextureCubemapPtr ptrTexture, const CGfxSampler* pSampler);
	bool SetTextureInputAttachment(uint32_t name, const CGfxRenderTexturePtr ptrTexture, const CGfxSampler* pSampler);
	bool SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrUniformBuffer, uint32_t offset, uint32_t range);

public:
	const CVKDescriptorPool* GetDescriptorPool(void) const;
	const CGfxDescriptorLayoutPtr GetDescriptorLayout(void) const;
	const DescriptorImageInfo* GetDescriptorImageInfo(uint32_t name) const;
	const DescriptorBufferInfo* GetDescriptorBufferInfo(uint32_t name) const;


private:
	CVKDescriptorPool* m_pDescriptorPool;
	CGfxDescriptorLayoutPtr m_ptrDescriptorLayout;

private:
	CVKDevice* m_pDevice;
};
