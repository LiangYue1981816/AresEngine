#include "EngineHeader.h"


CComponentPointLight::CComponentPointLight(uint32_t name)
	: CComponent(name)
	, m_indexInstance(INVALID_VALUE)
{
	m_indexInstance = RenderSystem()->GetGPUScene()->AddInstance();

	SetMaterial(GfxRenderer()->NewMaterial("PointLight.material"));
	SetMeshDraw(GfxRenderer()->NewMesh("PointLight.mesh", 0));
}

CComponentPointLight::CComponentPointLight(const CComponentPointLight& component)
	: CComponent(component)
	, m_indexInstance(INVALID_VALUE)
{
	m_indexInstance = RenderSystem()->GetGPUScene()->AddInstance();

	m_ptrMaterial = component.m_ptrMaterial;
	m_ptrMeshDraw = component.m_ptrMeshDraw;

	SetColor(component.m_instanceData[0].color.r, component.m_instanceData[0].color.g, component.m_instanceData[0].color.b);
	SetAttenuation(component.m_instanceData[0].attenuation.x, component.m_instanceData[0].attenuation.y, component.m_instanceData[0].attenuation.z);
}

CComponentPointLight::~CComponentPointLight(void)
{
	RenderSystem()->GetGPUScene()->RemoveInstance(m_indexInstance);
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
	m_instanceData[0].color = glm::vec4(red, green, blue, 0.0f);
	m_instanceData[1].color = glm::vec4(red, green, blue, 0.0f);
}

void CComponentPointLight::SetAttenuation(float linear, float square, float constant)
{
	m_instanceData[0].attenuation = glm::vec4(linear, square, constant, 0.0f);
	m_instanceData[1].attenuation = glm::vec4(linear, square, constant, 0.0f);
}

void CComponentPointLight::TaskUpdate(float gameTime, float deltaTime)
{
	if (m_ptrMeshDraw) {
		if (m_pParentNode && m_pParentNode->IsActive()) {
			m_instanceData[Engine()->GetFrameCount() % 2].transformMatrix = m_pParentNode->GetWorldTransform();
		}
	}
}

void CComponentPointLight::TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, int indexThread)
{
	if (m_ptrMeshDraw && m_ptrMeshDraw->GetMask() & mask) {
		if (m_pParentNode && m_pParentNode->IsActive()) {
			if (pCamera->IsVisible(m_ptrMeshDraw->GetLocalAABB() * m_instanceData[1 - Engine()->GetFrameCount() % 2].transformMatrix)) {
				pRenderQueue->Add(indexThread, m_ptrMaterial, m_ptrMeshDraw, (const uint8_t*)& m_instanceData[1 - Engine()->GetFrameCount() % 2], sizeof(InstanceData));
			}
		}
	}
}
