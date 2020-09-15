#pragma once
#include "GfxCommandBuffer.h"


class CVKCommandSetImageLayout : public CGfxCommandBase
{
public:
	CVKCommandSetImageLayout(VkCommandBuffer vkCommandBuffer, const CGfxTexture2DPtr ptrTexture, GfxImageLayout imageLayout)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_imageLayout(imageLayout)
		, m_ptrTexture2D(ptrTexture)
	{
		Execute();
	}
	CVKCommandSetImageLayout(VkCommandBuffer vkCommandBuffer, const CGfxTexture2DArrayPtr ptrTexture, GfxImageLayout imageLayout)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_imageLayout(imageLayout)
		, m_ptrTexture2DArray(ptrTexture)
	{
		Execute();
	}
	CVKCommandSetImageLayout(VkCommandBuffer vkCommandBuffer, const CGfxTextureCubemapPtr ptrTexture, GfxImageLayout imageLayout)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_imageLayout(imageLayout)
		, m_ptrTextureCubemap(ptrTexture)
	{
		Execute();
	}
	CVKCommandSetImageLayout(VkCommandBuffer vkCommandBuffer, const CGfxRenderTexturePtr ptrTexture, GfxImageLayout imageLayout)
		: m_vkCommandBuffer(vkCommandBuffer)
		, m_imageLayout(imageLayout)
		, m_ptrRenderTexture(ptrTexture)
	{
		Execute();
	}
	virtual ~CVKCommandSetImageLayout(void)
	{

	}

public:
	virtual void Execute(void)
	{
		ASSERT(m_vkCommandBuffer);

		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_SET_IMAGELAYOUT);
		{
			/*
			if (m_ptrTexture2D) {
				((CVKTexture2D*)m_ptrTexture2D.GetPointer())->PipelineBarrier(m_vkCommandBuffer, CVKHelper::TranslateImageLayout(m_imageLayout));
			}

			if (m_ptrTexture2DArray) {
				((CVKTexture2DArray*)m_ptrTexture2DArray.GetPointer())->PipelineBarrier(m_vkCommandBuffer, CVKHelper::TranslateImageLayout(m_imageLayout));
			}

			if (m_ptrTextureCubemap) {
				((CVKTextureCubemap*)m_ptrTextureCubemap.GetPointer())->PipelineBarrier(m_vkCommandBuffer, CVKHelper::TranslateImageLayout(m_imageLayout));
			}

			if (m_ptrRenderTexture) {
				((CVKRenderTexture*)m_ptrRenderTexture.GetPointer())->PipelineBarrier(m_vkCommandBuffer, CVKHelper::TranslateImageLayout(m_imageLayout));
			}
			*/
		}
	}


private:
	GfxImageLayout m_imageLayout;
	CGfxTexture2DPtr m_ptrTexture2D;
	CGfxTexture2DArrayPtr m_ptrTexture2DArray;
	CGfxTextureCubemapPtr m_ptrTextureCubemap;
	CGfxRenderTexturePtr m_ptrRenderTexture;

private:
	VkCommandBuffer m_vkCommandBuffer;
};
