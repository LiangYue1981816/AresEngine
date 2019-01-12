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
	HANDLE GetFrameBuffer(void);

	int GetWidth(void) const;
	int GetHeight(void) const;

public:
	bool SetAttachmentTexture(int indexAttachment, CGfxRenderTexturePtr &ptrAttachmentTexture);
	CGfxRenderTexturePtr GetAttachmentTexture(int indexAttachment) const;

public:
	bool Create(HANDLE hRenderPass);
	void Destroy(void);


private:
	int m_width;
	int m_height;

private:
	VkFramebuffer m_vkFrameBuffer;
	eastl::vector<CGfxRenderTexturePtr> m_ptrAttachmentTextures;

private:
	CVKDevice *m_pDevice;

private:
	CVKFrameBufferManager *m_pManager;
};
