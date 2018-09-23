#pragma once
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
		CGfxProfilerSample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_CAMERA, "CommandBindCamera");
		{
			if (m_pCamera) {
				Renderer()->BindCamera(m_pCamera);
			}
		}
	}


private:
	CGfxCamera *m_pCamera;
};
