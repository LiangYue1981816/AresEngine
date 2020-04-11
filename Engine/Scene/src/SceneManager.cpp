#include "SceneHeader.h"

#include "TaskComponentUpdateLogic.h"
#include "TaskComponentUpdateCamera.h"


CSceneManager::CSceneManager(void)
{
	m_skinManager.Reserve(512);
	m_meshManager.Reserve(8192);
	m_particleManager.Reserve(2048);
	m_pointLightManager.Reserve(4096);
}

CSceneManager::~CSceneManager(void)
{
	for (const auto& itScene : m_pScenes) {
		delete itScene.second;
	}

	for (const auto& itNode : m_pNodes) {
		delete itNode.second;
	}

	m_pScenes.clear();
	m_pNodes.clear();
}

uint32_t CSceneManager::GetNextNodeName(void) const
{
	static uint32_t count = 0;
	return HashValueFormat("_NODE_%d", count++);
}

uint32_t CSceneManager::GetNextComponentMeshName(void) const
{
	static uint32_t count = 0;
	return HashValueFormat("_MESH_%d", count++);
}

uint32_t CSceneManager::GetNextComponentSkinName(void) const
{
	static uint32_t count = 0;
	return HashValueFormat("_SKIN_%d", count++);
}

uint32_t CSceneManager::GetNextComponentParticleName(void) const
{
	static uint32_t count = 0;
	return HashValueFormat("_PARTICLE_%d", count++);
}

uint32_t CSceneManager::GetNextComponentPointLightName(void) const
{
	static uint32_t count = 0;
	return HashValueFormat("_POINTLIGHT_%d", count++);
}

CScene* CSceneManager::GetOrCreateScene(uint32_t name)
{
	if (m_pScenes[name] == nullptr) {
		m_pScenes[name] = new CScene(name, this);
	}

	return m_pScenes[name];
}

void CSceneManager::DestroyScene(CScene* pScene)
{
	if (pScene) {
		m_pScenes.erase(pScene->GetName());
		delete pScene;
	}
}

CSceneNode* CSceneManager::GetOrCreateNode(uint32_t name)
{
	if (m_pNodes[name] == nullptr) {
		m_pNodes[name] = new CSceneNode(name, this);
	}

	return m_pNodes[name];
}

void CSceneManager::DestroyNode(CSceneNode* pNode)
{
	if (pNode) {
		m_pNodes.erase(pNode->GetName());
		delete pNode;
	}
}

CComponentMeshPtr CSceneManager::GetOrCreateComponentMesh(uint32_t name)
{
	if (m_meshManager.IsExist(name) == false) {
		m_meshManager.NewComponent(name, CComponentMesh(name));
	}

	return CComponentMeshPtr(name, &m_meshManager);
}

CComponentSkinPtr CSceneManager::GetOrCreateComponentSkin(uint32_t name)
{
	if (m_skinManager.IsExist(name) == false) {
		m_skinManager.NewComponent(name, CComponentSkin(name));
	}

	return CComponentSkinPtr(name, &m_skinManager);
}

CComponentParticlePtr CSceneManager::GetOrCreateComponentParticle(uint32_t name)
{
	if (m_particleManager.IsExist(name) == false) {
		m_particleManager.NewComponent(name, CComponentParticle(name));
	}

	return CComponentParticlePtr(name, &m_particleManager);
}

CComponentPointLightPtr CSceneManager::GetOrCreateComponentPointLight(uint32_t name)
{
	if (m_pointLightManager.IsExist(name) == false) {
		m_pointLightManager.NewComponent(name, CComponentPointLight(name));
	}

	return CComponentPointLightPtr(name, &m_pointLightManager);
}

