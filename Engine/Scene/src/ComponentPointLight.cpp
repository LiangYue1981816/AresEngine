#include "EngineHeader.h"


CComponentPointLight::CComponentPointLight(uint32_t name)
	: CComponent(name)
	, m_indexInstance(INVALID_VALUE)
	, m_bNeedUpdateInstanceData{ false }

	, m_distance2(0.0f)
	, m_screenSize2(0.0f)

	, m_cullDistance2(FLT_MAX)
	, m_cullScreenSize2(0.0f)
{
	m_indexInstance = RenderSystem()->AddInstance();

	SetMaterial(GfxRenderer()->NewMaterial("PointLight.material", VERTEX_BINDING, INSTANCE_BINDING));
	SetMeshDraw(GfxRenderer()->NewMesh("PointLight.mesh", VERTEX_BINDING));
}

CComponentPointLight::CComponentPointLight(const CComponentPointLight& component)
	: CComponent(component)
	, m_indexInstance(INVALID_VALUE)
	, m_bNeedUpdateInstanceData{ false }

	, m_cullDistance2(FLT_MAX)
	, m_cullScreenSize2(0.0f)
{
	m_indexInstance = RenderSystem()->AddInstance();

	m_ptrMaterial = component.m_ptrMaterial;
	m_ptrMeshDraw = component.m_ptrMeshDraw;

	SetColor(component.m_color.r, component.m_color.g, component.m_color.b);
	SetAttenuation(component.m_attenuation.x, component.m_attenuation.y, component.m_attenuation.z);
}

CComponentPointLight::~CComponentPointLight(void)
{
	RenderSystem()->RemoveInstance(m_indexInstance);
}

void CComponentPointLight::SetMaterial(const CGfxMaterialPtr ptrMaterial)
{
	m_ptrMaterial = ptrMaterial;
}

void CComponentPointLight::SetMeshDraw(const CGfxMeshPtr ptrMesh)
{
	m_ptrMeshDraw = GfxRenderer()->NewMeshDraw(ptrMesh->GetName(), ptrMesh, 0);
}

void CComponentPointLight::SetMask(uint32_t mask)
{
	if (m_ptrMeshDraw) {
		m_ptrMeshDraw->SetMask(mask);
	}
}

void CComponentPointLight::SetColor(float red, float green, float blue)
{
	m_color = glm::vec4(red, green, blue, 0.0f);
	m_instanceData[0].SetLightColor(m_color);
	m_instanceData[1].SetLightColor(m_color);
}

void CComponentPointLight::SetAttenuation(float linear, float square, float constant)
{
	m_attenuation = glm::vec4(linear, square, constant, 0.0f);
	m_instanceData[0].SetLightAttenuation(m_attenuation);
	m_instanceData[1].SetLightAttenuation(m_attenuation);
}

void CComponentPointLight::SetCullDistance(float distance)
{
	m_cullDistance2 = distance * distance;
}

void CComponentPointLight::SetCullScreenSize(float screenSize)
{
	m_cullScreenSize2 = screenSize * screenSize;
}

void CComponentPointLight::TaskUpdate(float gameTime, float deltaTime)
{
	/*
	int indexFrame = Engine()->GetFrameCount() % 2;

	if (m_ptrMeshDraw) {
		if (m_pParentNode && m_pParentNode->IsActive()) {
			if (m_bUpdateInstanceData[indexFrame] == false || m_pParentNode->UpdateTransform()) {
				m_bUpdateInstanceData[indexFrame] = true;
				m_bNeedUpdateInstanceData[indexFrame] = true;
				m_instanceData[indexFrame].transformMatrix = m_pParentNode->GetWorldTransform();
				m_instanceData[indexFrame].center = m_instanceData[indexFrame].transformMatrix * glm::vec4(m_ptrMeshDraw->GetAABB().center, 1.0f);
			}
		}
	}
	*/
}

void CComponentPointLight::TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, bool bComputeLOD, int indexThread)
{
	/*
	int indexFrame = 1 - Engine()->GetFrameCount() % 2;

	if (m_ptrMeshDraw) {
		if (m_ptrMeshDraw->GetMask() & mask) {
			if (m_pParentNode && m_pParentNode->IsActive()) {
				if (m_bUpdateInstanceData[indexFrame]) {
					if (pCamera->IsVisible(m_ptrMeshDraw->GetAABB() * m_instanceData[indexFrame].transformMatrix)) {
						if (m_bNeedUpdateInstanceData[indexFrame]) {
							m_bNeedUpdateInstanceData[indexFrame] = false;
							RenderSystem()->ModifyInstanceData(m_indexInstance, m_instanceData[indexFrame], indexThread);
						}

						pRenderQueue->Add(m_ptrMaterial, m_ptrMeshDraw, m_indexInstance, indexThread);
					}
				}
			}
		}
	}
	*/
}
