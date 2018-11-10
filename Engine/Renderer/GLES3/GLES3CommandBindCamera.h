#pragma once
#include "GfxCommandBuffer.h"


class CGLES3CommandBindCamera : public CGfxCommandBase
{
public:
	CGLES3CommandBindCamera(CGfxUniformCameraPtr &ptrUniformCamera)
		: m_ptrUniformCamera(ptrUniformCamera)
	{

	}
	virtual ~CGLES3CommandBindCamera(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample sample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_CAMERA, "CommandBindCamera");
		{
			if (m_ptrUniformCamera.IsValid()) {
				GLES3Renderer()->BindUniformCamera(m_ptrUniformCamera);
			}
		}
	}


private:
	CGfxUniformCameraPtr m_ptrUniformCamera;
};
