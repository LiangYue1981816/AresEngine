#pragma once
#include "PreHeader.h"


class CALL_API CComponentPointLight : public CComponent
{
public:
	CComponentPointLight(uint32_t name);
	CComponentPointLight(const CComponentPointLight& component);
	virtual ~CComponentPointLight(void);


public:
	void SetMaterial(const CGfxMaterialPtr ptrMaterial);
	void SetMeshDraw(const CGfxMeshPtr ptrMesh);
	void SetMask(uint32_t mask);

	void SetColor(float red, float green, float blue);
	void SetAttenuation(float linear, float square, float constant);

	void SetCullDistance(float distance);
	void SetCullScreenSize(float screenSize);

public:
	virtual void TaskUpdate(float gameTime, float deltaTime);
	virtual void TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, bool bComputeLOD, int indexThread);


private:
	glm::vec4 m_color;
	glm::vec4 m_attenuation;

private:
	int m_indexInstance;
	bool m_bNeedUpdateInstanceData[2];
	InstanceData m_instanceData[2];

private:
	glm::aabb m_aabb;
	float m_distance2;
	float m_screenSize2;

	CGfxMaterialPtr m_ptrMaterial;
	CGfxMeshDrawPtr m_ptrMeshDraw;

private:
	float m_cullDistance2;
	float m_cullScreenSize2;
};

typedef CComponentPtr<CComponentPointLight> CComponentPointLightPtr;
