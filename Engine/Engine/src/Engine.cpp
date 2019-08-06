#include "EngineHeader.h"


extern int dump_memory_objects();
extern int check_mem_corruption();


CEngine* CEngine::pInstance = nullptr;
CEngine* CEngine::GetInstance(void)
{
	return pInstance;
}

void CEngine::Create(GfxApi api, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format)
{
	if (pInstance == nullptr) {
		pInstance = new CEngine(api, hInstance, hWnd, hDC, width, height, format);
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

CEngine::CEngine(GfxApi api, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format)
	: m_lastTime(0.0f)
	, m_deltaTime(0.0f)
	, m_totalTime(0.0f)

	, m_pFileManager(nullptr)
	, m_pSceneManager(nullptr)
	, m_pRenderSystem(nullptr)
	, m_pResourceLoader(nullptr)

#ifdef PLATFORM_WINDOWS
	, m_pShaderCompiler(nullptr)
#endif

	, m_taskGraphUpdate("TashGraph_Update")
	, m_taskGraphRender("TashGraph_Render")
{
	pInstance = this;

	m_pRenderSystem = new CRenderSystem(api, hInstance, hWnd, hDC, width, height, format);
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
	delete m_pRenderSystem;
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

CRenderSystem* CEngine::GetRenderSystem(void) const
{
	return m_pRenderSystem;
}

void CEngine::Wait(void)
{
	event_wait(&m_eventFinish);
}

void CEngine::Update(void)
{
	event_unsignal(&m_eventFinish);
	event_signal(&m_eventDispatch);
}

void CEngine::RenderDefault(CCamera* pCamera, bool bPresent)
{
	m_pRenderSystem->Update(m_taskGraphRender, pCamera, false);
	m_pRenderSystem->RenderDefault(m_taskGraphRender, pCamera, bPresent);
}

void CEngine::RenderForwardLighting(CCamera* pCamera, bool bShadow, bool bPresent)
{
	m_pRenderSystem->Update(m_taskGraphRender, pCamera, bShadow);
	m_pRenderSystem->RenderForwardLighting(m_taskGraphRender, pCamera, bShadow, bPresent);
}

void CEngine::UpdateThread(void)
{
	float currTime = Tick() / 1000000.0f;

	m_deltaTime = currTime - m_lastTime;
	m_totalTime = m_totalTime + m_deltaTime;
	m_lastTime = currTime;

	m_pSceneManager->UpdateLogic(m_taskGraphUpdate, m_totalTime, m_deltaTime);
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
