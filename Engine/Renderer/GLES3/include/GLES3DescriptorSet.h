#pragma once
#include "GLES3Renderer.h"


class CALL_API CGLES3DescriptorSet : public CGfxDescriptorSet
{
	friend class CGLES3DescriptorSetManager;


private:
	CGLES3DescriptorSet(CGLES3DescriptorSetManager* pManager, const CGfxDescriptorLayoutPtr ptrDescriptorLayout);
	virtual ~CGLES3DescriptorSet(void);
	virtual void Release(void);


public:
	bool SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture, const CGfxSampler* pSampler);
	bool SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture, const CGfxSampler* pSampler);
	bool SetTextureCubeMap(uint32_t name, const CGfxTextureCubeMapPtr ptrTexture, const CGfxSampler* pSampler);
	bool SetTextureInputAttachment(uint32_t name, const CGfxRenderTexturePtr ptrTexture, const CGfxSampler* pSampler);
	bool SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrUniformBuffer, uint32_t offset, uint32_t range);

public:
	const CGfxDescriptorLayoutPtr GetDescriptorLayout(void) const;
	const DescriptorImageInfo* GetDescriptorImageInfo(uint32_t name) const;
	const DescriptorBufferInfo* GetDescriptorBufferInfo(uint32_t name) const;


private:
	CGfxDescriptorLayoutPtr m_ptrDescriptorLayout;

private:
	eastl::unordered_map<uint32_t, DescriptorImageInfo> m_imageDescriptorInfos;
	eastl::unordered_map<uint32_t, DescriptorBufferInfo> m_bufferDescriptorInfos;

private:
	CGLES3DescriptorSetManager* m_pManager;
};
