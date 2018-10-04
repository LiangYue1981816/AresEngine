#pragma once
#include<io.h>
#include "MemoryAllocator.h"
#include "TaskGraph.h"
#include "GfxRenderer.h"
#include "SceneManager.h"


#define CreateEngine(hDC, szShaderCachePath) CEngine::Create((hDC), (szShaderCachePath))
#define DestroyEngine() CEngine::Destroy()

#define Engine() CEngine::GetInstance()
#define Renderer() CEngine::GetInstance()->GetRenderer()
#define SceneManager() CEngine::GetInstance()->GetSceneManager()


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
	void Update(float deltaTime);
	void UpdateCamera(CGfxCamera *pCamera);
	void RenderCamera(CGfxCamera *pCamera);
	void RenderPresent(void);


private:
	float m_totalTime;

private:
	CGfxRenderer *m_pRenderer;
	CSceneManager *m_pSceneManager;

private:
	static CEngine *pInstance;
};
