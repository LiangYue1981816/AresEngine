#include "EngineHeader.h"


extern int dump_memory_objects();
extern int check_mem_corruption();


CEngine* CEngine::pInstance = nullptr;
CEngine* CEngine::GetInstance(void)
{
	return pInstance;
}

void CEngine::Create(GfxApi api, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format, const char* szPath)
{
	if (pInstance == nullptr) {
		pInstance = new CEngine(api, hInstance, hWnd, hDC, width, height, format, szPath);
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

CEngine::CEngine(GfxApi api, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format, const char* szPath)
	: m_numFrames(0)

	, m_lastTime(0.0f)
	, m_deltaTime(0.0f)
	, m_totalTime(0.0f)

	, m_pSettings(nullptr)

	, m_pFileManager(nullptr)
	, m_pResourceLoader(nullptr)
	, m_pShaderCompiler(nullptr)

	, m_pSceneManager(nullptr)
	, m_pRenderSystem(nullptr)

	, m_taskPoolUpdate("TashPool_Update")
	, m_taskPoolRender("TashPool_Render")
	, m_taskGraphUpdate("TashGraph_Update")
	, m_taskGraphRender("TashGraph_Render")
{
	pInstance = this;

	m_pSettings = new CSettings;
	m_pSettings->SetValue("RenderSystem.Shadow.Factor", 1.0f);
	m_pSettings->SetValue("RenderSystem.Shadow.SplitFactor0", exp(-4.0f));
	m_pSettings->SetValue("RenderSystem.Shadow.SplitFactor1", exp(-3.0f));
	m_pSettings->SetValue("RenderSystem.Shadow.SplitFactor2", exp(-2.0f));
	m_pSettings->SetValue("RenderSystem.Shadow.SplitFactor3", exp(-1.0f));
	m_pSettings->SetValue("RenderSystem.SSAO.SampleCount", 8.0f);
	m_pSettings->SetValue("RenderSystem.SSAO.MinSampleRadius", 0.02f);
	m_pSettings->SetValue("RenderSystem.SSAO.MaxSampleRadius", 1.25f);
	m_pSettings->SetValue("RenderSystem.SSAO.MinDepthRange", 0.00f);
	m_pSettings->SetValue("RenderSystem.SSAO.MaxDepthRange", 0.20f);
	m_pSettings->SetValue("RenderSystem.SSAO.BlurRange", 1.00f);
	m_pSettings->SetValue("RenderSystem.Bloom.LuminanceThreshold", 1.00f);
	m_pSettings->SetValue("RenderSystem.Bloom.BlurRange.FirstTime", 1.00f);
	m_pSettings->SetValue("RenderSystem.Bloom.BlurRange.SecondTime", 0.75f);
	m_pSettings->SetValue("RenderSystem.AutoExposure.Lower", 0.10f);
	m_pSettings->SetValue("RenderSystem.AutoExposure.Upper", 0.90f);
	m_pSettings->SetValue("RenderSystem.AutoExposure.Luminance", 0.15f);
	m_pSettings->SetValue("RenderSystem.AutoExposure.MinScaleLuminance", 0.20f);
	m_pSettings->SetValue("RenderSystem.AutoExposure.MaxScaleLuminance", 2.00f);

	m_pFileManager = new CFileManager;
	m_pFileManager->SetPath(szPath, ".xml");
	m_pFileManager->SetPath(szPath, ".png");
	m_pFileManager->SetPath(szPath, ".tga");
	m_pFileManager->SetPath(szPath, ".dds");
	m_pFileManager->SetPath(szPath, ".glsl");
	m_pFileManager->SetPath(szPath, ".vert");
	m_pFileManager->SetPath(szPath, ".frag");
	m_pFileManager->SetPath(szPath, ".comp");
	m_pFileManager->SetPath(szPath, ".mesh");
	m_pFileManager->SetPath(szPath, ".material");

	m_pResourceLoader = new CResourceLoader;
	m_pShaderCompiler = new CShaderCompiler;
	m_pShaderCompiler->SetCachePath("../Data/Engine/ShaderCache");
	m_pShaderCompiler->AddIncludePath("../Data/Engine/Shader/inc");

	m_pSceneManager = new CSceneManager;
	m_pRenderSystem = new CRenderSystem;
	m_pRenderSystem->Create(api, hInstance, hWnd, hDC, width, height, format);

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

	delete m_pSettings;

	delete m_pFileManager;
	delete m_pResourceLoader;
	delete m_pShaderCompiler;

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

void CEngine::RenderUnlit(CCamera* pCamera, CGfxCommandBufferPtr ptrComputeCommandBuffer, CGfxCommandBufferPtr ptrGraphicCommandBuffer, const CGfxSemaphore* pWaitSemaphore)
{
	m_pRenderSystem->UpdateCamera(m_taskPoolRender, m_taskGraphRender, pCamera, 0xffffffff, true);
	m_pRenderSystem->RenderUnlit(m_taskPoolRender, m_taskGraphRender, pCamera, ptrComputeCommandBuffer, ptrGraphicCommandBuffer, pWaitSemaphore);
}

void CEngine::RenderForwardShading(CCamera* pCamera, CGfxCommandBufferPtr ptrComputeCommandBuffer, CGfxCommandBufferPtr ptrGraphicCommandBuffer, const CGfxSemaphore* pWaitSemaphore)
{
	m_pRenderSystem->UpdateCamera(m_taskPoolRender, m_taskGraphRender, pCamera, 0xffffffff, true);
	m_pRenderSystem->RenderForwardShading(m_taskPoolRender, m_taskGraphRender, pCamera, ptrComputeCommandBuffer, ptrGraphicCommandBuffer, pWaitSemaphore);
}

void CEngine::RenderDeferredShading(CCamera* pCamera, CGfxCommandBufferPtr ptrComputeCommandBuffer, CGfxCommandBufferPtr ptrGraphicCommandBuffer, const CGfxSemaphore* pWaitSemaphore)
{
	m_pRenderSystem->UpdateCamera(m_taskPoolRender, m_taskGraphRender, pCamera, 0xffffffff, true);
	m_pRenderSystem->RenderDeferredShading(m_taskPoolRender, m_taskGraphRender, pCamera, ptrComputeCommandBuffer, ptrGraphicCommandBuffer, pWaitSemaphore);
}

void CEngine::RenderTileDeferredShading(CCamera* pCamera, CGfxCommandBufferPtr ptrComputeCommandBuffer, CGfxCommandBufferPtr ptrGraphicCommandBuffer, const CGfxSemaphore* pWaitSemaphore)
{
	m_pRenderSystem->UpdateCamera(m_taskPoolRender, m_taskGraphRender, pCamera, 0xffffffff, true);
	m_pRenderSystem->RenderTileDeferredShading(m_taskPoolRender, m_taskGraphRender, pCamera, ptrComputeCommandBuffer, ptrGraphicCommandBuffer, pWaitSemaphore);
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
