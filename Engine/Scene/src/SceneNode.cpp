#include "SceneHeader.h"


CSceneNode::CSceneNode(uint32_t name, CScene *pScene)
	: m_name(name)
	, m_pScene(pScene)

	, m_bActive(true)
	, m_bNeedUpdateTransform(false)

	, m_pParentNode(nullptr)
{
	Identity();
}

CSceneNode::CSceneNode(uint32_t name)
	: CSceneNode(name, nullptr)
{

}

CSceneNode::~CSceneNode(void)
{
	DetachNodeAll();
	DetachComponentAll();

	if (m_pParentNode) {
		m_pParentNode->DetachNode(this);
	}
}

uint32_t CSceneNode::GetName(void) const
{
	return m_name;
}

CSceneNode* CSceneNode::GetParent(void) const
{
	return m_pParentNode;
}

void CSceneNode::SetActive(bool bActive)
{
	m_bActive = bActive;
}

bool CSceneNode::IsActive(void) const
{
	return m_pParentNode ? m_bActive && m_pParentNode->IsActive() : m_bActive;
}

bool CSceneNode::AttachNode(CSceneNode *pNode)
{
	if (pNode == nullptr) {
		return false;
	}

	if (pNode == this) {
		return false;
	}

	if (pNode->m_pParentNode != nullptr) {
		return false;
	}

	if (pNode->m_pScene != nullptr) {
		return false;
	}

	if (m_pScene == nullptr || m_pScene->AttachNode(pNode) == false) {
		return false;
	}

	pNode->m_pScene = m_pScene;
	pNode->m_pParentNode = this;
	pNode->m_bNeedUpdateTransform = true;

	m_pChildNodes.insert(eastl::pair<uint32_t, CSceneNode*>(pNode->m_name, pNode));

	return true;
}

bool CSceneNode::DetachNode(CSceneNode *pNode)
{
	if (pNode == nullptr) {
		return false;
	}

	if (pNode == this) {
		return false;
	}

	if (pNode->m_pParentNode != this) {
		return false;
	}

	if (pNode->m_pScene != m_pScene) {
		return false;
	}

	if (m_pScene == nullptr || m_pScene->DetachNode(pNode) == false) {
		return false;
	}

	pNode->m_pScene = nullptr;
	pNode->m_pParentNode = nullptr;
	pNode->m_bNeedUpdateTransform = true;

	m_pChildNodes.erase(pNode->m_name);

	return true;
}

void CSceneNode::DetachNodeAll(void)
{
	for (const auto &itNode : m_pChildNodes) {
		if (m_pScene) {
			m_pScene->DetachNode(itNode.second);
		}

		itNode.second->m_pScene = nullptr;
		itNode.second->m_pParentNode = nullptr;
		itNode.second->m_bNeedUpdateTransform = true;
	}

	m_pChildNodes.clear();
}

CSceneNode* CSceneNode::GetNode(uint32_t name) const
{
	const auto &itNode = m_pChildNodes.find(name);
	return itNode != m_pChildNodes.end() ? itNode->second : nullptr;
}

bool CSceneNode::AttachComponentMesh(CComponentMeshPtr &ptrComponentMesh)
{
	return AttachComponent(ptrComponentMesh, m_ptrComponentMeshs);
}

bool CSceneNode::DetachComponentMesh(CComponentMeshPtr &ptrComponentMesh)
{
	return DetachComponent(ptrComponentMesh, m_ptrComponentMeshs);
}

void CSceneNode::DetachComponentMeshAll(void)
{
	DetachComponentAll(m_ptrComponentMeshs);
}

CComponentMeshPtr CSceneNode::GetComponentMesh(uint32_t name) const
{
	return GetComponent(name, m_ptrComponentMeshs);
}

bool CSceneNode::AttachComponentSkin(CComponentSkinPtr &ptrComponentSkin)
{
	return AttachComponent(ptrComponentSkin, m_ptrComponentSkins);
}

bool CSceneNode::DetachComponentSkin(CComponentSkinPtr &ptrComponentSkin)
{
	return DetachComponent(ptrComponentSkin, m_ptrComponentSkins);
}

void CSceneNode::DetachComponentSkinAll(void)
{
	DetachComponentAll(m_ptrComponentSkins);
}

CComponentSkinPtr CSceneNode::GetComponentSkin(uint32_t name) const
{
	return GetComponent(name, m_ptrComponentSkins);
}

bool CSceneNode::AttachComponentParticle(CComponentParticlePtr &ptrComponentParticle)
{
	return AttachComponent(ptrComponentParticle, m_ptrComponentParticles);
}

