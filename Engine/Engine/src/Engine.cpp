#include "EngineHeader.h"


extern int dump_memory_objects();
extern int check_mem_corruption();


CEngine* CEngine::pInstance = nullptr;
CEngine* CEngine::GetInstance(void)
{
	return pInstance;
}

void CEngine::Create(GfxApi api, RenderSolution solution, void *hDC, int width, int height, uint32_t format)
{
	if (pInstance == nullptr) {
		pInstance = new CEngine(api, solution, hDC, width, height, format);
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

CEngine::CEngine(GfxApi api, RenderSolution solution, void *hDC, int width, int height, uint32_t format)
	: m_indexQueue(0)

	, m_lastTime(0.0f)
	, m_deltaTime(0.0f)
	, m_totalTime(0.0f)

	, m_pRenderer(nullptr)
	, m_pSceneManager(nullptr)
	, m_pRenderSolution(nullptr)
{
	pInstance = this;

	switch ((int)api) {
	case GFX_API_GLES3:
		m_pRenderer = new CGLES3Renderer(hDC, width, height, format);
		break;

	case GFX_API_VULKAN:
		break;

	case GFX_API_METAL:
		break;
	}

	switch ((int)solution) {
	case RENDER_SOLUTION_DEFAULT:
		m_pRenderSolution = new CRenderSolutionDefault;
		break;

	case RENDER_SOLUTION_DEFERRED:
		m_pRenderSolution = new CRenderSolutionDeferred;
		break;

	case RENDER_SOLUTION_FORWARD:
		m_pRenderSolution = new CRenderSolutionForward;
		break;

	case RENDER_SOLUTION_FORWARD_PLUS:
		m_pRenderSolution = new CRenderSolutionForwardPlus;
		break;
	}

	m_pSceneManager = new CSceneManager;

	event_init(&m_eventExit, 0);
	event_init(&m_eventFinish, 1);
	event_init(&m_eventDispatch, 0);
	pthread_create(&m_thread, nullptr, WorkThread, this);
}

CEngine::~CEngine(void)
{
	event_signal(&m_eventExit);
	event_signal(&m_eventDispatch);
	pthread_join(m_thread, nullptr);
	event_destroy(&m_eventExit);
	event_destroy(&m_eventFinish);
	event_destroy(&m_eventDispatch);

	delete m_pRenderSolution;
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

CRenderSolutionBase* CEngine::GetRenderSolution(void) const
{
	return m_pRenderSolution;
}

float CEngine::GetDeltaTime(void) const
{
	return m_deltaTime;
}

float CEngine::GetTotalTime(void) const
{
	return m_totalTime;
}

void CEngine::Tick(void)
{
	event_wait(&m_eventFinish);
	{
		m_indexQueue = 1 - m_indexQueue;
	}
	event_unsignal(&m_eventFinish);
	event_signal(&m_eventDispatch);
}

void CEngine::Present(void)
{
	m_pRenderSolution->Present(1 - m_indexQueue);
	m_pRenderSolution->Clearup(1 - m_indexQueue);
}

void CEngine::TickThread(void)
{
	float currTime = tick() / 1000000.0f;

	m_deltaTime = currTime - m_lastTime;
	m_totalTime = m_totalTime + m_deltaTime;
	m_lastTime = currTime;

	m_pRenderSolution->Render(m_indexQueue);
}

void* CEngine::WorkThread(void *pParams)
{
	if (CEngine *pEngine = (CEngine *)pParams) {
		while (true) {
			event_wait(&pEngine->m_eventDispatch);
			{
				if (event_wait_timeout(&pEngine->m_eventExit, 0) == NO_ERROR) {
					break;
				}

				pEngine->TickThread();
			}
			event_reset(&pEngine->m_eventDispatch);
			event_signal(&pEngine->m_eventFinish);
		}
	}

	return nullptr;
}
