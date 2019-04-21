#pragma once
#include "VKRenderer.h"


class CVKFrameBuffer : public CGfxFrameBuffer
{
	friend class CVKFrameBufferManager;


private:
	CVKFrameBuffer(CVKDevice* pDevice, CVKFrameBufferManager* pManager, int width, int height, int numAttachments);
	virtual ~CVKFrameBuffer(void);
	virtual void Release(void);


public:
	HANDLE GetFrameBuffer(void);

public:
	int GetWidth(void) const;
	int GetHeight(void) const;

public:
	bool Create(const CGfxRenderPassPtr ptrRenderPass);
	void Destroy(void);

public:
	bool SetAttachmentTexture(int indexAttachment, CGfxRenderTexturePtr ptrAttachmentTexture);
	CGfxRenderTexturePtr GetAttachmentTexture(int indexAttachment) const;


private:
	int m_width;
	int m_height;

	eastl::vector<CGfxRenderTexturePtr> m_ptrAttachmentTextures;

private:
	VkFramebuffer m_vkFrameBuffer;

private:
	CVKDevice* m_pDevice;
	CVKFrameBufferManager* m_pManager;
};
