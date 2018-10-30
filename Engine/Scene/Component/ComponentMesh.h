#pragma once
#include "Component.h"
#include "ComponentManager.h"


class CComponentMesh : public CComponent
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
	void SetDrawIndirectBuffer(const CGfxDrawIndirectBufferPtr &ptrDrawIndirectBuffer);

public:
	glm::aabb GetLocalAABB(void);
	glm::aabb GetWorldAABB(void);

public:
	virtual void TaskUpdate(float gameTime, float deltaTime);
	virtual void TaskUpdateCamera(CGfxCamera *pCamera, int indexThread, int indexQueue);


private:
	InstanceData m_instanceData;

private:
	CGfxMaterialPtr m_ptrMaterial;
	CGfxMeshPtr m_ptrMesh;
	CGfxDrawIndirectBufferPtr m_ptrDrawIndirectBuffer;
};

typedef CComponentPtr<CComponentMesh> CComponentMeshPtr;
