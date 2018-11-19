#pragma once
#include "Component.h"


class CALL_API CComponentMesh : public CComponent
{
private:
	typedef struct InstanceData {
		glm::mat4 transformMatrix;
	} InstanceData;


public:
	CComponentMesh(uint32_t name);
	CComponentMesh(const CComponentMesh &component);
	virtual ~CComponentMesh(void);


public:
	void SetMaterial(const CGfxMaterialPtr &ptrMaterial);
	void SetMesh(const CGfxMeshPtr &ptrMesh);
	void SetIndexDraw(int indexDraw);

public:
	glm::aabb GetLocalAABB(void);
	glm::aabb GetWorldAABB(void);

public:
	virtual void TaskUpdate(float gameTime, float deltaTime);
	virtual void TaskUpdateCamera(CGfxCamera *pCamera, int indexThread, int indexQueue);


private:
	glm::aabb m_localAABB;
	glm::aabb m_worldAABB;

private:
	InstanceData m_instanceData;

private:
	CGfxMaterialPtr m_ptrMaterial;
	CGfxMeshPtr m_ptrMesh;
	int m_indexDraw;
};

typedef CComponentPtr<CComponentMesh> CComponentMeshPtr;
