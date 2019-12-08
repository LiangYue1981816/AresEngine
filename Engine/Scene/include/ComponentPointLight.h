#pragma once
#include "PreHeader.h"


class CALL_API CComponentPointLight : public CComponent
{
private:
	typedef struct InstanceData {
		glm::mat4 transformMatrix;
		glm::vec4 color;
		glm::vec4 attenuation;
	} InstanceData;


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
	InstanceData m_instanceData[2];

private:
	CGfxMaterialPtr m_ptrMaterial;
	CGfxMeshDrawPtr m_ptrMeshDraw;
};

typedef CComponentPtr<CComponentPointLight> CComponentPointLightPtr;
