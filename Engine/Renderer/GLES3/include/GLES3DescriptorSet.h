#pragma once
#include "GLES3Renderer.h"


class CGLES3DescriptorSet : public CGfxDescriptorSet
{
	friend class CGLES3DescriptorSetManager;


private:
	CGLES3DescriptorSet(CGLES3DescriptorSetManager* pManager, uint32_t name, const CGfxDescriptorLayoutPtr ptrDescriptorLayout);
	CGLES3DescriptorSet(CGLES3DescriptorSetManager* pManager, uint32_t name, const CGfxDescriptorSetPtr ptrDescriptorSetCopyFrom);
	virtual ~CGLES3DescriptorSet(void);
	virtual void Release(void);


public:
	bool SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrUniformBuffer, uint32_t offset, uint32_t range);
	bool SetStorageBuffer(uint32_t name, const CGfxStorageBufferPtr ptrStorageBuffer, uint32_t offset, uint32_t range);
	bool SetImage2D(uint32_t name, const CGfxTexture2DPtr ptrImage);
	bool SetImage2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrImage);
	bool SetImageCubemap(uint32_t name, const CGfxTextureCubemapPtr ptrImage);
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
	CGfxDescriptorLayoutPtr m_ptrDescriptorLayout;

private:
	eastl::unordered_map<uint32_t, DescriptorImageInfo> m_imageDescriptorInfos;
	eastl::unordered_map<uint32_t, DescriptorBufferInfo> m_bufferDescriptorInfos;

private:
	CGLES3DescriptorSetManager* m_pManager;
};
