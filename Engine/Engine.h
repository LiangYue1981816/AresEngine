#pragma once
#include "TaskGraph.h"
#include "GfxRenderer.h"
#include "SceneManager.h"


#define CreateEngine(hDC, szShaderPath, szTexturePath, szMaterialPath, szMeshPath) CEngine::Create((hDC), (szShaderPath), (szTexturePath), (szMaterialPath), (szMeshPath))
#define DestroyEngine() CEngine::Destroy()

#define Engine() CEngine::GetInstance()
#define Renderer() CEngine::GetInstance()->GetRenderer()
#define SceneManager() CEngine::GetInstance()->GetSceneManager()
#define Camera(name) CEngine::GetInstance()->GetCamera(name)


class CEngine
{
public:
	static CEngine* GetInstance(void);
	static void Create(void *hDC, const char *szShaderPath, const char *szTexturePath, const char *szMaterialPath, const char *szMeshPath);
	static void Destroy(void);


private:
	CEngine(void *hDC, const char *szShaderPath, const char *szTexturePath, const char *szMaterialPath, const char *szMeshPath);
	virtual ~CEngine(void);


public:
	CGfxCamera* GetCamera(const char *szName) const;
	CGfxRenderer* GetRenderer(void) const;
	CSceneManager* GetSceneManager(void) const;

public:
	void Update(float deltaTime);
	void UpdateCamera(CGfxCamera *pCamera);
	void RenderCamera(CGfxCamera *pCamera);
	void RenderPresent(void);


private:
	CGfxRenderer *m_pRenderer;
	CSceneManager *m_pSceneManager;

private:
	static CEngine *pInstance;
};
