#pragma once
#include "PreHeader.h"


class CSceneNode
{
	friend class CScene;
	friend class CSceneManager;


private:
	CSceneNode(uint32_t name, CSceneManager* pSceneManager);
	virtual ~CSceneNode(void);


public:
	uint32_t GetName(void) const;
	CSceneNode* GetParent(void) const;
	CSceneManager* GetSceneManager(void) const;

public:
	void SetActive(bool bActive);
	bool IsActive(void) const;

public:
	bool AttachNode(CSceneNode* pNode);
	bool DetachNode(CSceneNode* pNode, bool bDestroy = false);
	void DetachNodeAll(bool bDestroy = false);
	CSceneNode* GetNode(uint32_t name) const;

public:
	bool AttachComponentMesh(CComponentMeshPtr ptrComponentMesh);
	bool DetachComponentMesh(CComponentMeshPtr ptrComponentMesh);
	void DetachComponentMeshAll(void);
	CComponentMeshPtr GetComponentMesh(uint32_t name) const;

	bool AttachComponentSkin(CComponentSkinPtr ptrComponentSkin);
	bool DetachComponentSkin(CComponentSkinPtr ptrComponentSkin);
	void DetachComponentSkinAll(void);
	CComponentSkinPtr GetComponentSkin(uint32_t name) const;

	bool AttachComponentParticle(CComponentParticlePtr ptrComponentParticle);
	bool DetachComponentParticle(CComponentParticlePtr ptrComponentParticle);
	void DetachComponentParticleAll(void);
	CComponentParticlePtr GetComponentParticle(uint32_t name) const;

	bool AttachComponentPointLight(CComponentPointLightPtr ptrComponentPointLight);
	bool DetachComponentPointLight(CComponentPointLightPtr ptrComponentPointLight);
	void DetachComponentPointLightAll(void);
	CComponentPointLightPtr GetComponentPointLight(uint32_t name) const;

	void DetachComponentAll(void);

private:
	template<class T>
	bool AttachComponent(CComponentPtr<T> ptrComponent, eastl::unordered_map<uint32_t, CComponentPtr<T>>& container);

	template<class T>
	bool DetachComponent(CComponentPtr<T> ptrComponent, eastl::unordered_map<uint32_t, CComponentPtr<T>>& container);

	template<class T>
	void DetachComponentAll(eastl::unordered_map<uint32_t, CComponentPtr<T>>& container);

	template<class T>
	CComponentPtr<T> GetComponent(uint32_t name, const eastl::unordered_map<uint32_t, CComponentPtr<T>>& container) const;

public:
	void Identity(void);

	void SetLocalScale(float x, float y, float z);
	void SetLocalPosition(float x, float y, float z);
	void SetLocalOrientation(float x, float y, float z, float w);
	void SetLocalDirection(float dirx, float diry, float dirz, float upx = 0.0f, float upy = 1.0f, float upz = 0.0f);

	void SetWorldScale(float x, float y, float z);
	void SetWorldPosition(float x, float y, float z);
	void SetWorldOrientation(float x, float y, float z, float w);
	void SetWorldDirection(float dirx, float diry, float dirz, float upx = 0.0f, float upy = 1.0f, float upz = 0.0f);

	const glm::vec3& GetLocalScale(void) const;
	const glm::vec3& GetLocalPosition(void) const;
	const glm::quat& GetLocalOrientation(void) const;
	const glm::mat4& GetLocalTransform(void) const;

	const glm::vec3& GetWorldScale(void);
	const glm::vec3& GetWorldPosition(void);
	const glm::quat& GetWorldOrientation(void);
	const glm::mat4& GetWorldTransform(void);

public:
	bool UpdateTransform(void);


private:
	uint32_t m_name;

private:
	bool m_bActive;

private:
	glm::vec3 m_localScale;
	glm::vec3 m_localPosition;
	glm::quat m_localOrientation;
	glm::mat4 m_localTransform;
	glm::vec3 m_worldScale;
	glm::vec3 m_worldPosition;
	glm::quat m_worldOrientation;
	glm::mat4 m_worldTransform;

	bool m_bUpdateTransform;
	uint32_t m_nUpdateTransformCount;
	uint32_t m_nParentUpdateTransformCount;

private:
	eastl::unordered_map<uint32_t, CSceneNode*> m_pChildNodes;
	eastl::unordered_map<uint32_t, CComponentMeshPtr> m_ptrComponentMeshs;
	eastl::unordered_map<uint32_t, CComponentSkinPtr> m_ptrComponentSkins;
	eastl::unordered_map<uint32_t, CComponentParticlePtr> m_ptrComponentParticles;
	eastl::unordered_map<uint32_t, CComponentPointLightPtr> m_ptrComponentPointLights;

private:
	CSceneNode* m_pParentNode;
	CSceneManager* m_pSceneManager;
};
