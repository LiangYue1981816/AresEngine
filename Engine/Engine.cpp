#include "Engine.h"
#include "RenderSolutionDefault.h"
#include "RenderSolutionDeferred.h"
#include "RenderSolutionForward.h"
#include "RenderSolutionForwardPlus.h"


CEngine* CEngine::pInstance = nullptr;
CEngine* CEngine::GetInstance(void)
{
	return pInstance;
}

void CEngine::Create(GfxApi api, void *hDC, int width, int height, uint32_t format)
{
	if (pInstance == nullptr) {
		pInstance = new CEngine(api, hDC, width, height, format);
	}
}

void CEngine::Destroy(void)
{
	if (pInstance) {
		delete pInstance;
		pInstance = nullptr;
	}

	CGfxProfiler::LogGfxMemory();

#if defined (_DEBUG) || defined (DEBUG)
	dump_memory_objects();
#endif
}

CEngine::CEngine(GfxApi api, void *hDC, int width, int height, uint32_t format)
	: m_indexQueue(0)

	, m_totalLogicTime(0.0f)
	, m_totalRenderTime(0.0f)

	, m_pSceneManager(nullptr)
	, m_pRenderSolutions{ nullptr }
{
	pInstance = this;

	RendererCreate(api, hDC, width, height, format);

	m_pSceneManager = new CSceneManager;
	m_pRenderSolutions[RENDER_SOLUTION_DEFAULT] = new CRenderSolutionDefault;
	m_pRenderSolutions[RENDER_SOLUTION_DEFERRED] = new CRenderSolutionDeferred;
	m_pRenderSolutions[RENDER_SOLUTION_FORWARD] = new CRenderSolutionForward;
	m_pRenderSolutions[RENDER_SOLUTION_FORWARD_PLUS] = new CRenderSolutionForwardPlus;
}

CEngine::~CEngine(void)
{
	delete m_pRenderSolutions[RENDER_SOLUTION_DEFAULT];
	delete m_pRenderSolutions[RENDER_SOLUTION_DEFERRED];
	delete m_pRenderSolutions[RENDER_SOLUTION_FORWARD];
	delete m_pRenderSolutions[RENDER_SOLUTION_FORWARD_PLUS];
	delete m_pSceneManager;

	RendererDestroy();
}

CSceneManager* CEngine::GetSceneManager(void) const
{
	return m_pSceneManager;
}

CRenderSolutionBase* CEngine::GetRenderSolution(RenderSolution solution) const
{
	return m_pRenderSolutions[solution];
}

void CEngine::UpdateLogic(float deltaTime)
{
	m_totalLogicTime += deltaTime;
	m_pSceneManager->UpdateLogic(m_totalLogicTime, deltaTime);
}

void CEngine::UpdateCamera(CGfxCamera *pCamera)
{
	m_pSceneManager->UpdateCamera(pCamera, m_indexQueue);
}

void CEngine::Render(float deltaTime, RenderSolution solution)
{
	m_totalRenderTime += deltaTime;

	m_pRenderSolutions[solution]->Render(m_indexQueue);
	m_pRenderSolutions[solution]->Present(1 - m_indexQueue);
	m_pRenderSolutions[solution]->Clearup(1 - m_indexQueue);

	m_indexQueue = 1 - m_indexQueue;
}
