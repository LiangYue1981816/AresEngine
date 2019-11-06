#pragma once
#include "PreHeader.h"


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
	virtual void TaskUpdate(float gameTime, float deltaTime);
	virtual void TaskUpdateCamera(CGfxCamera* pCamera, CGfxRenderQueue* pRenderQueue, uint32_t mask, int indexThread);


private:
	InstanceData m_instanceData[2];

private:
	CGfxMaterialPtr m_ptrMaterial;
	CGfxMeshDrawPtr m_ptrMeshDraw;
};

typedef CComponentPtr<CComponentMesh> CComponentMeshPtr;
