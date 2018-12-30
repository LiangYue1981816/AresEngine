#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindSubPassInputTexture : public CGfxCommandBase
{
public:
	CGLES3CommandBindSubPassInputTexture(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass, int indexSubPass)
		: m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
		, m_indexSubPass(indexSubPass)
	{

	}
	virtual ~CGLES3CommandBindSubPassInputTexture(void)
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
	int m_indexSubPass;
};
