#pragma once
#include "PreHeader.h"


class CALL_API CComponentMesh : public CComponent
{
public:
	static const int MAX_LOD_COUNT = 8;


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
	virtual void TaskUpdate(float gameTime, float deltaTime);
	virtual void TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, bool bComputeLOD, int indexThread);

private:
	bool ComputeLOD(bool bComputeLOD, const glm::vec3& cameraPosition, const glm::mat4& transformMatrix);


private:
	int m_indexInstance;
	bool m_bNeedUpdateInstanceData[2];
	InstanceData m_instanceData[2];

private:
	struct LODMeshDraw {
		float factor;

		glm::aabb aabb;
		float length2;
		float screenSize2;

		CGfxMaterialPtr ptrMaterial;
		CGfxMeshDrawPtr ptrMeshDraw;
	};

	int m_indexLOD;
	LODMeshDraw m_LODMeshDraws[MAX_LOD_COUNT];

private:
	float m_cullDistance;
	float m_cullScreenSize;
};

typedef CComponentPtr<CComponentMesh> CComponentMeshPtr;
