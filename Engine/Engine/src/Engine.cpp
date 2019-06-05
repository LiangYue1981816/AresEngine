#include "EngineHeader.h"


extern int dump_memory_objects();
extern int check_mem_corruption();


CEngine* CEngine::pInstance = nullptr;
CEngine* CEngine::GetInstance(void)
{
	return pInstance;
}

void CEngine::Create(GfxApi api, RenderSolution solution, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format)
{
	if (pInstance == nullptr) {
		pInstance = new CEngine(api, solution, hInstance, hWnd, hDC, width, height, format);
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

CEngine::CEngine(GfxApi api, RenderSolution solution, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format)
	: m_indexQueue(0)

	, m_lastTime(0.0f)
	, m_deltaTime(0.0f)
	, m_totalTime(0.0f)

	, m_pFileManager(nullptr)
	, m_pSceneManager(nullptr)
	, m_pRenderSolution(nullptr)
	, m_pResourceLoader(nullptr)

#ifdef PLATFORM_WINDOWS
	, m_pShaderCompiler(nullptr)
#endif
{
	pInstance = this;

	m_pRenderSolution = new CRenderSolution(api, solution, hInstance, hWnd, hDC, width, height, format);
	m_pFileManager = new CFileManager;
	m_pSceneManager = new CSceneManager;
	m_pResourceLoader = new CResourceLoader;

#ifdef PLATFORM_WINDOWS
	m_pShaderCompiler = new CShaderCompiler;
#endif

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

#ifdef PLATFORM_WINDOWS
	delete m_pShaderCompiler;
#endif

	delete m_pResourceLoader;
	delete m_pSceneManager;
	delete m_pFileManager;
	delete m_pRenderSolution;
}

float CEngine::GetDeltaTime(void) const
{
	return m_deltaTime;
}

float CEngine::GetTotalTime(void) const
{
	return m_totalTime;
}

CSceneManager* CEngine::GetSceneManager(void) const
{
	return m_pSceneManager;
}

CRenderSolution* CEngine::GetRenderSolution(void) const
{
	return m_pRenderSolution;
}

void CEngine::Wait(void)
{
	event_wait(&m_eventFinish);
	m_indexQueue = 1 - m_indexQueue;
}

void CEngine::Update(void)
{
	event_unsignal(&m_eventFinish);
	event_signal(&m_eventDispatch);
}

void CEngine::Render(void)
{
	m_pRenderSolution->GetCurrentRenderSolution()->Render(1 - m_indexQueue);
}

void CEngine::UpdateThread(void)
{
	float currTime = Tick() / 1000000.0f;

	m_deltaTime = currTime - m_lastTime;
	m_totalTime = m_totalTime + m_deltaTime;
	m_lastTime = currTime;

	m_pSceneManager->UpdateLogic(m_totalTime, m_deltaTime);
	m_pRenderSolution->GetCurrentRenderSolution()->UpdateCamera(m_indexQueue);
}

void* CEngine::WorkThread(void* pParams)
{
	if (CEngine* pEngine = (CEngine*)pParams) {
		while (true) {
			event_wait(&pEngine->m_eventDispatch);
			{
				if (event_wait_timeout(&pEngine->m_eventExit, 0) == NO_ERROR) {
					break;
				}

				pEngine->UpdateThread();
			}
			event_reset(&pEngine->m_eventDispatch);
			event_signal(&pEngine->m_eventFinish);
		}
	}

	return nullptr;
}
