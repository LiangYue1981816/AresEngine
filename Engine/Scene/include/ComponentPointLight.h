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

	void SetColor(float red, float green, float blue);
	void SetAttenuation(float linear, float square, float constant);

public:
	virtual void TaskUpdate(float gameTime, float deltaTime);
	virtual void TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, int indexThread);


private:
	glm::vec4 m_color;
	glm::vec4 m_attenuation;

private:
	int m_indexInstance;
	bool m_bUpdateInstanceData[2];
	bool m_bNeedUpdateInstanceData[2];
	InstanceData m_instanceData[2];

private:
	CGfxMaterialPtr m_ptrMaterial;
	CGfxMeshDrawPtr m_ptrMeshDraw;
};

typedef CComponentPtr<CComponentPointLight> CComponentPointLightPtr;
