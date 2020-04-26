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

	SetMaterial(GfxRenderer()->NewMaterial("PointLightCullFaceBack.material", VERTEX_BINDING, INSTANCE_BINDING), GfxRenderer()->NewMaterial("PointLightCullFaceFront.material", VERTEX_BINDING, INSTANCE_BINDING));
	SetMeshDraw(GfxRenderer()->NewMesh("PointLight.mesh", VERTEX_BINDING));
	SetMask(0xffffffff);
}

CComponentPointLight::CComponentPointLight(const CComponentPointLight& component)
	: CComponent(component)
	, m_indexInstance(INVALID_VALUE)
	, m_bNeedUpdateInstanceData{ false }

	, m_distance2(0.0f)
	, m_screenSize2(0.0f)

	, m_cullDistance2(FLT_MAX)
	, m_cullScreenSize2(0.0f)
{
	m_indexInstance = RenderSystem()->AddInstance();

	m_ptrMaterialCullFaceBack = component.m_ptrMaterialCullFaceBack;
	m_ptrMaterialCullFaceFront = component.m_ptrMaterialCullFaceFront;
	m_ptrMeshDraw = component.m_ptrMeshDraw;

	m_cullDistance2 = component.m_cullDistance2;
	m_cullScreenSize2 = component.m_cullScreenSize2;

	SetColor(component.m_color.r, component.m_color.g, component.m_color.b);
	SetAttenuation(component.m_attenuation.x, component.m_attenuation.y, component.m_attenuation.z, component.m_attenuation.w);
}

CComponentPointLight::~CComponentPointLight(void)
{
	RenderSystem()->RemoveInstance(m_indexInstance);
}

void CComponentPointLight::SetMaterial(const CGfxMaterialPtr ptrMaterialCullFaceBack, const CGfxMaterialPtr ptrMaterialCullFaceFront)
{
	m_ptrMaterialCullFaceBack = ptrMaterialCullFaceBack;
	m_ptrMaterialCullFaceFront = ptrMaterialCullFaceFront;
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

	m_bNeedUpdateInstanceData[0] = true;
	m_bNeedUpdateInstanceData[1] = true;
}

void CComponentPointLight::SetAttenuation(float linear, float square, float constant, float range)
{
	m_attenuation = glm::vec4(linear, square, constant, range);

	m_instanceData[0].SetLightAttenuation(m_attenuation);
	m_instanceData[1].SetLightAttenuation(m_attenuation);

	m_bNeedUpdateInstanceData[0] = true;
	m_bNeedUpdateInstanceData[1] = true;
}

void CComponentPointLight::SetCullDistance(float distance)
{
	m_cullDistance2 = distance * distance;
}

void CComponentPointLight::SetCullScreenSize(float screenSize)
{
	m_cullScreenSize2 = screenSize * screenSize;
}

bool CComponentPointLight::TaskUpdate(float gameTime, float deltaTime)
{
	int indexFrame = Engine()->GetFrameCount() % 2;
	glm::mat4 transformMatrix = m_pParentNode->GetWorldTransform();

	if (m_instanceData[indexFrame].transformMatrix != transformMatrix) {
		m_instanceData[indexFrame].transformMatrix  = transformMatrix;
		m_bNeedUpdateInstanceData[indexFrame] = true;

		if (m_ptrMeshDraw && m_ptrMaterialCullFaceBack && m_ptrMaterialCullFaceFront) {
			m_aabb = m_ptrMeshDraw->GetAABB() * transformMatrix;
		}

		return true;
	}
	else {
		return false;
	}
}

bool CComponentPointLight::TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, bool bComputeLOD, int indexThread)
{
	int indexFrame = 1 - Engine()->GetFrameCount() % 2;

	if (m_ptrMeshDraw && m_ptrMaterialCullFaceBack && m_ptrMaterialCullFaceFront) {
		m_distance2 = glm::length2(m_aabb.center - pCamera->GetPosition());
		m_screenSize2 = glm::min(glm::length2(m_aabb.size()) / glm::max(1.0f, m_distance2), 1.0f);

		if ((m_ptrMeshDraw->GetMask() & mask) == 0) {
			return false;
		}

		if (m_distance2 > m_cullDistance2) {
			return false;
		}

		if (m_screenSize2 < m_cullScreenSize2) {
			return false;
		}

		if (pCamera->IsVisible(m_aabb) == false) {
			return false;
		}

		if (m_bNeedUpdateInstanceData[indexFrame]) {
			m_bNeedUpdateInstanceData[indexFrame] = false;
			m_instanceData[indexFrame].SetCenter(glm::vec4(m_aabb.center, 1.0f));
			RenderSystem()->ModifyInstanceData(m_indexInstance, m_instanceData[indexFrame], indexThread);
		}

		if (glm::sphere(m_aabb.center, m_attenuation.w).inside(pCamera->GetPosition())) {
			pRenderQueue->Add(m_ptrMaterialCullFaceFront, m_ptrMeshDraw, m_indexInstance, indexThread);
		}
		else {
			pRenderQueue->Add(m_ptrMaterialCullFaceBack, m_ptrMeshDraw, m_indexInstance, indexThread);
		}

		return true;
	}
	else {
		return false;
	}
}
