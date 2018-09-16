#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxCameraManager.h"


CGfxCameraManager::CGfxCameraManager(void)
{

}

CGfxCameraManager::~CGfxCameraManager(void)
{
	for (const auto &itCamera : m_pCameras) {
		delete itCamera.second;
	}

	m_pCameras.clear();
}

CGfxCamera* CGfxCameraManager::GetCamera(const char *szName)
{
	GLuint name = HashValue(szName);

	if (m_pCameras[name] == NULL) {
		m_pCameras[name] = new CGfxCamera;
	}

	return m_pCameras[name];
}
