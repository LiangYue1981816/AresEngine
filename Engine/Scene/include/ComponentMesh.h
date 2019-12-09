#pragma once
#include "PreHeader.h"


class CALL_API CComponentMesh : public CComponent
{
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
	virtual void TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, int indexThread);


private:
	int m_indexInstance;
	bool m_bNeedUpdateInstanceData[2];
	CGPUScene::InstanceData m_instanceData[2];

private:
	CGfxMaterialPtr m_ptrMaterial;
	CGfxMeshDrawPtr m_ptrMeshDraw;
};

typedef CComponentPtr<CComponentMesh> CComponentMeshPtr;
