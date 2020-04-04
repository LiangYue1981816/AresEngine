#pragma once
#include "PreHeader.h"


class CALL_API CComponentMesh : public CComponent
{
private:
	typedef struct LODMeshDraw {
		LODMeshDraw(void)
		{
			distance2 = 0.0f;
			screenSize2 = 0.0f;
			screenSizeFactor = 0.0f;
		}

		float distance2;
		float screenSize2;
		float screenSizeFactor;

		glm::aabb aabb;
		CGfxMaterialPtr ptrMaterial;
		CGfxMeshDrawPtr ptrMeshDraw;
	} LODMeshDraw;


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
	float m_cullDistance2;
	float m_cullScreenSize2;

private:
	static const int MAX_LOD_COUNT = 8;

	int m_LODIndex;
	LODMeshDraw m_LODMeshDraws[MAX_LOD_COUNT];
};
