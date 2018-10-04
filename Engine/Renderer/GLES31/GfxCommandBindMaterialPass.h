#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandBindMaterialPass : public CGfxCommandBase
{
public:
	CGfxCommandBindMaterialPass(const CGfxMaterialPtr &ptrMaterial, uint32_t namePass)
		: m_ptrMaterial(ptrMaterial)
		, m_namePass(namePass)
	{

	}
	virtual ~CGfxCommandBindMaterialPass(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_MATERIALPASS, "CommandBindMaterialPass");
		{
			if (m_ptrMaterial.IsValid()) {
				if (const CGfxMaterialPass *pPass = m_ptrMaterial->GetPass(m_namePass)) {
					Renderer()->BindMaterialPass((CGfxMaterialPass *)pPass);
				}
				else {
					glBindProgramPipeline(0);
				}
			}
			else {
				glBindProgramPipeline(0);
			}
		}
	}


private:
	CGfxMaterialPtr m_ptrMaterial;
	uint32_t m_namePass;
};
