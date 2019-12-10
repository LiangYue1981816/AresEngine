#include "EngineHeader.h"


CComponentPointLight::CComponentPointLight(uint32_t name)
	: CComponent(name)
	, m_indexInstance(INVALID_VALUE)
	, m_bNeedUpdateInstanceData{ false }
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

	SetColor(component.m_color.r, component.m_color.g, component.m_color.b);
	SetAttenuation(component.m_attenuation.x, component.m_attenuation.y, component.m_attenuation.z);
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
	m_color = glm::vec4(red, green, blue, 0.0f);
	m_instanceData[0].lightColor = m_color;
	m_instanceData[1].lightColor = m_color;
}

void CComponentPointLight::SetAttenuation(float linear, float square, float constant)
{
	m_attenuation = glm::vec4(linear, square, constant, 0.0f);
	m_instanceData[0].lightAttenuation = m_attenuation;
	m_instanceData[1].lightAttenuation = m_attenuation;
}

void CComponentPointLight::TaskUpdate(float gameTime, float deltaTime)
{
	int indexFrame = Engine()->GetFrameCount() % 2;

	if (m_ptrMeshDraw) {
		if (m_pParentNode && m_pParentNode->IsActive() && m_pParentNode->UpdateTransform()) {
			m_bNeedUpdateInstanceData[indexFrame] = true;
			m_instanceData[indexFrame].transformMatrix = m_pParentNode->GetWorldTransform();
			m_instanceData[indexFrame].position = m_pParentNode->GetWorldPosition();
		}
	}
}

void CComponentPointLight::TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, int indexThread)
{
	int indexFrame = 1 - Engine()->GetFrameCount() % 2;

	if (m_ptrMeshDraw && m_ptrMeshDraw->GetMask() & mask) {
		if (m_pParentNode && m_pParentNode->IsActive()) {
			if (pCamera->IsVisible(m_ptrMeshDraw->GetLocalAABB() * m_instanceData[indexFrame].transformMatrix)) {
				if (m_bNeedUpdateInstanceData[indexFrame]) {
					m_bNeedUpdateInstanceData[indexFrame] = false;
					RenderSystem()->GetGPUScene()->ModifyInstanceData(m_indexInstance, m_instanceData[indexFrame], indexThread);
				}

				pRenderQueue->Add(m_ptrMaterial, m_ptrMeshDraw, (const uint8_t*)&m_indexInstance, sizeof(m_indexInstance), indexThread);
			}
		}
	}
}