bool CSceneNode::DetachComponentParticle(CComponentParticlePtr &ptrComponentParticle)
{
	return DetachComponent(ptrComponentParticle, m_ptrComponentParticles);
}

void CSceneNode::DetachComponentParticleAll(void)
{
	DetachComponentAll(m_ptrComponentParticles);
}

CComponentParticlePtr CSceneNode::GetComponentParticle(uint32_t name) const
{
	return GetComponent(name, m_ptrComponentParticles);
}

bool CSceneNode::AttachComponentPointLight(CComponentPointLightPtr &ptrComponentPointLight)
{
	return AttachComponent(ptrComponentPointLight, m_ptrComponentPointLights);
}

bool CSceneNode::DetachComponentPointLight(CComponentPointLightPtr &ptrComponentPointLight)
{
	return DetachComponent(ptrComponentPointLight, m_ptrComponentPointLights);
}

void CSceneNode::DetachComponentPointLightAll(void)
{
	DetachComponentAll(m_ptrComponentPointLights);
}

CComponentPointLightPtr CSceneNode::GetComponentPointLight(uint32_t name) const
{
	return GetComponent(name, m_ptrComponentPointLights);
}

void CSceneNode::DetachComponentAll(void)
{
	DetachComponentMeshAll();
	DetachComponentSkinAll();
	DetachComponentParticleAll();
	DetachComponentPointLightAll();
}

template<class T>
bool CSceneNode::AttachComponent(CComponentPtr<T> &ptrComponent, eastl::unordered_map<uint32_t, CComponentPtr<T>> &container)
{
	if (ptrComponent.IsNull()) {
		return false;
	}

	if (ptrComponent->m_pParentNode != nullptr) {
		return false;
	}

	ptrComponent->m_pParentNode = this;
	container.insert(eastl::pair<uint32_t, CComponentPtr<T>>(ptrComponent->m_name, ptrComponent));

	return true;
}

template<class T>
bool CSceneNode::DetachComponent(CComponentPtr<T> &ptrComponent, eastl::unordered_map<uint32_t, CComponentPtr<T>> &container)
{
	if (ptrComponent.IsNull()) {
		return false;
	}

	if (ptrComponent->m_pParentNode != this) {
		return false;
	}

	ptrComponent->m_pParentNode = nullptr;
	container.erase(ptrComponent->m_name);

	return true;
}

template<class T>
void CSceneNode::DetachComponentAll(eastl::unordered_map<uint32_t, CComponentPtr<T>> &container)
{
	container.clear();
}

template<class T>
CComponentPtr<T> CSceneNode::GetComponent(uint32_t name, const eastl::unordered_map<uint32_t, CComponentPtr<T>> &container) const
{
	const auto &itComponent = container.find(name);
	return itComponent != container.end() ? itComponent->second : CComponentPtr<T>();
}

void CSceneNode::Identity(void)
{
	SetLocalScale(1.0f, 1.0f, 1.0f);
	SetLocalPosition(0.0f, 0.0f, 0.0f);
	SetLocalOrientation(0.0f, 0.0f, 0.0f, 1.0f);
}

void CSceneNode::SetLocalScale(float x, float y, float z)
{
	m_bNeedUpdateTransform = true;
	m_localScale = glm::vec3(x, y, z);
}

void CSceneNode::SetLocalPosition(float x, float y, float z)
{
	m_bNeedUpdateTransform = true;
	m_localPosition = glm::vec3(x, y, z);
}

void CSceneNode::SetLocalOrientation(float x, float y, float z, float w)
{
	m_bNeedUpdateTransform = true;
	m_localOrientation = glm::quat(w, x, y, z);
}

void CSceneNode::SetLocalDirection(float dirx, float diry, float dirz, float upx, float upy, float upz)
{
	m_bNeedUpdateTransform = true;
	m_localOrientation = glm::toQuat(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(dirx, diry, dirz), glm::vec3(upx, upy, upz)));
}

void CSceneNode::SetWorldScale(float x, float y, float z)
{
	if (m_pParentNode) {
		glm::vec3 worldScaleParent = m_pParentNode->GetWorldScale();
		glm::vec3 worldScale = glm::vec3(x, y, z);
		glm::vec3 localScale = worldScale / worldScaleParent;
		SetLocalScale(localScale.x, localScale.y, localScale.z);
	}
	else {
		SetLocalScale(x, y, z);
	}
}

