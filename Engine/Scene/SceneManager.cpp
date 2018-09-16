#include "Engine.h"
#include "SceneManager.h"
#include "TaskCommandBuffer.h"
#include "TaskComponentUpdate.h"
#include "TaskComponentUpdateCamera.h"


CSceneManager::CSceneManager(void)
{
	event_init(&m_eventUpdateSkin, 1);
	event_init(&m_eventUpdateParticle, 1);
	event_init(&m_eventUpdateCameraSkin, 1);
	event_init(&m_eventUpdateCameraParticle, 1);
	event_init(&m_eventCommandBuffer, 1);
}

CSceneManager::~CSceneManager(void)
{
	event_destroy(&m_eventUpdateSkin);
	event_destroy(&m_eventUpdateParticle);
	event_destroy(&m_eventUpdateCameraSkin);
	event_destroy(&m_eventUpdateCameraParticle);
	event_destroy(&m_eventCommandBuffer);
}

uint32_t CSceneManager::GetNextNodeName(void) const
{
	static uint32_t count = 0;
	static char szName[260] = { 0 };
	sprintf(szName, "_NODE_%d", count++);
	return HashValue(szName);
}

uint32_t CSceneManager::GetNextComponentMeshName(void) const
{
	static uint32_t count = 0;
	static char szName[260] = { 0 };
	sprintf(szName, "_MESH_%d", count++);
	return HashValue(szName);
}

uint32_t CSceneManager::GetNextComponentSkinName(void) const
{
	static uint32_t count = 0;
	static char szName[260] = { 0 };
	sprintf(szName, "_SKIN_%d", count++);
	return HashValue(szName);
}

uint32_t CSceneManager::GetNextComponentParticleName(void) const
{
	static uint32_t count = 0;
	static char szName[260] = { 0 };
	sprintf(szName, "_PARTICLE_%d", count++);
	return HashValue(szName);
}

CScene* CSceneManager::CreateScene(uint32_t name)
{
	if (m_pScenes[name] == NULL) {
		m_pScenes[name] = new CScene(name);
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
	if (m_pNodes[name] == NULL) {
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

void CSceneManager::Update(float deltaTime)
{
	for (const auto &itScene : m_pScenes) {
		itScene.second->GetRootNode()->UpdateTransform(false);
	}

	static CTaskComponentUpdate<CComponentMesh> taskUpdateMeshs[THREAD_COUNT];
	static CTaskComponentUpdate<CComponentSkin> taskUpdateSkins[THREAD_COUNT];
	static CTaskComponentUpdate<CComponentParticle> taskUpdateParticles[THREAD_COUNT];

	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		taskUpdateMeshs[indexThread].SetParams(indexThread, &m_meshManager, deltaTime);
		m_taskGraphUpdate.Task(&taskUpdateMeshs[indexThread], NULL, &m_eventUpdateSkin, NULL);
	}

	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		taskUpdateSkins[indexThread].SetParams(indexThread, &m_skinManager, deltaTime);
		m_taskGraphUpdate.Task(&taskUpdateSkins[indexThread], NULL, &m_eventUpdateParticle, &m_eventUpdateSkin);
	}

	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		taskUpdateParticles[indexThread].SetParams(indexThread, &m_particleManager, deltaTime);
		m_taskGraphUpdate.Task(&taskUpdateParticles[indexThread], NULL, NULL, &m_eventUpdateParticle);
	}

	m_taskGraphUpdate.Dispatch();
	m_taskGraphUpdate.Wait();
}

void CSceneManager::UpdateCamera(CGfxCamera *pCamera)
{
	if (pCamera == NULL) {
		return;
	}

	static CTaskComponentUpdateCamera<CComponentMesh> taskUpdateCameraMeshs[THREAD_COUNT];
	static CTaskComponentUpdateCamera<CComponentSkin> taskUpdateCameraSkins[THREAD_COUNT];
	static CTaskComponentUpdateCamera<CComponentParticle> taskUpdateCameraParticles[THREAD_COUNT];
	static CTaskCommandBuffer taskCommandBuffer;

	m_taskGraphRender.Wait();
	{
		for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			taskUpdateCameraMeshs[indexThread].SetParams(indexThread, &m_meshManager, pCamera);
			m_taskGraphRender.Task(&taskUpdateCameraMeshs[indexThread], NULL, &m_eventUpdateCameraSkin, NULL);
		}

		for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			taskUpdateCameraSkins[indexThread].SetParams(indexThread, &m_skinManager, pCamera);
			m_taskGraphRender.Task(&taskUpdateCameraSkins[indexThread], NULL, &m_eventUpdateCameraParticle, &m_eventUpdateCameraSkin);
		}

		for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			taskUpdateCameraParticles[indexThread].SetParams(indexThread, &m_particleManager, pCamera);
			m_taskGraphRender.Task(&taskUpdateCameraParticles[indexThread], NULL, &m_eventCommandBuffer, &m_eventUpdateCameraParticle);
		}

		m_taskGraphRender.Task(&taskCommandBuffer, pCamera, NULL, &m_eventCommandBuffer);
	}
	m_taskGraphRender.Dispatch();
}

void CSceneManager::RenderCamera(CGfxCamera *pCamera)
{
	if (pCamera == NULL) {
		return;
	}

	pCamera->Submit();
}
