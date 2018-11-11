#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindMaterialPass : public CGfxCommandBase
{
public:
	CGLES3CommandBindMaterialPass(const CGfxMaterialPtr &ptrMaterial, uint32_t namePass)
		: m_ptrMaterial(ptrMaterial)
		, m_namePass(namePass)
	{

	}
	virtual ~CGLES3CommandBindMaterialPass(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_MATERIALPASS, "CommandBindMaterialPass");
		{
			if (m_ptrMaterial.IsValid()) {
				if (CGfxMaterialPass *pPass = m_ptrMaterial->GetPass(m_namePass)) {
					GLES3Renderer()->BindMaterialPass(pPass);
				}
			}
		}
	}


private:
	CGfxMaterialPtr m_ptrMaterial;
	uint32_t m_namePass;
};
