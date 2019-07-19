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
	CComponentMesh(const CComponentMesh& component);
	virtual ~CComponentMesh(void);


public:
	void SetMaterial(const CGfxMaterialPtr ptrMaterial);
	void SetMeshDraw(const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding, uint32_t nameAlias = INVALID_HASHNAME);
	void SetMask(uint32_t mask);

public:
	glm::aabb GetWorldAABB(void);

public:
	virtual void TaskUpdate(float gameTime, float deltaTime);
	virtual void TaskUpdateCamera(CGfxCamera* pCamera, CGfxRenderQueue* pRenderQueue, int indexThread);


private:
	InstanceData m_instanceData;

private:
	CGfxMaterialPtr m_ptrMaterial;
	CGfxMeshDrawPtr m_ptrMeshDraw;
};

typedef CComponentPtr<CComponentMesh> CComponentMeshPtr;
