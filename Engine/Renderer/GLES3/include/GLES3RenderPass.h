#pragma once
#include "GLES3Renderer.h"


class CGLES3RenderPass : public CGfxRenderPass
{
	friend class CGLES3RenderPassManager;


private:
	CGLES3RenderPass(CGLES3RenderPassManager *pManager, uint32_t name, int numAttachments, int numSubpasses);
	virtual ~CGLES3RenderPass(void);
	virtual void Release(void);


public:
	uint32_t GetName(void) const;

public:
	HANDLE GetRenderPass(void) const;

public:
	bool SetColorAttachment(int indexAttachment, GfxPixelFormat pixelFormat, int samples, bool bInvalidation, bool bClear, float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 0.0f);
	bool SetDepthStencilAttachment(int indexAttachment, GfxPixelFormat pixelFormat, int samples, bool bInvalidation, bool bClear, float depth = 1.0f, int stencil = 0);

	bool SetSubpassInputColorReference(int indexSubPass, int indexAttachment, const char *szName);
	bool SetSubpassOutputColorReference(int indexSubPass, int indexAttachment);
	bool SetSubpassOutputDepthStencilReference(int indexSubPass, int indexAttachment);
	bool SetSubpassResolveReference(int indexSubPass, int indexAttachment);
	bool SetSubpassPreserveReference(int indexSubPass, int indexAttachment);

public:
	uint32_t GetAttachmentCount(void) const;
	const AttachmentInformation* GetAttachments(void) const;
	const AttachmentInformation* GetAttachment(int indexAttachment) const;

	uint32_t GetSubPassCount(void) const;
	uint32_t GetSubpassInputAttachmentCount(int indexSubPass) const;
	uint32_t GetSubpassOutputAttachmentCount(int indexSubPass) const;
	const SubPassInformation* GetSubPass(int indexSubPass) const;

public:
	bool Create(void);
	void Destroy(void);


private:
	uint32_t m_name;

private:
	eastl::vector<AttachmentInformation> m_attachments;
	eastl::vector<SubPassInformation> m_subpasses;

private:
	CGLES3RenderPassManager *m_pManager;
};
