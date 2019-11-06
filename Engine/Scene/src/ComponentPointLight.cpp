#include "EngineHeader.h"


CComponentPointLight::CComponentPointLight(uint32_t name)
	: CComponent(name)
{
	SetMaterial(GfxRenderer()->NewMaterial("PointLight.material"));
	SetMeshDraw(GfxRenderer()->NewMesh("PointLight.mesh", 0));
}

CComponentPointLight::CComponentPointLight(const CComponentPointLight& component)
	: CComponent(component)
{
	m_ptrMaterial = component.m_ptrMaterial;
	m_ptrMeshDraw = component.m_ptrMeshDraw;

	SetColor(component.m_instanceData.color.r, component.m_instanceData.color.g, component.m_instanceData.color.b);
	SetAttenuation(component.m_instanceData.attenuation.x, component.m_instanceData.attenuation.y, component.m_instanceData.attenuation.z);
}

CComponentPointLight::~CComponentPointLight(void)
{

}

void CComponentPointLight::SetMaterial(const CGfxMaterialPtr ptrMaterial)
{
	m_ptrMaterial = ptrMaterial;
}

void CComponentPointLight::SetMeshDraw(const CGfxMeshPtr ptrMesh)
{
	m_ptrMeshDraw = GfxRenderer()->NewMeshDraw(ptrMesh->GetName(), ptrMesh, 0, INSTANCE_ATTRIBUTE_TRANSFORM | INSTANCE_ATTRIBUTE_POINTLIGHT_COLOR | INSTANCE_ATTRIBUTE_POINTLIGHT_ATTENUATION, 1);
}

void CComponentPointLight::SetColor(float red, float green, float blue)
{
	m_instanceData.color = glm::vec4(red, green, blue, 0.0f);
}

void CComponentPointLight::SetAttenuation(float linear, float square, float constant)
{
	m_instanceData.attenuation = glm::vec4(linear, square, constant, 0.0f);
}

glm::aabb CComponentPointLight::GetWorldAABB(void)
{
	if (m_ptrMeshDraw && m_pParentNode) {
		return m_ptrMeshDraw->GetLocalAABB()* m_pParentNode->GetWorldTransform();
	}
	else {
		return glm::aabb();
	}
}

void CComponentPointLight::TaskUpdate(float gameTime, float deltaTime)
{
	if (m_pParentNode && m_pParentNode->IsActive()) {
		m_instanceData.transformMatrix = m_pParentNode->GetWorldTransform();
	}
}

void CComponentPointLight::TaskUpdateCamera(CGfxCamera* pCamera, CGfxRenderQueue* pRenderQueue, uint32_t mask, int indexThread)
{
	if (m_ptrMeshDraw->GetMask() & mask) {
		if (m_pParentNode && m_pParentNode->IsActive()) {
			if (pCamera->IsVisible(GetWorldAABB())) {
				pRenderQueue->Add(indexThread, m_ptrMaterial, m_ptrMeshDraw, (const uint8_t*)& m_instanceData, sizeof(m_instanceData));
			}
		}
	}
}
