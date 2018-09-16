#pragma once
#include <eastl/vector.h>
#include <eastl/unordered_map.h>
#include "Engine.h"
#include "Scene.h"
#include "SceneNode.h"
#include "ComponentMesh.h"
#include "ComponentSkin.h"
#include "ComponentParticle.h"


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

private:
	void Update(float deltaTime);
	void UpdateCamera(CGfxCamera *pCamera);
	void RenderCamera(CGfxCamera *pCamera);


private:
	CComponentManager<CComponentMesh> m_meshManager;
	CComponentManager<CComponentSkin> m_skinManager;
	CComponentManager<CComponentParticle> m_particleManager;

private:
	eastl::unordered_map<uint32_t, CScene*> m_pScenes;
	eastl::unordered_map<uint32_t, CSceneNode*> m_pNodes;

private:
	event_t m_eventUpdateSkin;
	event_t m_eventUpdateParticle;
	event_t m_eventUpdateCameraSkin;
	event_t m_eventUpdateCameraParticle;
	event_t m_eventCommandBuffer;

private:
	CTaskGraph m_taskGraphUpdate;
	CTaskGraph m_taskGraphRender;
};
