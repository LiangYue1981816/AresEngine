#include "EngineHeader.h"
#include "VKRenderer.h"
#include "GLES3Renderer.h"


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

	, m_pRenderer(nullptr)

	, m_pRenderSolution{ nullptr }
	, m_pCurrentRenderSolution(nullptr)

	, m_pFileManager(nullptr)
	, m_pSceneManager(nullptr)
	, m_pResourceLoader(nullptr)

#ifdef PLATFORM_WINDOWS
	, m_pShaderCompiler(nullptr)
#endif
{
	pInstance = this;

	switch ((int)api) {
	case GFX_API_GLES3:
		m_pRenderer = new CGLES3Renderer(hInstance, hWnd, hDC, width, height, format);
		break;

	case GFX_API_VULKAN:
		m_pRenderer = new CVKRenderer(hInstance, hWnd, hDC, width, height, format);
		break;
	}

	m_pRenderSolution[RENDER_SOLUTION_DEFAULT] = new CRenderSolutionDefault;
	m_pRenderSolution[RENDER_SOLUTION_FORWARD] = new CRenderSolutionForward;
	m_pRenderSolution[RENDER_SOLUTION_DEFERRED] = new CRenderSolutionDeferred;
	m_pRenderSolution[RENDER_SOLUTION_TILED_BASE_FORWARD] = new CRenderSolutionTiledBaseForward;
	m_pRenderSolution[RENDER_SOLUTION_TILED_BASE_DEFERRED] = new CRenderSolutionTiledBaseDeferred;

	m_pCurrentRenderSolution = m_pRenderSolution[RENDER_SOLUTION_DEFAULT];
	m_pCurrentRenderSolution->Create();

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

	delete m_pRenderSolution[RENDER_SOLUTION_DEFAULT];
	delete m_pRenderSolution[RENDER_SOLUTION_FORWARD];
	delete m_pRenderSolution[RENDER_SOLUTION_DEFERRED];
	delete m_pRenderSolution[RENDER_SOLUTION_TILED_BASE_DEFERRED];

	delete m_pRenderer;
}

void CEngine::SetRenderSolution(RenderSolution solution, int samples)
{
	m_pCurrentRenderSolution->Destroy();
	m_pCurrentRenderSolution = m_pRenderSolution[solution];
	m_pCurrentRenderSolution->Create(samples);
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
	return m_pCurrentRenderSolution;
}

float CEngine::GetDeltaTime(void) const
{
	return m_deltaTime;
}

float CEngine::GetTotalTime(void) const
{
	return m_totalTime;
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
	m_pCurrentRenderSolution->Render(1 - m_indexQueue);
}

void CEngine::UpdateThread(void)
{
	float currTime = Tick() / 1000000.0f;

	m_deltaTime = currTime - m_lastTime;
	m_totalTime = m_totalTime + m_deltaTime;
	m_lastTime = currTime;

	m_pCurrentRenderSolution->Update(m_indexQueue);
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
