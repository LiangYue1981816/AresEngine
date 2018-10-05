#pragma once
#include <eastl/vector.h>
#include <eastl/unordered_map.h>
#include "Engine.h"
#include "Scene.h"
#include "SceneNode.h"
#include "ComponentMesh.h"
#include "ComponentSkin.h"
#include "ComponentParticle.h"
#include "ComponentPointLight.h"


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
	CGfxCamera* GetMainCamera(void) const;
	CGfxCamera* GetShadowCamera(void) const;

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

public:
	void SetShadowOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	void SetShadowLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);
	void SetShadowDistance(float distance);
	void SetShadowResolution(float resolution);

	void SetLightFactor(float ambientLightFactor, float pointLightFactor, float directLightFactor, float envLightFactor);

	void SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9]);
	void SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz);

	void SetMainPointLightColor(float red, float green, float blue);
	void SetMainPointLightPosition(float posx, float posy, float posz, float radius);
	void SetMainPointLightAttenuation(float linear, float square, float constant);

	void SetMainDirectLightColor(float red, float green, float blue);
	void SetMainDirectLightDirection(float dirx, float diry, float dirz);

	void SetFogColor(float red, float green, float blue);
	void SetFogHeightDensity(float startHeight, float endHeight, float density);
	void SetFogDistanceDensity(float startDistance, float endDistance, float density);

private:
	void UpdateLogic(float totalTime, float deltaTime);
	void UpdateCamera(CGfxCamera *pCamera);


private:
	CComponentManager<CComponentMesh> m_meshManager;
	CComponentManager<CComponentSkin> m_skinManager;
	CComponentManager<CComponentParticle> m_particleManager;
	CComponentManager<CComponentPointLight> m_pointLightManager;

private:
	CGfxCamera *m_pMainCamera;
	CGfxCamera *m_pShadowCamera;

	eastl::unordered_map<uint32_t, CScene*> m_pScenes;
	eastl::unordered_map<uint32_t, CSceneNode*> m_pNodes;

private:
	event_t m_eventUpdateLogicSkin;
	event_t m_eventUpdateLogicParticle;
	event_t m_eventUpdateLogicPointLight;

	event_t m_eventUpdateCameraSkin;
	event_t m_eventUpdateCameraParticle;
	event_t m_eventUpdateCameraPointLight;

private:
	CTaskGraph m_taskGraphUpdateLogic;
	CTaskGraph m_taskGraphUpdateCamera;
};
