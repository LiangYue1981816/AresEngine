#include "EngineHeader.h"


CComponentMesh::CComponentMesh(uint32_t name)
	: CComponent(name)
	, m_indexInstance(INVALID_VALUE)
	, m_bNeedUpdateInstanceData{ false }
{
	m_indexInstance = RenderSystem()->GetGPUScene()->AddInstance();
}

CComponentMesh::CComponentMesh(const CComponentMesh& component)
	: CComponent(component)
	, m_indexInstance(INVALID_VALUE)
{
	m_indexInstance = RenderSystem()->GetGPUScene()->AddInstance();

	m_ptrMaterial = component.m_ptrMaterial;
	m_ptrMeshDraw = component.m_ptrMeshDraw;
}

CComponentMesh::~CComponentMesh(void)
{
	RenderSystem()->GetGPUScene()->RemoveInstance(m_indexInstance);
}

void CComponentMesh::SetMaterial(const CGfxMaterialPtr ptrMaterial)
{
	m_ptrMaterial = ptrMaterial;
}

void CComponentMesh::SetMeshDraw(const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding, uint32_t nameAlias)
{
	if (nameAlias == INVALID_HASHNAME) {
		m_ptrMeshDraw = GfxRenderer()->NewMeshDraw(HashValueFormat("%x_%x", ptrMesh->GetName(), nameDraw), ptrMesh, nameDraw, instanceFormat, instanceBinding);
	}
	else {
		m_ptrMeshDraw = GfxRenderer()->NewMeshDraw(HashValueFormat("%x_%x", ptrMesh->GetName(), nameAlias), ptrMesh, nameDraw, instanceFormat, instanceBinding);
	}
}

void CComponentMesh::SetMask(uint32_t mask)
{
	if (m_ptrMeshDraw) {
		m_ptrMeshDraw->SetMask(mask);
	}
}

void CComponentMesh::TaskUpdate(float gameTime, float deltaTime)
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

void CComponentMesh::TaskUpdateCamera(CGfxCamera* pCamera, CRenderQueue* pRenderQueue, uint32_t mask, int indexThread)
{
	int indexFrame = 1 - Engine()->GetFrameCount() % 2;

	if (m_ptrMeshDraw && m_ptrMeshDraw->GetMask() & mask) {
		if (m_pParentNode && m_pParentNode->IsActive()) {
			if (pCamera->IsVisible(m_ptrMeshDraw->GetLocalAABB() * m_instanceData[indexFrame].transformMatrix)) {
				if (m_bNeedUpdateInstanceData[indexFrame]) {
					m_bNeedUpdateInstanceData[indexFrame] = false;
					RenderSystem()->GetGPUScene()->ModifyInstanceData(m_indexInstance, m_instanceData[indexFrame]);
				}

				pRenderQueue->Add(indexThread, m_ptrMaterial, m_ptrMeshDraw, (const uint8_t*)&m_indexInstance, sizeof(m_indexInstance));
			}
		}
	}
}
