#pragma once
#include "GfxRenderer.h"


typedef struct AttachmentInformation {
	bool bInvalidation = false;
	bool bClear = true;
	int stencil = 0;
	float depth = 1.0f;
	float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
} AttachmentInformation;

typedef struct SubPassInformation {
	int depthStencilAttachment = -1;
	eastl::unordered_map<int, int> inputAttachments;
	eastl::unordered_map<int, int> outputAttachments;
	eastl::unordered_map<int, int> resolveAttachments;
} SubPassInformation;

class CGfxRenderPass : public CGfxResource
{
	friend class CGfxRenderer;
	friend class CGfxRenderPassManager;


private:
	CGfxRenderPass(int numAttachments, int numSubpasses);
	virtual ~CGfxRenderPass(void);
	virtual void Release(void);


public:
	bool SetColorAttachment(int indexAttachment, bool bInvalidation, bool bClear, float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 0.0f);
	bool SetDepthStencilAttachment(int indexAttachment, bool bInvalidation, bool bClear, float depth = 1.0f, int stencil = 0);

	bool SetSubpassInputColorReference(int indexSubPass, int indexAttachment);
	bool SetSubpassOutputColorReference(int indexSubPass, int indexAttachment);
	bool SetSubpassOutputDepthStencilReference(int indexSubPass, int indexAttachment);
	bool SetSubpassResolveColorReference(int indexSubPass, int indexAttachment);

public:
	uint32_t GetAttachmentCount(void) const;
	const AttachmentInformation* GetAttachments(void) const;
	const AttachmentInformation* GetAttachment(int indexAttachment) const;

	uint32_t GetSubPassCount(void) const;
	uint32_t GetSubpassInputAttachmentCount(int indexSubPass) const;
	uint32_t GetSubpassOutputAttachmentCount(int indexSubPass) const;
	const SubPassInformation* GetSubPass(int indexSubPass) const;


private:
	eastl::vector<AttachmentInformation> m_attachments;
	eastl::vector<SubPassInformation> m_subpasses;
};
