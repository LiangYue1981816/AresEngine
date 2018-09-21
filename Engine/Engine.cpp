#include "Engine.h"
#include "nvwa/debug_new.h"


CEngine* CEngine::pInstance = NULL;
CEngine* CEngine::GetInstance(void)
{
	return pInstance;
}

void CEngine::Create(void *hDC, const char *szShaderPath, const char *szTexturePath, const char *szMaterialPath, const char *szMeshPath)
{
	if (pInstance == NULL) {
		pInstance = new CEngine(hDC, szShaderPath, szTexturePath, szMaterialPath, szMeshPath);
	}
}

void CEngine::Destroy(void)
{
	if (pInstance) {
		delete pInstance;
	}

	pInstance = NULL;

#ifdef _DEBUG
	dump_memory_objects();
#endif
}

CEngine::CEngine(void *hDC, const char *szShaderPath, const char *szTexturePath, const char *szMaterialPath, const char *szMeshPath)
	: m_pRenderer(NULL)
	, m_pSceneManager(NULL)
{
	m_pRenderer = new CGfxRenderer(hDC, szShaderPath, szTexturePath, szMaterialPath, szMeshPath);
	m_pSceneManager = new CSceneManager;
}

CEngine::~CEngine(void)
{
	delete m_pSceneManager;
	delete m_pRenderer;
}

CGfxCamera* CEngine::GetCamera(const char *szName) const
{
	return m_pRenderer->GetCamera(szName);
}

CGfxRenderer* CEngine::GetRenderer(void) const
{
	return m_pRenderer;
}

CSceneManager* CEngine::GetSceneManager(void) const
{
	return m_pSceneManager;
}

void CEngine::Update(float deltaTime)
{
	m_pSceneManager->Update(deltaTime);
}

void CEngine::UpdateCamera(CGfxCamera *pCamera)
{
	m_pSceneManager->UpdateCamera(pCamera);
}

void CEngine::RenderCamera(CGfxCamera *pCamera)
{
	m_pSceneManager->RenderCamera(pCamera);
}

void CEngine::RenderPresent(void)
{
	m_pRenderer->Present();
}
