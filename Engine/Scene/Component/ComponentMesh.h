#pragma once
#include "Component.h"
#include "ComponentManager.h"


class CComponentMesh : public CComponent
{
public:
	CComponentMesh(uint32_t name);
	CComponentMesh(const CComponentMesh &component);
	virtual ~CComponentMesh(void);


public:
	void SetMaterial(const CGfxMaterialPtr &ptrMaterial);
	void SetMesh(const CGfxMeshPtr &ptrMesh);
	void SetDrawIndirectBuffer(const CGfxDrawIndirectBufferPtr &ptrDrawIndirectBuffer);

	const CGfxMaterialPtr& GetMaterial(void) const;
	const CGfxMeshPtr& GetMesh(void) const;
	const CGfxDrawIndirectBufferPtr& GetDrawIndirectBuffer(void) const;

public:
	glm::aabb GetLocalAABB(void);
	glm::aabb GetWorldAABB(void);

public:
	virtual void TaskUpdate(float gameTime, float deltaTime);
	virtual void TaskUpdateCamera(CGfxCamera *pCamera, int indexThread, int indexQueue);


private:
	CGfxMaterialPtr m_ptrMaterial;
	CGfxMeshPtr m_ptrMesh;
	CGfxDrawIndirectBufferPtr m_ptrDrawIndirectBuffer;
};

typedef CComponentPtr<CComponentMesh> CComponentMeshPtr;
