#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindSubpassInputTexture : public CGfxCommandBase
{
public:
	CGLES3CommandBindSubpassInputTexture(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, int indexSubpass)
		: m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
		, m_indexSubpass(indexSubpass)
	{

	}
	virtual ~CGLES3CommandBindSubpassInputTexture(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_SUBPASSINPUTTEXTURE, "CommandBindSubpassInputTexture");
		{
			if (m_ptrFrameBuffer.IsValid() && m_ptrRenderPass.IsValid()) {
				if (const SubpassInformation *pSubpassInformation = m_ptrRenderPass->GetSubpass(m_indexSubpass)) {
					for (const auto &itInputAttachment : pSubpassInformation->inputAttachments) {
						const CGfxRenderTexturePtr ptrInputTexture = m_ptrFrameBuffer->GetAttachmentTexture(itInputAttachment.first);
						if (ptrInputTexture.IsValid()) {
							GLES3Renderer()->BindInputTexture(itInputAttachment.second.c_str(), (CGfxRenderTexture *)ptrInputTexture.GetPointer());
						}
					}
				}
			}
		}
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderPassPtr m_ptrRenderPass;
	int m_indexSubpass;
};
