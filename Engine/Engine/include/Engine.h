#pragma once
#include "PreHeader.h"


#define CreateEngine(api, solution, hInstance, hWnd, hDC, width, height, format) CEngine::Create((api), (solution), (hInstance), (hWnd), (hDC), (width), (height), (format))
#define DestroyEngine() CEngine::Destroy()
#define Engine() CEngine::GetInstance()


#define SceneManager() CEngine::GetInstance()->GetSceneManager()
#define RenderSolution() CEngine::GetInstance()->GetRenderSolution()


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
	CSceneManager* GetSceneManager(void) const;
	CRenderSolution* GetRenderSolution(void) const;

public:
	float GetDeltaTime(void) const;
	float GetTotalTime(void) const;

public:
	void Wait(void);
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
	CFileManager* m_pFileManager;
	CSceneManager* m_pSceneManager;
	CRenderSolution* m_pRenderSolution;
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
