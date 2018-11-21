#include "SceneHeader.h"
#include "TaskComponentUpdateLogic.h"
#include "TaskComponentUpdateCamera.h"


CSceneManager::CSceneManager(void)
	: m_pMainCamera(nullptr)
	, m_pShadowCamera(nullptr)

	, m_taskGraphUpdateLogic("TashGraph_UpdateLogic")
	, m_taskGraphUpdateCamera("TaskGraph_UpdateCamera")
{
	m_pMainCamera = new CGfxCamera;
	m_pShadowCamera = new CGfxCamera;
	m_ptrUniformEngine = Renderer()->NewUniformEngine(true);

	event_init(&m_eventUpdateLogicSkin, 1);
	event_init(&m_eventUpdateLogicParticle, 1);
	event_init(&m_eventUpdateLogicPointLight, 1);

	event_init(&m_eventUpdateCameraSkin, 1);
	event_init(&m_eventUpdateCameraParticle, 1);
	event_init(&m_eventUpdateCameraPointLight, 1);
}

CSceneManager::~CSceneManager(void)
{
	delete m_pMainCamera;
	delete m_pShadowCamera;

	event_destroy(&m_eventUpdateLogicSkin);
	event_destroy(&m_eventUpdateLogicParticle);
	event_destroy(&m_eventUpdateLogicPointLight);

	event_destroy(&m_eventUpdateCameraSkin);
	event_destroy(&m_eventUpdateCameraParticle);
	event_destroy(&m_eventUpdateCameraPointLight);
}

uint32_t CSceneManager::GetNextNodeName(void) const
{
	static uint32_t count = 0;
	static char szName[_MAX_STRING] = { 0 };
	sprintf(szName, "_NODE_%d", count++);
	return HashValue(szName);
}

uint32_t CSceneManager::GetNextComponentMeshName(void) const
{
	static uint32_t count = 0;
	static char szName[_MAX_STRING] = { 0 };
	sprintf(szName, "_MESH_%d", count++);
	return HashValue(szName);
}

uint32_t CSceneManager::GetNextComponentSkinName(void) const
{
	static uint32_t count = 0;
	static char szName[_MAX_STRING] = { 0 };
	sprintf(szName, "_SKIN_%d", count++);
	return HashValue(szName);
}

uint32_t CSceneManager::GetNextComponentParticleName(void) const
{
	static uint32_t count = 0;
	static char szName[_MAX_STRING] = { 0 };
	sprintf(szName, "_PARTICLE_%d", count++);
	return HashValue(szName);
}

uint32_t CSceneManager::GetNextComponentPointLightName(void) const
{
	static uint32_t count = 0;
	static char szName[_MAX_STRING] = { 0 };
	sprintf(szName, "_POINTLIGHT_%d", count++);
	return HashValue(szName);
}

CGfxCamera* CSceneManager::GetMainCamera(void) const
{
	return m_pMainCamera;
}

CGfxCamera* CSceneManager::GetShadowCamera(void) const
{
	return m_pShadowCamera;
}

CScene* CSceneManager::CreateScene(uint32_t name)
{
	if (m_pScenes[name] == nullptr) {
		m_pScenes[name] = new CScene(name, this);
	}

	return m_pScenes[name];
}

void CSceneManager::DestroyScene(CScene *pScene)
{
	m_pScenes.erase(pScene->GetName());
	delete pScene;
}

CSceneNode* CSceneManager::CreateNode(uint32_t name)
{
	if (m_pNodes[name] == nullptr) {
		m_pNodes[name] = new CSceneNode(name);
	}

	return m_pNodes[name];
}

void CSceneManager::DestroyNode(CSceneNode *pNode)
{
	m_pNodes.erase(pNode->GetName());
	delete pNode;
}

CComponentMeshPtr CSceneManager::CreateComponentMesh(uint32_t name)
{
	if (m_meshManager.NewComponent(name, CComponentMesh(name))) {
		return CComponentMeshPtr(name, &m_meshManager);
	}
	else {
		return CComponentMeshPtr();
	}
}

