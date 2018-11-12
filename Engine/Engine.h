#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "PreHeader.h"

#include "Allocator.h"
#include "TaskGraph.h"
#include "GfxRenderer.h"
#include "SceneManager.h"
#include "RenderSolutionBase.h"


#define CreateEngine(api, hDC, width, height, format) CEngine::Create((api), (hDC), (width), (height), (format))
#define DestroyEngine() CEngine::Destroy()
#define Engine() CEngine::GetInstance()


#define SceneManager() CEngine::GetInstance()->GetSceneManager()
#define MainCamera() CEngine::GetInstance()->GetSceneManager()->GetMainCamera()
#define ShadowCamera() CEngine::GetInstance()->GetSceneManager()->GetShadowCamera()


class CALL_API CEngine
{
public:
	static CEngine* GetInstance(void);
	static void Create(GfxApi api, void *hDC, int width, int height, uint32_t format);
	static void Destroy(void);


private:
	CEngine(GfxApi api, void *hDC, int width, int height, uint32_t format);
	virtual ~CEngine(void);


public:
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
	CSceneManager *m_pSceneManager;
	CRenderSolutionBase *m_pRenderSolutions[RENDER_SOLUTION_COUNT];

private:
	static CEngine *pInstance;
};
