#pragma once
#include "Component.h"
#include "ComponentManager.h"


class CComponentPointLight : public CComponent
{
public:
	CComponentPointLight(uint32_t name);
	CComponentPointLight(const CComponentPointLight &component);
	virtual ~CComponentPointLight(void);


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

typedef CComponentPtr<CComponentPointLight> CComponentPointLightPtr;
