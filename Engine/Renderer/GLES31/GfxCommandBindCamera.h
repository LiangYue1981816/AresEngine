#pragma once
#include <gl31.h>
#include "GfxRenderer.h"
#include "GfxCommandBuffer.h"


class CGfxCommandBindCamera : public CGfxCommandBase
{
public:
	CGfxCommandBindCamera(CGfxCamera *pCamera)
		: m_pCamera(pCamera)
	{

	}
	virtual ~CGfxCommandBindCamera(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		if (m_pCamera) {
			Renderer()->BindCamera(m_pCamera);
		}
	}


private:
	CGfxCamera *m_pCamera;
};
