#include "EngineHeader.h"
#include "RenderHeader.h"


CCluster::CCluster(CRenderSystem* pRenderSystem, int screenWidth, int screenHeight, int tileSize, int numSlices, int maxInstanceCount)
	: MAX_INSTANCE_COUNT(maxInstanceCount)
	, HORIZONTAL_TILE_COUNT((int)(1.0f * screenWidth / tileSize + 0.5f))
	, VERTICAL_TILE_COUNT((int)(1.0f * screenHeight / tileSize + 0.5f))
	, DEPTH_SLICE_COUNT(numSlices)
	, TILE_SIZE(tileSize)

	, m_pRenderSystem(pRenderSystem)
	, m_pCamera(nullptr)
	, m_pShaderCompute(nullptr)
	, m_pPipelineCompute(nullptr)
{
	char szFileName[] = "Cluster.glsl";
	char szBinFileName[_MAX_STRING] = { 0 };

	sprintf(szBinFileName, "%x.comp", HashValue(szFileName));
	ShaderCompiler()->Compile(FileManager()->GetFullName(szFileName), szBinFileName, shaderc_compute_shader);

	m_pShaderCompute = GfxRenderer()->CreateShader(szBinFileName, compute_shader);
	m_pPipelineCompute = GfxRenderer()->CreatePipelineCompute(m_pShaderCompute);

	m_ptrClusterBuffer = GfxRenderer()->NewStorageBuffer(HORIZONTAL_TILE_COUNT * VERTICAL_TILE_COUNT * DEPTH_SLICE_COUNT * 32);
	m_ptrFullLightListBuffer = GfxRenderer()->NewStorageBuffer(MAX_INSTANCE_COUNT * sizeof(int));
	m_ptrCullLightListBuffer = GfxRenderer()->NewStorageBuffer(MAX_INSTANCE_COUNT * sizeof(int));

	m_ptrDescriptorSet = GfxRenderer()->NewDescriptorSet(HashValue(szFileName), m_pPipelineCompute->GetDescriptorLayout(DESCRIPTOR_SET_PASS));
	m_ptrDescriptorSet->SetStorageBuffer(STORAGE_SCENE_DATA_NAME, m_pRenderSystem->GetGPUScene()->GetInstanceBuffer(), 0, m_pRenderSystem->GetGPUScene()->GetInstanceBuffer()->GetSize());
	m_ptrDescriptorSet->SetStorageBuffer(STORAGE_CLUSTER_DATA_NAME, m_ptrClusterBuffer, 0, m_ptrClusterBuffer->GetSize());
	m_ptrDescriptorSet->SetStorageBuffer(STORAGE_FULL_LIGHT_LIST_DATA_NAME, m_ptrFullLightListBuffer, 0, m_ptrFullLightListBuffer->GetSize());
	m_ptrDescriptorSet->SetStorageBuffer(STORAGE_CULL_LIGHT_LIST_DATA_NAME, m_ptrCullLightListBuffer, 0, m_ptrCullLightListBuffer->GetSize());
}

CCluster::~CCluster(void)
{

}

const CGfxStorageBufferPtr CCluster::GetClusterBuffer(void) const
{
	return m_ptrClusterBuffer;
}

const CGfxStorageBufferPtr CCluster::GetCullLightListBuffer(void) const
{
	return m_ptrCullLightListBuffer;
}

void CCluster::SetCamera(CCamera* pCamera)
{
	if (m_pCamera != pCamera) {
		m_pCamera = pCamera;
		m_ptrDescriptorSet->SetUniformBuffer(UNIFORM_CAMERA_NAME, pCamera->GetCameraUniform()->GetUniformBuffer(), 0, pCamera->GetCameraUniform()->GetUniformBuffer()->GetSize());
	}
}

void CCluster::Update(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer)
{
	// Update
	m_pCamera->GetCameraUniform()->Apply();

	// Update Light list
	CGfxMeshPtr ptrMesh = GfxRenderer()->NewMesh("PointLight.mesh", VERTEX_BINDING);
	CGfxMeshDrawPtr ptrMeshDraw = GfxRenderer()->NewMeshDraw(ptrMesh->GetName(), ptrMesh, 0);
	CGfxMaterialPtr ptrMaterialCullFaceBack = GfxRenderer()->NewMaterial("PointLightCullFaceBack.material", VERTEX_BINDING, INSTANCE_BINDING);
	CGfxMaterialPtr ptrMaterialCullFaceFront = GfxRenderer()->NewMaterial("PointLightCullFaceFront.material", VERTEX_BINDING, INSTANCE_BINDING);

	const eastl::vector<int>& instnaces0 = m_pCamera->GetRenderQueue()->GetInstanceBuffer(ptrMaterialCullFaceFront, ptrMeshDraw);
	const eastl::vector<int>& instnaces1 = m_pCamera->GetRenderQueue()->GetInstanceBuffer(ptrMaterialCullFaceBack, ptrMeshDraw);

	eastl::vector<int> instance;
	instance.insert(instance.end(), instnaces0.begin(), instnaces0.end());
	instance.insert(instance.end(), instnaces1.begin(), instnaces1.end());

	m_ptrFullLightListBuffer->BufferData(0, std::min((int)instance.size(), MAX_INSTANCE_COUNT), instance.data());

	// Cluster
	GfxRenderer()->CmdPushDebugGroup(ptrCommandBuffer, "Cluster");
	{
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_ptrClusterBuffer, GFX_ACCESS_TRANSFER_READ_BIT, GFX_ACCESS_TRANSFER_WRITE_BIT);
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_ptrCullLightListBuffer, GFX_ACCESS_TRANSFER_READ_BIT, GFX_ACCESS_TRANSFER_WRITE_BIT);
		{
			GfxRenderer()->CmdBindPipelineCompute(ptrCommandBuffer, m_pPipelineCompute);
			GfxRenderer()->CmdBindDescriptorSet(ptrCommandBuffer, m_ptrDescriptorSet);
			GfxRenderer()->CmdUniform1i(ptrCommandBuffer, HashValue("Param.tileSize"), TILE_SIZE);
			GfxRenderer()->CmdUniform1f(ptrCommandBuffer, HashValue("Param.numDepthSlices"), DEPTH_SLICE_COUNT);
			GfxRenderer()->CmdDispatch(ptrCommandBuffer, HORIZONTAL_TILE_COUNT, VERTICAL_TILE_COUNT, DEPTH_SLICE_COUNT);
		}
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_ptrCullLightListBuffer, GFX_ACCESS_TRANSFER_WRITE_BIT, GFX_ACCESS_TRANSFER_READ_BIT);
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_ptrClusterBuffer, GFX_ACCESS_TRANSFER_WRITE_BIT, GFX_ACCESS_TRANSFER_READ_BIT);
	}
	GfxRenderer()->CmdPopDebugGroup(ptrCommandBuffer);
}