void CSceneNode::SetWorldPosition(float x, float y, float z)
{
	if (m_pParentNode) {
		glm::mat4 worldTransformParent = m_pParentNode->GetWorldTransform();
		glm::mat4 worldTransformParentInv = glm::inverse(worldTransformParent);
		glm::vec4 worldPosition = glm::vec4(x, y, z, 1.0);
		glm::vec4 localPosition = worldTransformParentInv * worldPosition;
		SetLocalPosition(localPosition.x, localPosition.y, localPosition.z);
	}
	else {
		SetLocalPosition(x, y, z);
	}
}

void CSceneNode::SetWorldOrientation(float x, float y, float z, float w)
{
	if (m_pParentNode) {
		glm::quat worldOrientationParent = m_pParentNode->GetWorldOrientation();
		glm::quat worldOrientationParentInv = glm::inverse(worldOrientationParent);
		glm::quat worldOrientation = glm::quat(w, x, y, z);
		glm::quat localOrientation = worldOrientationParentInv * worldOrientation;
		SetLocalOrientation(localOrientation.x, localOrientation.y, localOrientation.z, localOrientation.w);
	}
	else {
		SetLocalOrientation(x, y, z, w);
	}
}

void CSceneNode::SetWorldDirection(float dirx, float diry, float dirz, float upx, float upy, float upz)
{
	glm::vec3 eye = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 center = glm::vec3(dirx, diry, dirz);
	glm::vec3 up = glm::vec3(upx, upy, upz);
	glm::mat4 rotation = glm::lookAt(eye, center, up);
	glm::quat worldOrientation = glm::toQuat(rotation);
	SetWorldOrientation(worldOrientation.x, worldOrientation.y, worldOrientation.z, worldOrientation.w);
}

const glm::vec3& CSceneNode::GetLocalScale(void) const
{
	return m_localScale;
}

const glm::vec3& CSceneNode::GetLocalPosition(void) const
{
	return m_localPosition;
}

const glm::quat& CSceneNode::GetLocalOrientation(void) const
{
	return m_localOrientation;
}

const glm::mat4& CSceneNode::GetLocalTransform(void) const
{
	return m_localTransform;
}

const glm::vec3& CSceneNode::GetWorldScale(void)
{
	UpdateTransformImmediately();
	return m_worldScale;
}

const glm::vec3& CSceneNode::GetWorldPosition(void)
{
	UpdateTransformImmediately();
	return m_worldPosition;
}

const glm::quat& CSceneNode::GetWorldOrientation(void)
{
	UpdateTransformImmediately();
	return m_worldOrientation;
}

const glm::mat4& CSceneNode::GetWorldTransform(void)
{
	UpdateTransformImmediately();
	return m_worldTransform;
}

bool CSceneNode::UpdateTransform(bool bParentUpdate)
{
	bool bUpdate = bParentUpdate || m_bNeedUpdateTransform;

	if (bUpdate) {
		if (m_bNeedUpdateTransform) {
			m_bNeedUpdateTransform = false;
			m_localTransform = glm::translate(glm::mat4(), m_localPosition) * glm::toMat4(m_localOrientation) * glm::scale(glm::mat4(), m_localScale);
		}

		if (m_pParentNode) {
			m_worldTransform = m_pParentNode->m_worldTransform * m_localTransform;
		}
		else {
			m_worldTransform = m_localTransform;
		}
	}

	for (const auto &itNode : m_pChildNodes) {
		itNode.second->UpdateTransform(bUpdate);
	}

	return bUpdate;
}

bool CSceneNode::UpdateTransformImmediately(void)
{
	bool bUpdateParent = false;
	bool bUpdateCurrent = m_bNeedUpdateTransform;

	if (m_bNeedUpdateTransform) {
		m_bNeedUpdateTransform = false;
		m_localTransform = glm::translate(glm::mat4(), m_localPosition) * glm::toMat4(m_localOrientation) * glm::scale(glm::mat4(), m_localScale);
	}

	if (m_pParentNode) {
		bUpdateParent = m_pParentNode->UpdateTransformImmediately();
	}

	if (bUpdateParent || bUpdateCurrent) {
		if (m_pParentNode) {
			m_worldTransform = m_pParentNode->m_worldTransform * m_localTransform;
		}
		else {
			m_worldTransform = m_localTransform;
		}

		m_worldScale = glm::vec3(m_worldTransform * glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
		m_worldPosition = glm::vec3(m_worldTransform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		m_worldOrientation = glm::toQuat(m_worldTransform);
	}

	return bUpdateParent || bUpdateCurrent;
}
