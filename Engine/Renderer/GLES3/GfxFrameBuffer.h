#pragma once
#include "GfxRenderer.h"


class CGfxFrameBuffer : public CGfxResource
{
	friend class CGfxRenderer;
	friend class CGfxFrameBufferManager;


private:
	CGfxFrameBuffer(int width, int height);
	virtual ~CGfxFrameBuffer(void);
	virtual void Release(void);


public:
	bool SetAttachmentTexture(int indexAttachment, CGfxTexture2DPtr &ptrAttachmentTexture);

public:
	int GetWidth(void) const;
	int GetHeight(void) const;

	CGfxTexture2DPtr GetAttachmentTexture(int indexAttachment) const;

public:
	void Bind(const AttachmentInformation *pAttachmentInformations, const SubPassInformation *pSubPassInformation) const;
	void Resolve(const AttachmentInformation *pAttachmentInformations, const SubPassInformation *pSubPassInformation) const;
	void InvalidateFramebuffer(const AttachmentInformation *pAttachmentInformations, const SubPassInformation *pSubPassInformation) const;


private:
	uint32_t m_fbo;
	uint32_t m_resolve;

private:
	int m_width;
	int m_height;

	eastl::unordered_map<int, CGfxTexture2DPtr> m_ptrAttachmentTextures;
};
