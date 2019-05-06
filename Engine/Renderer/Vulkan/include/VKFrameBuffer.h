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
	VkFramebuffer GetFrameBuffer(void);

public:
	int GetWidth(void) const;
	int GetHeight(void) const;

public:
	bool Create(const CGfxRenderPassPtr ptrRenderPass);
	void Destroy(void);

public:
	bool SetAttachmentTexture(int indexAttachment, const CGfxRenderTexturePtr ptrAttachmentTexture);
	const CGfxRenderTexturePtr GetAttachmentTexture(int indexAttachment) const;


private:
	int m_width;
	int m_height;

private:
	VkFramebuffer m_vkFrameBuffer;
	eastl::vector<CGfxRenderTexturePtr> m_ptrAttachmentTextures;

private:
	CVKDevice* m_pDevice;

private:
	CVKFrameBufferManager* m_pManager;
};
