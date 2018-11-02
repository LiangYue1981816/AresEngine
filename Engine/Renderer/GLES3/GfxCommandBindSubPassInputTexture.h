#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandBindSubPassInputTexture : public CGfxCommandBase
{
public:
	CGfxCommandBindSubPassInputTexture(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, int indexSubPass)
		: m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
		, m_indexSubPass(indexSubPass)
	{

	}
	virtual ~CGfxCommandBindSubPassInputTexture(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_SUBPASSINPUTTEXTURE, "CommandBindSubPassInputTexture");
		{
			if (m_ptrFrameBuffer.IsValid() && m_ptrRenderPass.IsValid()) {
				if (const SubPassInformation *pSubPassInformation = m_ptrRenderPass->GetSubPass(m_indexSubPass)) {
					for (const auto &itInputAttachment : pSubPassInformation->inputAttachments) {
						const CGfxTexture2DPtr ptrInputTexture = m_ptrFrameBuffer->GetAttachmentTexture(itInputAttachment.first);
						if (ptrInputTexture.IsValid()) {
							Renderer()->BindInputTexture(itInputAttachment.second.c_str(), ptrInputTexture->GetTexture(), GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE);
						}
					}
				}
			}
		}
	}


private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	CGfxRenderPassPtr m_ptrRenderPass;
	int m_indexSubPass;
};
