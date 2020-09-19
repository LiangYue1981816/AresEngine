#pragma once
#include "VKRenderer.h"


class CVKRenderPass : public CGfxRenderPass
{
public:
	CVKRenderPass(CVKDevice* pDevice, CVKRenderPassManager* pManager, uint32_t name, int numAttachments, int numSubpasses);
	virtual ~CVKRenderPass(void);
	virtual void Release(void);


public:
	VkRenderPass GetRenderPass(void) const;

public:
	bool Create(void);
	void Destroy(bool bClear = true);

public:
	bool SetColorAttachment(int indexAttachment, GfxPixelFormat format, int samples, bool bInvalidation, bool bClear, float clearRed = 0.0f, float clearGreen = 0.0f, float clearBlue = 0.0f, float clearAlpha = 0.0f);
	bool SetDepthStencilAttachment(int indexAttachment, GfxPixelFormat format, int samples, bool bInvalidation, bool bClear, float clearDepth = 1.0f, int clearStencil = 0);

	bool SetSubpassInputReference(int indexSubpass, int indexAttachment);
	bool SetSubpassOutputReference(int indexSubpass, int indexAttachment);
	bool SetSubpassResolveReference(int indexSubpass, int indexAttachment);
	bool SetSubpassPreserveReference(int indexSubpass, int indexAttachment);
	bool SetSubpassDepthStencilReference(int indexSubpass, int indexAttachment);

public:
	uint32_t GetAttachmentCount(void) const;
	const AttachmentInformation* GetAttachments(void) const;
	const AttachmentInformation* GetAttachment(int indexAttachment) const;

	uint32_t GetSubpassCount(void) const;
	uint32_t GetSubpassInputAttachmentCount(int indexSubpass) const;
	uint32_t GetSubpassOutputAttachmentCount(int indexSubpass) const;
	const SubpassInformation* GetSubpass(int indexSubpass) const;


private:
	VkRenderPass m_vkRenderPass;

private:
	eastl::vector<AttachmentInformation> m_attachments;
	eastl::vector<SubpassInformation> m_subpasses;

private:
	CVKDevice* m_pDevice;

private:
	CVKRenderPassManager* m_pManager;
};
