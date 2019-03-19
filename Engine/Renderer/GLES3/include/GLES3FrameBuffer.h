#pragma once
#include "GLES3Renderer.h"


class CGLES3FrameBuffer : public CGfxFrameBuffer
{
	friend class CGLES3FrameBufferManager;


private:
	CGLES3FrameBuffer(CGLES3FrameBufferManager *pManager, int width, int height, int numAttachments);
	virtual ~CGLES3FrameBuffer(void);
	virtual void Release(void);


public:
	HANDLE GetFrameBuffer(void);

	int GetWidth(void) const;
	int GetHeight(void) const;

public:
	bool Create(const CGfxRenderPassPtr ptrRenderPass);
	void Destroy(void);

public:
	bool SetAttachmentTexture(int indexAttachment, CGfxRenderTexturePtr ptrAttachmentTexture);
	CGfxRenderTexturePtr GetAttachmentTexture(int indexAttachment) const;

public:
	void Bind(const AttachmentInformation *pAttachmentInformations, const SubpassInformation *pSubpassInformation) const;
	void Resolve(const AttachmentInformation *pAttachmentInformations, const SubpassInformation *pSubpassInformation) const;
	void InvalidateFramebuffer(const AttachmentInformation *pAttachmentInformations, const SubpassInformation *pSubpassInformation) const;


private:
	int m_width;
	int m_height;

	eastl::vector<CGfxRenderTexturePtr> m_ptrAttachmentTextures;

private:
	uint32_t m_fbo;
	uint32_t m_resolve;

private:
	CGLES3FrameBufferManager *m_pManager;
};
