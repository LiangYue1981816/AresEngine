#pragma once
#include "PreHeader.h"

#include "RenderSolutionBase.h"
#include "RenderSolutionDefault.h"
#include "RenderSolutionDeferred.h"
#include "RenderSolutionForward.h"
#include "RenderSolutionTiledBaseDeferred.h"


#define CreateEngine(api, solution, hInstance, hWnd, hDC, width, height, format) CEngine::Create((api), (solution), (hInstance), (hWnd), (hDC), (width), (height), (format))
#define DestroyEngine() CEngine::Destroy()
#define Engine() CEngine::GetInstance()


#define SceneManager() CEngine::GetInstance()->GetSceneManager()
#define RenderSolution() CEngine::GetInstance()->GetRenderSolution()
#define MainCamera() CEngine::GetInstance()->GetRenderSolution()->GetMainCamera()
#define ShadowCamera() CEngine::GetInstance()->GetRenderSolution()->GetShadowCamera()


typedef enum RenderSolution {
	RENDER_SOLUTION_DEFAULT = 0,
	RENDER_SOLUTION_FORWARD,
	RENDER_SOLUTION_DEFERRED,
	RENDER_SOLUTION_TILED_BASE_DEFERRED,
	RENDER_SOLUTION_COUNT
} RenderSolution;


class CALL_API CEngine
{
public:
	static CEngine* GetInstance(void);
	static void Create(GfxApi api, RenderSolution solution, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format);
	static void Destroy(void);


private:
	CEngine(GfxApi api, RenderSolution solution, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format);
	virtual ~CEngine(void);


public:
	CGfxRenderer* GetRenderer(void) const;
	CSceneManager* GetSceneManager(void) const;
	CRenderSolutionBase* GetRenderSolution(void) const;

public:
	float GetDeltaTime(void) const;
	float GetTotalTime(void) const;

public:
	void Update(void);
	void Render(void);

private:
	void UpdateThread(void);
	static void* WorkThread(void* pParams);


private:
	int m_indexQueue;

private:
	float m_lastTime;
	float m_deltaTime;
	float m_totalTime;

private:
	CGfxRenderer* m_pRenderer;
	CRenderSolutionBase* m_pRenderSolution;

	CFileManager* m_pFileManager;
	CSceneManager* m_pSceneManager;
	CResourceLoader* m_pResourceLoader;

#ifdef PLATFORM_WINDOWS
	CShaderCompiler* m_pShaderCompiler;
#endif

private:
	event_t m_eventExit;
	event_t m_eventFinish;
	event_t m_eventDispatch;
	pthread_t m_thread;

private:
	static CEngine* pInstance;
};
