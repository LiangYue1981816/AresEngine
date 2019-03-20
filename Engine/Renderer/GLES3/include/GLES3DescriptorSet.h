#pragma once
#include "GLES3Renderer.h"


class CALL_API CGLES3DescriptorSet : public CGfxDescriptorSet
{
	friend class CGLES3DescriptorSetManager;


private:
	typedef struct DescriptorImageInfo {
		CGfxSampler *pSampler;
		CGfxTexture2DPtr ptrTexture2D;
		CGfxTexture2DArrayPtr ptrTexture2DArray;
		CGfxTextureCubeMapPtr ptrTextureCubeMap;
		CGfxRenderTexturePtr ptrRenderTexture;
	} DescriptorImageInfo;

	typedef struct DescriptorBufferInfo {
		CGfxUniformBufferPtr ptrUniformBuffer;
	} DescriptorBufferInfo;


private:
	CGLES3DescriptorSet(CGLES3DescriptorSetManager *pManager, const CGfxDescriptorLayoutPtr ptrDescriptorLayout);
	virtual ~CGLES3DescriptorSet(void);
	virtual void Release(void);


public:
	HANDLE GetDescriptorSet(void) const;
	CGfxDescriptorLayoutPtr GetDescriptorLayout(void) const;

public:
	bool SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrUniformBuffer);
	bool SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture, const CGfxSampler *pSampler);
	bool SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture, const CGfxSampler *pSampler);
	bool SetTextureCubeMap(uint32_t name, const CGfxTextureCubeMapPtr ptrTexture, const CGfxSampler *pSampler);
	bool SetRenderTexture(uint32_t name, const CGfxRenderTexturePtr ptrRenderTexture, const CGfxSampler *pSampler);

public:
	CGfxSampler* GetSampler(uint32_t name) const;
	CGfxTexture2DPtr GetTexture2D(uint32_t name) const;
	CGfxTexture2DArrayPtr GetTexture2DArray(uint32_t name) const;
	CGfxTextureCubeMapPtr GetTextureCubeMap(uint32_t name) const;
	CGfxRenderTexturePtr GetRenderTexture(uint32_t name) const;


private:
	eastl::unordered_map<uint32_t, DescriptorImageInfo> m_imageDescriptors;
	eastl::unordered_map<uint32_t, DescriptorBufferInfo> m_bufferDescriptors;

private:
	CGLES3DescriptorSetManager *m_pManager;
};
