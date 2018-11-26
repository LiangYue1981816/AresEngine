#pragma once
#include "PreHeader.h"

#include "SceneDefinition.h"
#include "Scene.h"
#include "SceneNode.h"

#include "ComponentMesh.h"
#include "ComponentSkin.h"
#include "ComponentParticle.h"
#include "ComponentPointLight.h"


class CALL_API CSceneManager
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
	CScene* CreateScene(uint32_t name);
	void DestroyScene(CScene *pScene);

public:
	CSceneNode* CreateNode(uint32_t name);
	void DestroyNode(CSceneNode *pNode);

public:
	CComponentMeshPtr CreateComponentMesh(uint32_t name);
	CComponentSkinPtr CreateComponentSkin(uint32_t name);
	CComponentParticlePtr CreateComponentParticle(uint32_t name);
	CComponentPointLightPtr CreateComponentPointLight(uint32_t name);

private:
	void UpdateLogic(float totalTime, float deltaTime);
	void UpdateCamera(CGfxCamera *pCamera, int indexQueue);


private:
	eastl::unordered_map<uint32_t, CScene*> m_pScenes;
	eastl::unordered_map<uint32_t, CSceneNode*> m_pNodes;

	CComponentManager<CComponentMesh> m_meshManager;
	CComponentManager<CComponentSkin> m_skinManager;
	CComponentManager<CComponentParticle> m_particleManager;
	CComponentManager<CComponentPointLight> m_pointLightManager;

private:
	event_t m_eventUpdateLogicSkin;
	event_t m_eventUpdateLogicParticle;
	event_t m_eventUpdateLogicPointLight;
	CTaskGraph m_taskGraphUpdateLogic;

	event_t m_eventUpdateCameraSkin;
	event_t m_eventUpdateCameraParticle;
	event_t m_eventUpdateCameraPointLight;
	CTaskGraph m_taskGraphUpdateCamera;
};
