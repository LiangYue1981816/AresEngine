#include "Engine.h"
#include "SceneManager.h"
#include "TaskCommandBuffer.h"
#include "TaskComponentUpdate.h"
#include "TaskComponentUpdateCamera.h"


CSceneManager::CSceneManager(void)
	: m_taskGraphUpdateLogic("TashGraph_UpdateLogic", 75)
	, m_taskGraphUpdateCamera("TaskGraph_UpdateCamera", 75)
{
	event_init(&m_eventUpdateSkin, 1);
	event_init(&m_eventUpdateParticle, 1);
	event_init(&m_eventUpdatePointLight, 1);

	event_init(&m_eventUpdateCameraSkin, 1);
	event_init(&m_eventUpdateCameraParticle, 1);
	event_init(&m_eventUpdateCameraPointLight, 1);
}

CSceneManager::~CSceneManager(void)
{
	event_destroy(&m_eventUpdateSkin);
	event_destroy(&m_eventUpdateParticle);
	event_destroy(&m_eventUpdatePointLight);

	event_destroy(&m_eventUpdateCameraSkin);
	event_destroy(&m_eventUpdateCameraParticle);
	event_destroy(&m_eventUpdateCameraPointLight);
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

uint32_t CSceneManager::GetNextComponentPointLightName(void) const
{
	static uint32_t count = 0;
	static char szName[260] = { 0 };
	sprintf(szName, "_POINTLIGHT_%d", count++);
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
	Renderer()->SetShadowOrtho(left, right, bottom, top, zNear, zFar);
}

void CSceneManager::SetShadowLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	Renderer()->SetShadowLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}

void CSceneManager::SetShadowDistance(float distance)
{
	Renderer()->SetShadowDistance(distance);
}

void CSceneManager::SetShadowResolution(float resolution)
{
	Renderer()->SetShadowResolution(resolution);
}

void CSceneManager::SetLightFactor(float ambientLightFactor, float pointLightFactor, float directLightFactor, float envLightFactor)
{
	Renderer()->SetLightFactor(ambientLightFactor, pointLightFactor, directLightFactor, envLightFactor);
}

void CSceneManager::SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9])
{
	Renderer()->SetAmbientLightSH(shRed, shGreen, shBlue);
}

void CSceneManager::SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz)
{
	Renderer()->SetAmbientLightRotation(angle, axisx, axisy, axisz);
}

void CSceneManager::SetMainPointLightColor(float red, float green, float blue)
{
	Renderer()->SetPointLightColor(red, green, blue);
}

void CSceneManager::SetMainPointLightPosition(float posx, float posy, float posz, float radius)
{
	Renderer()->SetPointLightPosition(posx, posy, posz, radius);
}

void CSceneManager::SetMainPointLightAttenuation(float linear, float square, float constant)
{
	Renderer()->SetPointLightAttenuation(linear, square, constant);
}

void CSceneManager::SetMainDirectLightColor(float red, float green, float blue)
{
	Renderer()->SetDirectLightColor(red, green, blue);
}

void CSceneManager::SetMainDirectLightDirection(float dirx, float diry, float dirz)
{
	Renderer()->SetDirectLightDirection(dirx, diry, dirz);
}

void CSceneManager::SetFogColor(float red, float green, float blue)
{
	Renderer()->SetFogColor(red, green, blue);
}

void CSceneManager::SetFogHeightDensity(float startHeight, float endHeight, float density)
{
	Renderer()->SetFogHeightDensity(startHeight, endHeight, density);
}

void CSceneManager::SetFogDistanceDensity(float startDistance, float endDistance, float density)
{
	Renderer()->SetFogDistanceDensity(startDistance, endDistance, density);
}

