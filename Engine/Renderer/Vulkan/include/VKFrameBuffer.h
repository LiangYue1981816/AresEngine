#pragma once
#include "VKRenderer.h"


class CVKFrameBuffer : public CGfxFrameBuffer
{
	friend class CVKFrameBufferManager;


private:
	CVKFrameBuffer(CVKDevice *pDevice, CVKFrameBufferManager *pManager, int width, int height, int numAttachments);
	virtual ~CVKFrameBuffer(void);
	virtual void Release(void);


public:
	uint64_t GetFrameBuffer(void);

public:
	int GetWidth(void) const;
	int GetHeight(void) const;

public:
	bool Create(uint64_t hRenderPass);
	void Destroy(void);

public:
	bool SetAttachmentTexture(int indexAttachment, CGfxRenderTexturePtr &ptrAttachmentTexture);
	CGfxRenderTexturePtr GetAttachmentTexture(int indexAttachment) const;


private:
	int m_width;
	int m_height;

private:
	VkFramebuffer m_vkFrameBuffer;
	eastl::vector<CGfxRenderTexturePtr> m_ptrAttachmentTextures;

private:
	CVKFrameBufferManager *m_pManager;
};
