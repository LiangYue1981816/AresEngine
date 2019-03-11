#include "SceneHeader.h"
#include "EngineHeader.h"

#include "TaskComponentUpdateLogic.h"
#include "TaskComponentUpdateCamera.h"


CSceneManager::CSceneManager(void)
	: m_taskGraph("TashGraph_SceneManager")
{

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
	eastl::vector<CTaskComponentUpdateLogic<CComponentMesh>> taskUpdateLogicMeshs(m_taskGraph.GetNumThreads());
	eastl::vector<CTaskComponentUpdateLogic<CComponentSkin>> taskUpdateLogicSkins(m_taskGraph.GetNumThreads());
	eastl::vector<CTaskComponentUpdateLogic<CComponentParticle>> taskUpdateLogicParticles(m_taskGraph.GetNumThreads());
	eastl::vector<CTaskComponentUpdateLogic<CComponentPointLight>> taskUpdateLogicPointLights(m_taskGraph.GetNumThreads());

	for (int indexThread = 0; indexThread < m_taskGraph.GetNumThreads(); indexThread++) {
		taskUpdateLogicMeshs[indexThread].SetParams(&m_meshManager, m_taskGraph.GetNumThreads(), indexThread, totalTime, deltaTime);
		taskUpdateLogicSkins[indexThread].SetParams(&m_skinManager, m_taskGraph.GetNumThreads(), indexThread, totalTime, deltaTime);
		taskUpdateLogicParticles[indexThread].SetParams(&m_particleManager, m_taskGraph.GetNumThreads(), indexThread, totalTime, deltaTime);

		m_taskGraph.Task(&taskUpdateLogicMeshs[indexThread], nullptr, nullptr, nullptr);
		m_taskGraph.Task(&taskUpdateLogicSkins[indexThread], nullptr, nullptr, nullptr);
		m_taskGraph.Task(&taskUpdateLogicParticles[indexThread], nullptr, nullptr, nullptr);
	}

	m_taskGraph.Dispatch();
	m_taskGraph.Wait();
}

void CSceneManager::UpdateCamera(CGfxCamera *pCamera, int indexQueue)
{
	if (pCamera) {
		pCamera->Begin(indexQueue);
		{
			eastl::vector<CTaskComponentUpdateCamera<CComponentMesh>> taskUpdateCameraMeshs(m_taskGraph.GetNumThreads());
			eastl::vector<CTaskComponentUpdateCamera<CComponentSkin>> taskUpdateCameraSkins(m_taskGraph.GetNumThreads());
			eastl::vector<CTaskComponentUpdateCamera<CComponentParticle>> taskUpdateCameraParticles(m_taskGraph.GetNumThreads());
			eastl::vector<CTaskComponentUpdateCamera<CComponentPointLight>> taskUpdateCameraPointLights(m_taskGraph.GetNumThreads());

			for (int indexThread = 0; indexThread < m_taskGraph.GetNumThreads(); indexThread++) {
				taskUpdateCameraMeshs[indexThread].SetParams(&m_meshManager, m_taskGraph.GetNumThreads(), indexThread, pCamera, indexQueue);
				taskUpdateCameraSkins[indexThread].SetParams(&m_skinManager, m_taskGraph.GetNumThreads(), indexThread, pCamera, indexQueue);
				taskUpdateCameraParticles[indexThread].SetParams(&m_particleManager, m_taskGraph.GetNumThreads(), indexThread, pCamera, indexQueue);
				taskUpdateCameraPointLights[indexThread].SetParams(&m_pointLightManager, m_taskGraph.GetNumThreads(), indexThread, pCamera, indexQueue);

				m_taskGraph.Task(&taskUpdateCameraMeshs[indexThread], nullptr, nullptr, nullptr);
				m_taskGraph.Task(&taskUpdateCameraSkins[indexThread], nullptr, nullptr, nullptr);
				m_taskGraph.Task(&taskUpdateCameraParticles[indexThread], nullptr, nullptr, nullptr);
				m_taskGraph.Task(&taskUpdateCameraPointLights[indexThread], nullptr, nullptr, nullptr);
			}

			m_taskGraph.Dispatch();
			m_taskGraph.Wait();
		}
		pCamera->End(indexQueue);
	}
}
