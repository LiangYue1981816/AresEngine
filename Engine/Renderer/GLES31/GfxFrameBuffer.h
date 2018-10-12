#pragma once
#include "GfxRenderer.h"


class CGfxFrameBuffer : public CGfxResource
{
	friend class CGfxRenderer;
	friend class CGfxFrameBufferManager;


private:
	CGfxFrameBuffer(uint32_t width, uint32_t height);
	virtual ~CGfxFrameBuffer(void);
	virtual void Release(void);


public:
	bool SetAttachmentTexture(uint32_t indexAttachment, CGfxTexture2DPtr &ptrAttachmentTexture);

public:
	uint32_t GetFBO(void) const;
	uint32_t GetWidth(void) const;
	uint32_t GetHeight(void) const;
	CGfxTexture2DPtr GetAttachmentTexture(uint32_t indexAttachment) const;

public:
	void Bind(const AttachmentInformation *pAttachmentInformations, const SubPassInformation *pSubPassInformation) const;
	void InvalidateFramebuffer(const AttachmentInformation *pAttachmentInformations, const SubPassInformation *pSubPassInformation) const;


private:
	uint32_t m_fbo;

private:
	uint32_t m_width;
	uint32_t m_height;

	eastl::unordered_map<uint32_t, CGfxTexture2DPtr> m_ptrAttachmentTextures;
};
