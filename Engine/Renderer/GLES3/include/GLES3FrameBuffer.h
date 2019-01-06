#pragma once
#include "GLES3Renderer.h"


class CGLES3FrameBuffer : public CGfxFrameBuffer
{
	friend class CGLES3FrameBufferManager;


private:
	CGLES3FrameBuffer(CGLES3FrameBufferManager *pManager, int width, int height);
	virtual ~CGLES3FrameBuffer(void);
	virtual void Release(void);


public:
	int GetWidth(void) const;
	int GetHeight(void) const;

public:
	bool SetAttachmentTexture(int indexAttachment, CGfxRenderTexturePtr &ptrAttachmentTexture);
	CGfxRenderTexturePtr GetAttachmentTexture(int indexAttachment) const;

public:
	void Bind(const AttachmentInformation *pAttachmentInformations, const SubPassInformation *pSubPassInformation);
	void Resolve(const AttachmentInformation *pAttachmentInformations, const SubPassInformation *pSubPassInformation);
	void InvalidateFramebuffer(const AttachmentInformation *pAttachmentInformations, const SubPassInformation *pSubPassInformation);


private:
	int m_width;
	int m_height;

private:
	uint32_t m_fbo;
	uint32_t m_resolve;
	eastl::unordered_map<int, CGfxRenderTexturePtr> m_ptrAttachmentTextures;

private:
	CGLES3FrameBufferManager *m_pManager;
};
