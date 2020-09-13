#include "Game.h"
#include "Editor.h"
#include "Framework.h"


// Test
#pragma region
CScene* pLightScene = nullptr;
CScene* pMainScene = nullptr;
#pragma endregion

CFramework* CFramework::pInstance = nullptr;
CFramework* CFramework::GetInstance(void)
{
	return pInstance;
}

void CFramework::Create(int width, int height)
{
	if (pInstance == nullptr) {
		pInstance = new CFramework(width, height);
	}
}

void CFramework::Destroy(void)
{
	if (pInstance) {
		delete pInstance;
		pInstance = nullptr;
	}
}


CFramework::CFramework(int width, int height)
	: m_workMode(WORK_MODE_EDITOR)

	, m_pGame(nullptr)
	, m_pEditor(nullptr)

	, m_pImGUI_Console(nullptr)
{
	pInstance = this;

	for (int index = 0; index < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; index++) {
		m_ptrTransferCommandBuffers[index] = GfxRenderer()->NewCommandBuffer(index, true);
		m_ptrComputeCommandBuffers[index] = GfxRenderer()->NewCommandBuffer(index, true);
		m_ptrGraphicCommandBuffers[index] = GfxRenderer()->NewCommandBuffer(index, true);
		m_ptrImGuiCommandBuffers[index] = GfxRenderer()->NewCommandBuffer(index, true);
	}

	m_pGame = new CGame(width, height);
	m_pEditor = new CEditor(width, height);

	m_pImGUI_Console = new CImGUI_Console();

	// Test
#pragma region
	pLightScene = SceneManager()->GetOrCreateScene(HashValue("LightScene"));
	{
		float maxSize = 1.0f;
		float maxRangeX = 13.5f;
		float maxRangeY = 11.0f;
		float maxRangeZ = 6.0f;

		srand(0x0816);

		pLightScene->GetRootNode()->SetWorldScale(1.0f, 1.0f, 1.0f);
		pLightScene->GetRootNode()->SetWorldPosition(-0.5f, 0.0f, 0.0f);

		for (int index = 0; index < 200; index++) {
			float scale = RAND() * maxSize;
			float positionx = (2.0f * RAND() - 1.0f) * maxRangeX;
			float positiony = RAND() * maxRangeY;
			float positionz = (2.0f * RAND() - 1.0f) * maxRangeZ;

			CComponentPointLightPtr ptrPointLight = SceneManager()->GetOrCreateComponentPointLight(SceneManager()->GetNextComponentPointLightName());
			ptrPointLight->SetColor(4.0f, 4.0f, 4.0f);
			ptrPointLight->SetAttenuation(3.0f, 2.0f, 1.0f, scale);

			CSceneNode *pPointLightNode = SceneManager()->GetOrCreateNode(SceneManager()->GetNextNodeName());
			pPointLightNode->SetWorldScale(scale, scale, scale);
			pPointLightNode->SetWorldPosition(positionx, positiony, positionz);
			pPointLightNode->AttachComponentPointLight(ptrPointLight);
			pLightScene->GetRootNode()->AttachNode(pPointLightNode);
		}
	}

	pMainScene = SceneManager()->GetOrCreateScene(HashValue("MainScene"));
	{
		pMainScene->GetRootNode()->SetWorldScale(1.0f, 1.0f, 1.0f);
		pMainScene->GetRootNode()->SetWorldPosition(0.0f, 0.0f, 0.0f);

		CSceneNode *pSponzaSceneNode = ResourceLoader()->LoadSceneMesh("Sponza.xml", pMainScene->GetRootNode(), VERTEX_BINDING, INSTANCE_BINDING);
		pSponzaSceneNode->SetWorldScale(1.0f, 1.0f, 1.0f);
		pSponzaSceneNode->SetWorldPosition(0.0f, 0.0f, 0.0f);

		CSceneNode *pMarcusSceneNode = ResourceLoader()->LoadSceneMesh("Marcus.xml", pMainScene->GetRootNode(), VERTEX_BINDING, INSTANCE_BINDING);
		pMarcusSceneNode->SetWorldScale(1.0f, 1.0f, 1.0f);
		pMarcusSceneNode->SetWorldPosition(0.0f, 0.0f, 0.0f);
		pMarcusSceneNode->SetWorldDirection(-1.0f, 0.0f, -1.0);

		CSceneNode *pHeadSceneNode = ResourceLoader()->LoadSceneMesh("MaleHead.xml", pMainScene->GetRootNode(), VERTEX_BINDING, INSTANCE_BINDING);
		pHeadSceneNode->SetWorldScale(1.0f, 1.0f, 1.0f);
		pHeadSceneNode->SetWorldPosition(-1.0f, 1.0f, 0.0f);
		pHeadSceneNode->SetWorldDirection(-1.0f, 0.0f, -1.0f);
	}
#pragma endregion
}

