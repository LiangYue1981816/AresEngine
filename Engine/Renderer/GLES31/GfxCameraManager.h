#pragma once
#include "GfxRenderer.h"


class CGfxCameraManager
{
	friend class CGfxRenderer;


private:
	CGfxCameraManager(void);
	virtual ~CGfxCameraManager(void);


private:
	CGfxCamera* GetCamera(const char *szName);


private:
	eastl::unordered_map<GLuint, CGfxCamera*> m_pCameras;
};
