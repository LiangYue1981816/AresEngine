#include "SceneHeader.h"
#include "TaskComponentUpdateLogic.h"
#include "TaskComponentUpdateCamera.h"


CSceneManager::CSceneManager(void)
	: m_taskGraphUpdateLogic("TashGraph_UpdateLogic")
	, m_taskGraphUpdateCamera("TaskGraph_UpdateCamera")
{
	event_init(&m_eventUpdateLogicSkin, 1);
	event_init(&m_eventUpdateLogicParticle, 1);
	event_init(&m_eventUpdateLogicPointLight, 1);

	event_init(&m_eventUpdateCameraSkin, 1);
	event_init(&m_eventUpdateCameraParticle, 1);
	event_init(&m_eventUpdateCameraPointLight, 1);
}

CSceneManager::~CSceneManager(void)
{
	for (const auto &itScene : m_pScenes) {
		delete itScene.second;
	}

	for (const auto &itNode : m_pNodes) {
		delete itNode.second;
	}

	m_pScenes.clear();
	m_pNodes.clear();

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

CScene* CSceneManager::CreateScene(uint32_t name)
{
	if (m_pScenes[name] == nullptr) {
		m_pScenes[name] = new CScene(name, this);
	}

	return m_pScenes[name];
}

void CSceneManager::DestroyScene(CScene *pScene)
{
	if (pScene) {
		m_pScenes.erase(pScene->GetName());
		delete pScene;
	}
}

CSceneNode* CSceneManager::CreateNode(uint32_t name)
{
	if (m_pNodes[name] == nullptr) {
		m_pNodes[name] = new CSceneNode(name, this);
	}

	return m_pNodes[name];
}

void CSceneManager::DestroyNode(CSceneNode *pNode)
{
	if (pNode) {
		m_pNodes.erase(pNode->GetName());
		delete pNode;
	}
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

void CSceneManager::UpdateLogic(float totalTime, float deltaTime)
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

void CSceneManager::UpdateCamera(CGfxCamera *pCamera, int indexQueue)
{
	if (pCamera) {
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
}
