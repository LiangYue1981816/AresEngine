#pragma once
#include "Component.h"
#include "ComponentManager.h"


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
	void SetIndexDraw(int indexDraw);
	void SetMesh(const CGfxMeshPtr &ptrMesh);
	void SetMaterial(const CGfxMaterialPtr &ptrMaterial);

public:
	glm::aabb GetLocalAABB(void);
	glm::aabb GetWorldAABB(void);

public:
	virtual void TaskUpdate(float gameTime, float deltaTime);
	virtual void TaskUpdateCamera(CGfxCamera *pCamera, int indexThread, int indexQueue);


private:
	InstanceData m_instanceData;

private:
	int m_indexDraw;
	CGfxMeshPtr m_ptrMesh;
	CGfxMaterialPtr m_ptrMaterial;
};

typedef CComponentPtr<CComponentMesh> CComponentMeshPtr;
