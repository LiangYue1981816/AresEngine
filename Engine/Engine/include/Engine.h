#pragma once
#include "PreHeader.h"

#include "RenderSolutionBase.h"
#include "RenderSolutionDefault.h"
#include "RenderSolutionDeferred.h"
#include "RenderSolutionForward.h"
#include "RenderSolutionForwardPlus.h"


#define CreateEngine(api, solution, hDC, width, height, format) CEngine::Create((api), (solution), (hDC), (width), (height), (format))
#define DestroyEngine() CEngine::Destroy()
#define Engine() CEngine::GetInstance()


#define SceneManager() CEngine::GetInstance()->GetSceneManager()
#define MainCamera() CEngine::GetInstance()->GetSceneManager()->GetMainCamera()
#define ShadowCamera() CEngine::GetInstance()->GetSceneManager()->GetShadowCamera()


class CALL_API CEngine
{
public:
	static CEngine* GetInstance(void);
	static void Create(GfxApi api, RenderSolution solution, void *hDC, int width, int height, uint32_t format);
	static void Destroy(void);


private:
	CEngine(GfxApi api, RenderSolution solution, void *hDC, int width, int height, uint32_t format);
	virtual ~CEngine(void);


public:
	CSceneManager* GetSceneManager(void) const;
	CRenderSolutionBase* GetRenderSolution(void) const;

public:
	void UpdateLogic(float deltaTime);
	void UpdateCamera(CGfxCamera *pCamera, int indexQueue);

public:
	void Tick(void);
	void Present(void);

private:
	void TickThread(void);
	static void* WorkerThread(void *pParams);


private:
	int m_indexQueue;

private:
	float m_lastTime;
	float m_totalTime;

private:
	CSceneManager *m_pSceneManager;
	CRenderSolutionBase *m_pRenderSolution;

private:
	event_t m_eventExit;
	event_t m_eventFinish;
	event_t m_eventDispatch;
	pthread_t m_thread;

private:
	static CEngine *pInstance;
};
