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
	: m_numFrames(0)

	, m_lastTime(0.0f)
	, m_deltaTime(0.0f)
	, m_totalTime(0.0f)

	, m_pSettings(nullptr)
	, m_pFileManager(nullptr)
	, m_pShaderCompiler(nullptr)
	, m_pResourceLoader(nullptr)

	, m_pSceneManager(nullptr)
	, m_pRenderSystem(nullptr)

	, m_taskPoolUpdate("TashPool_Update")
	, m_taskPoolRender("TashPool_Render")
	, m_taskGraphUpdate("TashGraph_Update")
	, m_taskGraphRender("TashGraph_Render")
{
	pInstance = this;

	m_pSettings = new CSettings;

	m_pFileManager = new CFileManager;
	m_pFileManager->SetPath("../Data/Engine", ".xml");
	m_pFileManager->SetPath("../Data/Engine", ".png");
	m_pFileManager->SetPath("../Data/Engine", ".tga");
	m_pFileManager->SetPath("../Data/Engine", ".dds");
	m_pFileManager->SetPath("../Data/Engine", ".glsl");
	m_pFileManager->SetPath("../Data/Engine", ".vert");
	m_pFileManager->SetPath("../Data/Engine", ".frag");
	m_pFileManager->SetPath("../Data/Engine", ".comp");
	m_pFileManager->SetPath("../Data/Engine", ".mesh");
	m_pFileManager->SetPath("../Data/Engine", ".material");

	m_pShaderCompiler = new CShaderCompiler;
	m_pShaderCompiler->SetCachePath("../Data/Engine/ShaderCache");
	m_pShaderCompiler->AddIncludePath("../Data/Engine/Shader/inc");

	m_pResourceLoader = new CResourceLoader;

	m_pSceneManager = new CSceneManager;
	m_pRenderSystem = new CRenderSystem(api, hInstance, hWnd, hDC, width, height, format);

	m_pRenderSystem->CreateRenderTextures();
	m_pRenderSystem->CreatePasses();

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

	m_pRenderSystem->DestroyPasses();
	m_pRenderSystem->DestroyRenderTextures();

	delete m_pFileManager;
	delete m_pShaderCompiler;
	delete m_pResourceLoader;

	delete m_pSettings;
	delete m_pSceneManager;
	delete m_pRenderSystem;
}

uint32_t CEngine::GetFrameCount(void) const
{
	return m_numFrames;
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
	float currTime = Tick() / 1000000.0f;

	m_deltaTime = currTime - m_lastTime;
	m_totalTime = m_totalTime + m_deltaTime;

	m_lastTime = currTime;
	m_numFrames = m_numFrames + 1;

	event_unsignal(&m_eventFinish);
	event_signal(&m_eventDispatch);
}

void CEngine::RenderDefault(CCamera* pCamera, bool bPresent)
{
	m_pRenderSystem->UpdateCamera(m_taskPoolRender, m_taskGraphRender, pCamera, 0xffffffff, true);
	m_pRenderSystem->RenderDefault(m_taskPoolRender, m_taskGraphRender, pCamera, bPresent);
}

void CEngine::RenderForwardLighting(CCamera* pCamera, bool bPresent)
{
	m_pRenderSystem->UpdateCamera(m_taskPoolRender, m_taskGraphRender, pCamera, 0xffffffff, true);
	m_pRenderSystem->RenderForwardLighting(m_taskPoolRender, m_taskGraphRender, pCamera, bPresent);
}

void CEngine::RenderDeferredLighting(CCamera* pCamera, bool bPresent)
{
	m_pRenderSystem->UpdateCamera(m_taskPoolRender, m_taskGraphRender, pCamera, 0xffffffff, true);
	m_pRenderSystem->RenderDeferredLighting(m_taskPoolRender, m_taskGraphRender, pCamera, bPresent);
}

void CEngine::RenderTileBaseDeferredLighting(CCamera* pCamera, bool bPresent)
{
	m_pRenderSystem->UpdateCamera(m_taskPoolRender, m_taskGraphRender, pCamera, 0xffffffff, true);
	m_pRenderSystem->RenderTileBaseDeferredLighting(m_taskPoolRender, m_taskGraphRender, pCamera, bPresent);
}

void CEngine::UpdateThread(void)
{
	m_pSceneManager->UpdateLogic(m_taskPoolUpdate, m_taskGraphUpdate, m_totalTime, m_deltaTime);
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
