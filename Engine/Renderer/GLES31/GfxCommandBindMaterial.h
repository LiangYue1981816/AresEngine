#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandBindMaterial : public CGfxCommandBase
{
public:
	CGfxCommandBindMaterial(const CGfxMaterialPtr &ptrMaterial)
		: m_ptrMaterial(ptrMaterial)
	{

	}
	virtual ~CGfxCommandBindMaterial(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_MATERIAL, "CommandBindMaterial");
		{
			if (m_ptrMaterial.IsValid()) {
				Renderer()->BindMaterial(m_ptrMaterial);
			}
			else {
				glUseProgram(0);
			}
		}
	}


private:
	CGfxMaterialPtr m_ptrMaterial;
};
