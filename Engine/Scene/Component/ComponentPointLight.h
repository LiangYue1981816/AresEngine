#pragma once
#include "Component.h"
#include "ComponentManager.h"


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
	CComponentPointLight(const CComponentPointLight &component);
	virtual ~CComponentPointLight(void);


public:
	void SetColor(float red, float green, float blue);
	void SetAttenuation(float linear, float square, float constant);

public:
	glm::aabb GetLocalAABB(void);
	glm::aabb GetWorldAABB(void);

public:
	virtual void TaskUpdate(float gameTime, float deltaTime);
	virtual void TaskUpdateCamera(CGfxCamera *pCamera, int indexThread, int indexQueue);


private:
	InstanceData m_instanceData;

private:
	CGfxMaterialPtr m_ptrMaterial;
	CGfxMeshPtr m_ptrMesh;
};

typedef CComponentPtr<CComponentPointLight> CComponentPointLightPtr;
