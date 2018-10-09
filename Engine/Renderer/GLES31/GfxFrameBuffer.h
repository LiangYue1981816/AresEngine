#pragma once
#include "GfxRenderer.h"


class CGfxFrameBuffer : public CGfxResource
{
	friend class CGfxRenderer;
	friend class CGfxFrameBufferManager;


private:
	typedef struct Attachment {
		CGfxTexture2DPtr ptrTexture;
		bool bInvalidation;
		bool bClear;
		int stencil;
		float depth;
		float color[4];
	} Attachment;


private:
	CGfxFrameBuffer(uint32_t width, uint32_t height);
	CGfxFrameBuffer(uint32_t width, uint32_t height, bool bDepthStencilRenderBuffer, int samples = 0);
	virtual ~CGfxFrameBuffer(void);
	virtual void Release(void);


public:
	bool SetDepthStencilTexture(CGfxTexture2DPtr &ptrTexture, bool bInvalidation, bool bClear, float depth = 1.0f, int stencil = 0);
	bool SetColorTexture(uint32_t index, CGfxTexture2DPtr &ptrTexture, bool bInvalidation, bool bClear, float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 0.0f);
	bool Apply(void);

public:
	uint32_t GetFBO(void) const;

	uint32_t GetWidth(void) const;
	uint32_t GetHeight(void) const;

	CGfxTexture2DPtr GetDepthStencilTexture(void) const;
	CGfxTexture2DPtr GetColorTexture(uint32_t index) const;

public:
	void Bind(void) const;
	void InvalidateFramebuffer(void) const;


private:
	uint32_t m_fbo;
	uint32_t m_rbo;

private:
	uint32_t m_width;
	uint32_t m_height;

	Attachment m_attachmentDepthStencil;
	eastl::unordered_map<uint32_t, Attachment> m_attachmentColors;
};
