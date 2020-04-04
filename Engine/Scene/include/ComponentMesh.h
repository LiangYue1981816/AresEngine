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
	void SetMeshDraw(int indexLOD, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t nameAlias = INVALID_HASHNAME);
	void SetMask(int indexLOD, uint32_t mask);

	void SetCullDistance(float distance);
	void SetCullScreenSize(float screenSize);

public:
	virtual bool TaskUpdate(float gameTime, float deltaTime);
	virtual bool TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, bool bComputeLOD, int indexThread);

private:
	bool ComputeLOD(bool bComputeLOD, const glm::vec3& cameraPosition);


private:
	int m_indexInstance;
	bool m_bNeedUpdateInstanceData[2];
	InstanceData m_instanceData[2];

private:
	static const int MAX_LOD_COUNT = 8;

	int m_indexLOD;

	float m_distance2[MAX_LOD_COUNT];
	float m_screenSize2[MAX_LOD_COUNT];
	float m_screenSizeFactor[MAX_LOD_COUNT];

	glm::aabb m_aabb[MAX_LOD_COUNT];
	CGfxMaterialPtr m_ptrMaterial[MAX_LOD_COUNT];
	CGfxMeshDrawPtr m_ptrMeshDraw[MAX_LOD_COUNT];

private:
	float m_cullDistance2;
	float m_cullScreenSize2;
};
