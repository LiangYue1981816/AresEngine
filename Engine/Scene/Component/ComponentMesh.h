#pragma once
#include "Component.h"
#include "ComponentManager.h"


class CGfxMesh;
class CGfxMaterial;

class CComponentMesh : public CComponent
{
public:
	CComponentMesh(const CComponentMesh &component);
	CComponentMesh(uint32_t name);
	virtual ~CComponentMesh(void);


public:
	void SetMesh(CGfxMesh *pMesh);
	void SetMaterial(CGfxMaterial *pMaterial);

	glm::aabb GetAABB(void) const;
	CGfxMesh* GetMesh(void) const;
	CGfxMaterial* GetMaterial(void) const;

public:
	virtual void Update(float deltaTime);
	virtual void UpdateCamera(CGfxCamera *pCamera, int indexThread);


private:
	CGfxMesh *m_pMesh;
	CGfxMaterial *m_pMaterial;
};

typedef CComponentPtr<CComponentMesh> CComponentMeshPtr;
