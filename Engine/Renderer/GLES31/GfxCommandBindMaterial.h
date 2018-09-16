#pragma once
#include <gl31.h>
#include "GfxRenderer.h"
#include "GfxCommandBuffer.h"


class CGfxCommandBindMaterial : public CGfxCommandBase
{
public:
	CGfxCommandBindMaterial(CGfxMaterial *pMaterial)
		: m_pMaterial(pMaterial)
	{
		if (m_pMaterial) {
			m_pMaterial->Retain();
		}
	}
	virtual ~CGfxCommandBindMaterial(void)
	{
		if (m_pMaterial) {
			m_pMaterial->Release();
		}
	}

public:
	virtual void Execute(void) const
	{
		if (m_pMaterial) {
			Renderer()->BindMaterial(m_pMaterial);
		}
		else {
			glUseProgram(0);
		}
	}


private:
	CGfxMaterial *m_pMaterial;
};