void CSceneManager::UpdateLogic(CTaskPool& taskPool, CTaskGraph& taskGraph, float totalTime, float deltaTime)
{
	eastl::vector<CTaskComponentUpdateLogic<CComponentMesh>> taskUpdateLogicMeshs(taskPool.GetNumThreads());
	eastl::vector<CTaskComponentUpdateLogic<CComponentSkin>> taskUpdateLogicSkins(taskPool.GetNumThreads());
	eastl::vector<CTaskComponentUpdateLogic<CComponentParticle>> taskUpdateLogicParticles(taskPool.GetNumThreads());
	eastl::vector<CTaskComponentUpdateLogic<CComponentPointLight>> taskUpdateLogicPointLights(taskPool.GetNumThreads());

	for (int indexThread = 0; indexThread < taskPool.GetNumThreads(); indexThread++) {
		taskUpdateLogicMeshs[indexThread].SetParams(&m_meshManager, taskPool.GetNumThreads(), indexThread, totalTime, deltaTime);
		taskUpdateLogicSkins[indexThread].SetParams(&m_skinManager, taskPool.GetNumThreads(), indexThread, totalTime, deltaTime);
		taskUpdateLogicParticles[indexThread].SetParams(&m_particleManager, taskPool.GetNumThreads(), indexThread, totalTime, deltaTime);
		taskUpdateLogicPointLights[indexThread].SetParams(&m_pointLightManager, taskPool.GetNumThreads(), indexThread, totalTime, deltaTime);

		taskPool.Task(&taskUpdateLogicMeshs[indexThread], nullptr, nullptr);
		taskPool.Task(&taskUpdateLogicSkins[indexThread], nullptr, nullptr);
		taskPool.Task(&taskUpdateLogicParticles[indexThread], nullptr, nullptr);
		taskPool.Task(&taskUpdateLogicPointLights[indexThread], nullptr, nullptr);
	}

	taskPool.Dispatch();
	taskPool.Wait();
}

void CSceneManager::UpdateCamera(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, bool bComputeLOD)
{
	pRenderQueue->Begin(pCamera);
	{
		eastl::vector<CTaskComponentUpdateCamera<CComponentMesh>> taskUpdateCameraMeshs(taskPool.GetNumThreads());
		eastl::vector<CTaskComponentUpdateCamera<CComponentSkin>> taskUpdateCameraSkins(taskPool.GetNumThreads());
		eastl::vector<CTaskComponentUpdateCamera<CComponentParticle>> taskUpdateCameraParticles(taskPool.GetNumThreads());
		eastl::vector<CTaskComponentUpdateCamera<CComponentPointLight>> taskUpdateCameraPointLights(taskPool.GetNumThreads());

		for (int indexThread = 0; indexThread < taskPool.GetNumThreads(); indexThread++) {
			taskUpdateCameraMeshs[indexThread].SetParams(&m_meshManager, taskPool.GetNumThreads(), indexThread, pCamera, pRenderQueue, mask, bComputeLOD);
			taskUpdateCameraSkins[indexThread].SetParams(&m_skinManager, taskPool.GetNumThreads(), indexThread, pCamera, pRenderQueue, mask, bComputeLOD);
			taskUpdateCameraParticles[indexThread].SetParams(&m_particleManager, taskPool.GetNumThreads(), indexThread, pCamera, pRenderQueue, mask, bComputeLOD);
			taskUpdateCameraPointLights[indexThread].SetParams(&m_pointLightManager, taskPool.GetNumThreads(), indexThread, pCamera, pRenderQueue, mask, bComputeLOD);

			taskPool.Task(&taskUpdateCameraMeshs[indexThread], nullptr, nullptr);
			taskPool.Task(&taskUpdateCameraSkins[indexThread], nullptr, nullptr);
			taskPool.Task(&taskUpdateCameraParticles[indexThread], nullptr, nullptr);
			taskPool.Task(&taskUpdateCameraPointLights[indexThread], nullptr, nullptr);
		}

		taskPool.Dispatch();
		taskPool.Wait();
	}
	pRenderQueue->End();
}
