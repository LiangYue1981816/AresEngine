#pragma once
#include "VKRenderer.h"


class CVKFrameBuffer : public CGfxFrameBuffer
{
public:
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
	void Destroy(bool bClear = true);

public:
	bool SetAttachmentTexture(int indexAttachment, const CGfxRenderTexturePtr ptrAttachmentTexture);
	const CGfxRenderTexturePtr GetAttachmentTexture(int indexAttachment) const;


private:
	VkFramebuffer m_vkFrameBuffer;

private:
	int m_width;
	int m_height;
	eastl::vector<CGfxRenderTexturePtr> m_ptrAttachmentTextures;

private:
	CVKDevice* m_pDevice;

private:
	CVKFrameBufferManager* m_pManager;
};
