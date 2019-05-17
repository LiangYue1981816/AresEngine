#pragma once
#include "VKRenderer.h"


class CVKRenderPass : public CGfxRenderPass
{
	friend class CVKRenderPassManager;


private:
	CVKRenderPass(CVKDevice* pDevice, CVKRenderPassManager* pManager, uint32_t name, int numAttachments, int numSubpasses);
	virtual ~CVKRenderPass(void);
	virtual void Release(void);


public:
	VkRenderPass GetRenderPass(void) const;

public:
	bool Create(void);
	void Destroy(bool bClear = true);

public:
	bool SetColorAttachment(int indexAttachment, GfxPixelFormat format, int samples, bool bInvalidation, bool bClear, float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 0.0f);
	bool SetDepthStencilAttachment(int indexAttachment, GfxPixelFormat format, int samples, bool bInvalidation, bool bClear, float depth = 1.0f, int stencil = 0);

	bool SetSubpassInputColorReference(int indexSubpass, int indexAttachment);
	bool SetSubpassOutputColorReference(int indexSubpass, int indexAttachment);
	bool SetSubpassOutputDepthStencilReference(int indexSubpass, int indexAttachment);
	bool SetSubpassResolveReference(int indexSubpass, int indexAttachment);
	bool SetSubpassPreserveReference(int indexSubpass, int indexAttachment);

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