CComponentSkinPtr CSceneManager::CreateComponentSkin(uint32_t name)
{
	if (m_skinManager.NewComponent(name, CComponentSkin(name))) {
		return CComponentSkinPtr(name, &m_skinManager);
	}
	else {
		return CComponentSkinPtr();
	}
}

CComponentParticlePtr CSceneManager::CreateComponentParticle(uint32_t name)
{
	if (m_particleManager.NewComponent(name, CComponentParticle(name))) {
		return CComponentParticlePtr(name, &m_particleManager);
	}
	else {
		return CComponentParticlePtr();
	}
}

CComponentPointLightPtr CSceneManager::CreateComponentPointLight(uint32_t name)
{
	if (m_pointLightManager.NewComponent(name, CComponentPointLight(name))) {
		return CComponentPointLightPtr(name, &m_pointLightManager);
	}
	else {
		return CComponentPointLightPtr();
	}
}

void CSceneManager::SetShadowOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_ptrUniformEngine->SetShadowOrtho(left, right, bottom, top, zNear, zFar);
}

void CSceneManager::SetShadowLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	m_ptrUniformEngine->SetShadowLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}

void CSceneManager::SetShadowRange(float range)
{
	m_ptrUniformEngine->SetShadowRange(range);
}

void CSceneManager::SetShadowResolution(float resolution)
{
	m_ptrUniformEngine->SetShadowResolution(resolution);
}

void CSceneManager::SetLightFactor(float ambientLightFactor, float pointLightFactor, float directLightFactor, float envLightFactor)
{
	m_ptrUniformEngine->SetLightFactor(ambientLightFactor, pointLightFactor, directLightFactor, envLightFactor);
}

void CSceneManager::SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9])
{
	m_ptrUniformEngine->SetAmbientLightSH(shRed, shGreen, shBlue);
}

void CSceneManager::SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz)
{
	m_ptrUniformEngine->SetAmbientLightRotation(angle, axisx, axisy, axisz);
}

void CSceneManager::SetMainPointLightColor(float red, float green, float blue)
{
	m_ptrUniformEngine->SetPointLightColor(red, green, blue);
}

void CSceneManager::SetMainPointLightPosition(float posx, float posy, float posz, float radius)
{
	m_ptrUniformEngine->SetPointLightPosition(posx, posy, posz, radius);
}

void CSceneManager::SetMainPointLightAttenuation(float linear, float square, float constant)
{
	m_ptrUniformEngine->SetPointLightAttenuation(linear, square, constant);
}

void CSceneManager::SetMainDirectLightColor(float red, float green, float blue)
{
	m_ptrUniformEngine->SetDirectLightColor(red, green, blue);
}

void CSceneManager::SetMainDirectLightDirection(float dirx, float diry, float dirz)
{
	m_ptrUniformEngine->SetDirectLightDirection(dirx, diry, dirz);
}

void CSceneManager::SetFogColor(float red, float green, float blue)
{
	m_ptrUniformEngine->SetFogColor(red, green, blue);
}

void CSceneManager::SetFogHeightDensity(float startHeight, float endHeight, float density)
{
	m_ptrUniformEngine->SetFogHeightDensity(startHeight, endHeight, density);
}

void CSceneManager::SetFogDistanceDensity(float startDistance, float endDistance, float density)
{
	m_ptrUniformEngine->SetFogDistanceDensity(startDistance, endDistance, density);
}

const CGfxUniformEnginePtr& CSceneManager::GetUniformEngine(void) const
{
	return m_ptrUniformEngine;
}

