#pragma once
#include "MemoryAllocator.h"
#include "TaskGraph.h"
#include "GfxRenderer.h"
#include "SceneManager.h"


#define CreateEngine(hDC, szShaderCachePath) CEngine::Create((hDC), (szShaderCachePath))
#define DestroyEngine() CEngine::Destroy()

#define Engine() CEngine::GetInstance()
#define Renderer() CEngine::GetInstance()->GetRenderer()
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
	static void Create(void *hDC, const char *szShaderCachePath);
	static void Destroy(void);


private:
	CEngine(void *hDC, const char *szShaderCachePath);
	virtual ~CEngine(void);


public:
	CGfxRenderer* GetRenderer(void) const;
	CSceneManager* GetSceneManager(void) const;

public:
	void UpdateLogic(float deltaTime);
	void UpdateCamera(CGfxCamera *pCamera);
	void Render(float deltaTime, RenderSolution solution);


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
