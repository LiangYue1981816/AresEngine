#pragma once
#include "PreHeader.h"

#include "ComponentManager.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ComponentSkin.h"
#include "ComponentParticle.h"
#include "ComponentPointLight.h"

#include "Scene.h"
#include "SceneNode.h"


class CSceneManager
{
	friend class CEngine;


private:
	CSceneManager(void);
	virtual ~CSceneManager(void);


public:
	uint32_t GetNextNodeName(void) const;
	uint32_t GetNextComponentMeshName(void) const;
	uint32_t GetNextComponentSkinName(void) const;
	uint32_t GetNextComponentParticleName(void) const;
	uint32_t GetNextComponentPointLightName(void) const;

public:
	CScene* GetOrCreateScene(uint32_t name);
	void DestroyScene(CScene* pScene);

public:
	CSceneNode* GetOrCreateNode(uint32_t name);
	void DestroyNode(CSceneNode* pNode);

public:
	CComponentMeshPtr GetOrCreateComponentMesh(uint32_t name);
	CComponentSkinPtr GetOrCreateComponentSkin(uint32_t name);
	CComponentParticlePtr GetOrCreateComponentParticle(uint32_t name);
	CComponentPointLightPtr GetOrCreateComponentPointLight(uint32_t name);

public:
	void UpdateLogic(CTaskPool& taskPool, CTaskGraph& taskGraph, float totalTime, float deltaTime);
	void UpdateCamera(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, bool bComputeLOD);


private:
	eastl::unordered_map<uint32_t, CScene*> m_pScenes;
	eastl::unordered_map<uint32_t, CSceneNode*> m_pNodes;

	CComponentManager<CComponentMesh> m_meshManager;
	CComponentManager<CComponentSkin> m_skinManager;
	CComponentManager<CComponentParticle> m_particleManager;
	CComponentManager<CComponentPointLight> m_pointLightManager;
};
