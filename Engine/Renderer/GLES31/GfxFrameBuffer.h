#pragma once
#include "GfxRenderer.h"


class CGfxFrameBuffer : public CGfxResource
{
	friend class CGfxRenderer;
	friend class CGfxFrameBufferManager;


private:
	CGfxFrameBuffer(uint32_t width, uint32_t height, bool bDepthStencilRenderBuffer);
	virtual ~CGfxFrameBuffer(void);
	virtual void Release(void);


public:
	bool SetDepthStencilTexture(CGfxTexture2DPtr &ptrTexture);
	bool SetColorTexture(uint32_t index, CGfxTexture2DPtr &ptrTexture, bool invalidation);
	bool Apply(void);

public:
	uint32_t GetWidth(void) const;
	uint32_t GetHeight(void) const;

	CGfxTexture2DPtr GetDepthTexture(void) const;
	CGfxTexture2DPtr GetColorTexture(uint32_t index) const;

public:
	void Bind(void) const;
	void InvalidateFramebuffer(void) const;


private:
	uint32_t m_width;
	uint32_t m_height;

private:
	uint32_t m_fbo;
	uint32_t m_rbo;

	CGfxTexture2DPtr m_ptrDepthTexture;
	eastl::unordered_map<uint32_t, CGfxTexture2DPtr> m_ptrColorTextures;
	eastl::unordered_map<uint32_t, bool> m_invalidations;
};
