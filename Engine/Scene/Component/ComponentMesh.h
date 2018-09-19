#pragma once
#include "Component.h"
#include "ComponentManager.h"


class CComponentMesh : public CComponent
{
public:
	CComponentMesh(const CComponentMesh &component);
	CComponentMesh(uint32_t name);
	virtual ~CComponentMesh(void);


public:
	void SetMesh(const CGfxMeshPtr &ptrMesh);
	void SetMaterial(const CGfxMaterialPtr &ptrMaterial);

	const CGfxMeshPtr& GetMesh(void) const;
	const CGfxMaterialPtr& GetMaterial(void) const;

public:
	glm::aabb GetAABB(void);

public:
	virtual void TaskUpdate(float deltaTime);
	virtual void TaskUpdateCamera(CGfxCamera *pCamera, int indexThread);


private:
	CGfxMeshPtr m_ptrMesh;
	CGfxMaterialPtr m_ptrMaterial;
};

typedef CComponentPtr<CComponentMesh> CComponentMeshPtr;