CFramework::~CFramework(void)
{
	// Test
#pragma region
	SceneManager()->DestroyScene(pMainScene);
	SceneManager()->DestroyScene(pLightScene);
#pragma endregion

	for (int index = 0; index < CGfxSwapChain::SWAPCHAIN_FRAME_COUNT; index++) {
		m_ptrTransferCommandBuffers[index].Release();
		m_ptrComputeCommandBuffers[index].Release();
		m_ptrGraphicCommandBuffers[index].Release();
		m_ptrImGuiCommandBuffers[index].Release();
	}

	delete m_pImGUI_Console;

	delete m_pGame;
	delete m_pEditor;
}

void CFramework::SetWorkMode(CFramework::WorkMode mode)
{
	m_workMode = mode;
}

CWorkModeBase* CFramework::GetWorkMode(void) const
{
	switch (m_workMode) {
	case WORK_MODE_GAME:
		return m_pGame;

	case WORK_MODE_EDITOR:
		return m_pEditor;

	default:
		ASSERT(0);
		return nullptr;
	}
}

void CFramework::OnLButtonDown(int x, int y)
{
	GetWorkMode()->OnLButtonDown(x, y);
}

void CFramework::OnLButtonRelease(int x, int y)
{
	GetWorkMode()->OnLButtonRelease(x, y);
}

void CFramework::OnMouseMove(int x, int y, int ppi)
{
	GetWorkMode()->OnMouseMove(x, y, ppi);
}

void CFramework::OnKeyDown(int key)
{
	GetWorkMode()->OnKeyDown(key);
}

void CFramework::OnKeyRelease(int key)
{
	GetWorkMode()->OnKeyRelease(key);
}

void CFramework::Update(float deltaTime)
{
	GetWorkMode()->Update(deltaTime);
}

void CFramework::Render(CGfxCommandBufferPtr ptrComputeCommandBuffer, CGfxCommandBufferPtr ptrGraphicCommandBuffer, const CGfxSemaphore* pWaitSemaphore)
{
	m_pImGUI_Console->Draw();

	GetWorkMode()->Render(ptrComputeCommandBuffer, ptrGraphicCommandBuffer, pWaitSemaphore);
}

CGfxCommandBufferPtr CFramework::GetTransferCommandBuffer(void)
{
	return m_ptrTransferCommandBuffers[GfxRenderer()->GetSwapChain()->GetFrameIndex()];
}

CGfxCommandBufferPtr CFramework::GetComputeCommandBuffer(void)
{
	return m_ptrComputeCommandBuffers[GfxRenderer()->GetSwapChain()->GetFrameIndex()];
}

CGfxCommandBufferPtr CFramework::GetGraphicCommandBuffer(void)
{
	return m_ptrGraphicCommandBuffers[GfxRenderer()->GetSwapChain()->GetFrameIndex()];
}

CGfxCommandBufferPtr CFramework::GetImGuiCommandBuffer(void)
{
	return m_ptrImGuiCommandBuffers[GfxRenderer()->GetSwapChain()->GetFrameIndex()];
}
