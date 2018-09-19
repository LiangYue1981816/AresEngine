#pragma once
#include <gl31.h>
#include "GfxRenderer.h"
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
		if (m_ptrMaterial.IsValid()) {
			Renderer()->BindMaterial(m_ptrMaterial);
		}
		else {
			glUseProgram(0);
		}
	}


private:
	CGfxMaterialPtr m_ptrMaterial;
};
