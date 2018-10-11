#pragma once
#include "GfxRenderer.h"


typedef struct AttachmentInformation {
	bool bInvalidation;
	bool bClear;
	int stencil;
	float depth;
	float color[4];
} AttachmentInformation;

typedef struct SubPassInformation {
	eastl::unordered_map<uint32_t, uint32_t> inputAttachments;
	eastl::unordered_map<uint32_t, uint32_t> colorAttachments;
	eastl::unordered_map<uint32_t, uint32_t> resolveAttachments;
	uint32_t depthStencilAttachment;
} SubPassInformation;

class CGfxRenderPass : public CGfxResource
{
	friend class CGfxRenderer;


private:
	CGfxRenderPass(uint32_t numAttachments, uint32_t numSubpasses);
	virtual ~CGfxRenderPass(void);
	virtual void Release(void);


public:
	bool SetPresentAttachment(uint32_t indexAttachment, bool bInvalidation, bool bClear, float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 0.0f);
	bool SetColorAttachment(uint32_t indexAttachment, bool bInvalidation, bool bClear, float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 0.0f);
	bool SetDepthStencilAttachment(uint32_t indexAttachment, bool bInvalidation, bool bClear, float depth = 1.0f, int stencil = 0);

	bool SetSubpassInputColorReference(uint32_t indexSubPass, uint32_t indexAttachment);
	bool SetSubpassInputDepthStencilReference(uint32_t indexSubPass, uint32_t indexAttachment);
	bool SetSubpassOutputColorReference(uint32_t indexSubPass, uint32_t indexAttachment);
	bool SetSubpassOutputDepthStencilReference(uint32_t indexSubPass, uint32_t indexAttachment);
	bool SetSubpassResolveColorReference(uint32_t indexSubPass, uint32_t indexAttachment);

public:
	uint32_t GetSubPassCount(void) const;
	uint32_t GetSubpassInputAttachmentCount(uint32_t indexSubPass) const;
	uint32_t GetSubpassOutputAttachmentCount(uint32_t indexSubPass) const;
	const SubPassInformation* GetSubPass(int indexSubPass) const;

	uint32_t GetAttachmentCount(void) const;
	const AttachmentInformation* GetAttachment(int indexAttachment) const;


private:
	eastl::vector<AttachmentInformation> m_attachments;
	eastl::vector<SubPassInformation> m_subpasses;
};