void CSceneManager::Update(float totalTime, float deltaTime)
{
	for (const auto &itScene : m_pScenes) {
		itScene.second->GetRootNode()->UpdateTransform(false);
	}

	static CTaskComponentUpdate<CComponentMesh> taskUpdateMeshs[THREAD_COUNT];
	static CTaskComponentUpdate<CComponentSkin> taskUpdateSkins[THREAD_COUNT];
	static CTaskComponentUpdate<CComponentParticle> taskUpdateParticles[THREAD_COUNT];
	static CTaskComponentUpdate<CComponentPointLight> taskUpdatePointLights[THREAD_COUNT];

	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		taskUpdateMeshs[indexThread].SetParams(indexThread, &m_meshManager, totalTime, deltaTime);
		m_taskGraphUpdateLogic.Task(&taskUpdateMeshs[indexThread], NULL, &m_eventUpdateSkin, NULL);
	}

	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		taskUpdateSkins[indexThread].SetParams(indexThread, &m_skinManager, totalTime, deltaTime);
		m_taskGraphUpdateLogic.Task(&taskUpdateSkins[indexThread], NULL, &m_eventUpdateParticle, &m_eventUpdateSkin);
	}

	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		taskUpdateParticles[indexThread].SetParams(indexThread, &m_particleManager, totalTime, deltaTime);
		m_taskGraphUpdateLogic.Task(&taskUpdateParticles[indexThread], NULL, &m_eventUpdatePointLight, &m_eventUpdateParticle);
	}

	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		taskUpdatePointLights[indexThread].SetParams(indexThread, &m_pointLightManager, totalTime, deltaTime);
		m_taskGraphUpdateLogic.Task(&taskUpdatePointLights[indexThread], NULL, NULL, &m_eventUpdatePointLight);
	}

	m_taskGraphUpdateLogic.Dispatch();
	{
		Renderer()->SetTime(totalTime, deltaTime);
	}
	m_taskGraphUpdateLogic.Wait();
}

void CSceneManager::UpdateCamera(CGfxCamera *pCamera)
{
	if (pCamera == NULL) {
		return;
	}

	static CTaskComponentUpdateCamera<CComponentMesh> taskUpdateCameraMeshs[THREAD_COUNT];
	static CTaskComponentUpdateCamera<CComponentSkin> taskUpdateCameraSkins[THREAD_COUNT];
	static CTaskComponentUpdateCamera<CComponentParticle> taskUpdateCameraParticles[THREAD_COUNT];
	static CTaskComponentUpdateCamera<CComponentPointLight> taskUpdateCameraPointLights[THREAD_COUNT];

	pCamera->ClearQueue();

	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		taskUpdateCameraMeshs[indexThread].SetParams(indexThread, &m_meshManager, pCamera);
		m_taskGraphUpdateCamera.Task(&taskUpdateCameraMeshs[indexThread], NULL, &m_eventUpdateCameraSkin, NULL);
	}

	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		taskUpdateCameraSkins[indexThread].SetParams(indexThread, &m_skinManager, pCamera);
		m_taskGraphUpdateCamera.Task(&taskUpdateCameraSkins[indexThread], NULL, &m_eventUpdateCameraParticle, &m_eventUpdateCameraSkin);
	}

	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		taskUpdateCameraParticles[indexThread].SetParams(indexThread, &m_particleManager, pCamera);
		m_taskGraphUpdateCamera.Task(&taskUpdateCameraParticles[indexThread], NULL, &m_eventUpdateCameraPointLight, &m_eventUpdateCameraParticle);
	}

	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		taskUpdateCameraPointLights[indexThread].SetParams(indexThread, &m_pointLightManager, pCamera);
		m_taskGraphUpdateCamera.Task(&taskUpdateCameraPointLights[indexThread], NULL, NULL, &m_eventUpdateCameraPointLight);
	}

	m_taskGraphUpdateCamera.Dispatch();
	{
		// ...
	}
	m_taskGraphUpdateCamera.Wait();
}

void CSceneManager::RenderCamera(CGfxCamera *pCamera)
{
	if (pCamera == NULL) {
		return;
	}

	pCamera->Submit();
}
