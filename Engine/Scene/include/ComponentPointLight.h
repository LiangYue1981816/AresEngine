#pragma once
#include "PreHeader.h"


class CALL_API CComponentPointLight : public CComponent
{
public:
	CComponentPointLight(uint32_t name);
	CComponentPointLight(const CComponentPointLight& component);
	virtual ~CComponentPointLight(void);


public:
	void SetMaterial(const CGfxMaterialPtr ptrMaterialCullFaceBack, const CGfxMaterialPtr ptrMaterialCullFaceFront);
	void SetMeshDraw(const CGfxMeshPtr ptrMesh);
	void SetMask(uint32_t mask);

	void SetColor(float red, float green, float blue);
	void SetAttenuation(float linear, float square, float constant, float range);

	void SetCullDistance(float distance);
	void SetCullScreenSize(float screenSize);

public:
	virtual bool TaskUpdate(float gameTime, float deltaTime);
	virtual bool TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, bool bComputeLOD, int indexThread);


private:
	glm::vec4 m_color;
	glm::vec4 m_attenuation;

private:
	int m_indexInstance;
	bool m_bNeedUpdateInstance[2];
	InstanceData m_instances[2];

private:
	float m_distance2;
	float m_screenSize2;

	glm::aabb m_aabb;
	CGfxMaterialPtr m_ptrMaterialCullFaceBack;
	CGfxMaterialPtr m_ptrMaterialCullFaceFront;
	CGfxMeshDrawPtr m_ptrMeshDraw;

private:
	float m_cullDistance2;
	float m_cullScreenSize2;
};

typedef CComponentPtr<CComponentPointLight> CComponentPointLightPtr;
