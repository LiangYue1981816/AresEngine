#include "Engine.h"
#include "nvwa/debug_new.h"


CEngine* CEngine::pInstance = NULL;
CEngine* CEngine::GetInstance(void)
{
	return pInstance;
}

void CEngine::Create(void *hDC, const char *szShaderCachePath)
{
	if (pInstance == NULL) {
		pInstance = new CEngine(hDC, szShaderCachePath);
	}
}

void CEngine::Destroy(void)
{
	if (pInstance) {
		delete pInstance;
	}

	pInstance = NULL;

	CGfxProfiler::LogGfxMemory();

#if defined (_DEBUG) || defined (DEBUG)
	dump_memory_objects();
#endif
}

CEngine::CEngine(void *hDC, const char *szShaderCachePath)
	: m_totalTime(0.0f)

	, m_pRenderer(NULL)
	, m_pSceneManager(NULL)
{
	m_pRenderer = new CGfxRenderer(hDC, szShaderCachePath);
	m_pSceneManager = new CSceneManager;
}

CEngine::~CEngine(void)
{
	delete m_pSceneManager;
	delete m_pRenderer;
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
	m_totalTime += deltaTime;

	m_pRenderer->SetTime(m_totalTime, deltaTime);
	m_pSceneManager->UpdateLogic(m_totalTime, deltaTime);
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