void CSceneManager::UpdateLogic(float totalTime, float deltaTime)
{
	for (const auto &itScene : m_pScenes) {
		itScene.second->GetRootNode()->UpdateTransform(false);
	}

	{
		CTaskComponentUpdateLogic<CComponentMesh> taskUpdateLogicMeshs[THREAD_COUNT];
		CTaskComponentUpdateLogic<CComponentSkin> taskUpdateLogicSkins[THREAD_COUNT];
		CTaskComponentUpdateLogic<CComponentParticle> taskUpdateLogicParticles[THREAD_COUNT];
		CTaskComponentUpdateLogic<CComponentPointLight> taskUpdateLogicPointLights[THREAD_COUNT];

		for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			taskUpdateLogicMeshs[indexThread].SetParams(indexThread, &m_meshManager, totalTime, deltaTime);
			m_taskGraphUpdateLogic.Task(&taskUpdateLogicMeshs[indexThread], nullptr, &m_eventUpdateLogicSkin, nullptr);
		}

		for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			taskUpdateLogicSkins[indexThread].SetParams(indexThread, &m_skinManager, totalTime, deltaTime);
			m_taskGraphUpdateLogic.Task(&taskUpdateLogicSkins[indexThread], nullptr, &m_eventUpdateLogicParticle, &m_eventUpdateLogicSkin);
		}

		for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			taskUpdateLogicParticles[indexThread].SetParams(indexThread, &m_particleManager, totalTime, deltaTime);
			m_taskGraphUpdateLogic.Task(&taskUpdateLogicParticles[indexThread], nullptr, &m_eventUpdateLogicPointLight, &m_eventUpdateLogicParticle);
		}

		for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			taskUpdateLogicPointLights[indexThread].SetParams(indexThread, &m_pointLightManager, totalTime, deltaTime);
			m_taskGraphUpdateLogic.Task(&taskUpdateLogicPointLights[indexThread], nullptr, nullptr, &m_eventUpdateLogicPointLight);
		}

		m_taskGraphUpdateLogic.Dispatch();
		m_taskGraphUpdateLogic.Wait();
	}
}

void CSceneManager::UpdateCamera(CGfxCamera *pCamera, int indexQueue)
{
	if (pCamera == nullptr) {
		return;
	}

	pCamera->Begin(indexQueue);
	{
		CTaskComponentUpdateCamera<CComponentMesh> taskUpdateCameraMeshs[THREAD_COUNT];
		CTaskComponentUpdateCamera<CComponentSkin> taskUpdateCameraSkins[THREAD_COUNT];
		CTaskComponentUpdateCamera<CComponentParticle> taskUpdateCameraParticles[THREAD_COUNT];
		CTaskComponentUpdateCamera<CComponentPointLight> taskUpdateCameraPointLights[THREAD_COUNT];

		for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			taskUpdateCameraMeshs[indexThread].SetParams(indexThread, indexQueue, &m_meshManager, pCamera);
			m_taskGraphUpdateCamera.Task(&taskUpdateCameraMeshs[indexThread], nullptr, &m_eventUpdateCameraSkin, nullptr);
		}

		for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			taskUpdateCameraSkins[indexThread].SetParams(indexThread, indexQueue, &m_skinManager, pCamera);
			m_taskGraphUpdateCamera.Task(&taskUpdateCameraSkins[indexThread], nullptr, &m_eventUpdateCameraParticle, &m_eventUpdateCameraSkin);
		}

		for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			taskUpdateCameraParticles[indexThread].SetParams(indexThread, indexQueue, &m_particleManager, pCamera);
			m_taskGraphUpdateCamera.Task(&taskUpdateCameraParticles[indexThread], nullptr, &m_eventUpdateCameraPointLight, &m_eventUpdateCameraParticle);
		}

		for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			taskUpdateCameraPointLights[indexThread].SetParams(indexThread, indexQueue, &m_pointLightManager, pCamera);
			m_taskGraphUpdateCamera.Task(&taskUpdateCameraPointLights[indexThread], nullptr, nullptr, &m_eventUpdateCameraPointLight);
		}

		m_taskGraphUpdateCamera.Dispatch();
		m_taskGraphUpdateCamera.Wait();
	}
	pCamera->End(indexQueue);
}
