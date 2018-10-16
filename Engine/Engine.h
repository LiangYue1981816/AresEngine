#pragma once
#include "MemoryAllocator.h"
#include "TaskGraph.h"
#include "GfxRenderer.h"
#include "SceneManager.h"


#define LOG_TAG_MEMORY "Memory"
#define LOG_TAG_RENDERER "GfxRenderer"


#define CreateEngine(hDC, szShaderCachePath, nScreenWidth, nScreenHeight, nScreenPixelFormat) CEngine::Create((hDC), (szShaderCachePath), (nScreenWidth), (nScreenHeight), (nScreenPixelFormat))
#define DestroyEngine() CEngine::Destroy()

#define Engine() CEngine::GetInstance()
#define Renderer() CGfxRenderer::GetInstance()
#define SceneManager() CEngine::GetInstance()->GetSceneManager()
#define MainCamera() CEngine::GetInstance()->GetSceneManager()->GetMainCamera()
#define ShadowCamera() CEngine::GetInstance()->GetSceneManager()->GetShadowCamera()


typedef enum RenderSolution {
	RENDER_SOLUTION_DEFAULT = 0,
	RENDER_SOLUTION_DEFERRED,
	RENDER_SOLUTION_FORWARD,
	RENDER_SOLUTION_FORWARD_PLUS,
	RENDER_SOLUTION_COUNT
} RenderSolution;

class CRenderSolutionBase;
class CRenderSolutionDefault;
class CRenderSolutionDeferred;
class CRenderSolutionForward;
class CRenderSolutionForwardPlus;


class CEngine
{
public:
	static CEngine* GetInstance(void);
	static void Create(void *hDC, const char *szShaderCachePath, int screenWidth, int screenHeight, uint32_t screenPixelFormat);
	static void Destroy(void);


private:
	CEngine(void *hDC, const char *szShaderCachePath, int screenWidth, int screenHeight, uint32_t screenPixelFormat);
	virtual ~CEngine(void);


public:
	CGfxRenderer* GetRenderer(void) const;
	CSceneManager* GetSceneManager(void) const;
	CRenderSolutionBase* GetRenderSolution(RenderSolution solution) const;

public:
	void UpdateLogic(float deltaTime);
	void UpdateCamera(CGfxCamera *pCamera);
	void Render(float deltaTime, RenderSolution solution);


private:
	int m_indexQueue;

private:
	float m_totalLogicTime;
	float m_totalRenderTime;

private:
	CGfxRenderer *m_pRenderer;
	CSceneManager *m_pSceneManager;
	CRenderSolutionBase *m_pRenderSolutions[RENDER_SOLUTION_COUNT];

private:
	static CEngine *pInstance;
};
