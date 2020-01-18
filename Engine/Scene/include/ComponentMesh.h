#pragma once
#include "PreHeader.h"


class CALL_API CComponentMesh : public CComponent
{
public:
	CComponentMesh(uint32_t name);
	CComponentMesh(const CComponentMesh& component);
	virtual ~CComponentMesh(void);


public:
	void SetScreenFactor(int indexLOD, float factor);
	void SetMaterial(int indexLOD, const CGfxMaterialPtr ptrMaterial);
	void SetMeshDraw(int indexLOD, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding, uint32_t nameAlias = INVALID_HASHNAME);
	void SetMask(int indexLOD, uint32_t mask);

public:
	virtual void TaskUpdate(float gameTime, float deltaTime);
	virtual void TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, int indexThread);

private:
	int ComputeLOD(void) const;


private:
	int m_indexInstance;
	bool m_bUpdateInstanceData[2];
	bool m_bNeedUpdateInstanceData[2];
	CGPUScene::InstanceData m_instanceData[2];

private:
	static const int MAX_LOD_COUNT = 8;

	int m_indexLOD[2];
	float m_factor[MAX_LOD_COUNT];
	CGfxMaterialPtr m_ptrMaterial[MAX_LOD_COUNT];
	CGfxMeshDrawPtr m_ptrMeshDraw[MAX_LOD_COUNT];
};

typedef CComponentPtr<CComponentMesh> CComponentMeshPtr;
