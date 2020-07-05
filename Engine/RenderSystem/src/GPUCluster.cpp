#include "EngineHeader.h"
#include "RenderHeader.h"


CGPUCluster::CGPUCluster(CRenderSystem* pRenderSystem)
	: m_pRenderSystem(pRenderSystem)
	, m_pCamera(nullptr)

	, m_pClusterShaderCompute(nullptr)
	, m_pClusterPipelineCompute(nullptr)

	, m_pClusterCullShaderCompute(nullptr)
	, m_pClusterCullPipelineCompute(nullptr)
{
	m_ptrClusterBuffer = GfxRenderer()->NewStorageBuffer(CLUSTER_WIDTH_TILE_COUNT * CLUSTER_HEIGHT_TILE_COUNT * CLUSTER_DEPTH_TILE_COUNT * 32);
	m_ptrFullLightListBuffer = GfxRenderer()->NewStorageBuffer(MAX_GPUSCENE_INSTANCE_COUNT * sizeof(int));
	m_ptrCullLightListBuffer = GfxRenderer()->NewStorageBuffer(MAX_GPUSCENE_INSTANCE_COUNT * sizeof(int));

	{
		char szFileName[] = "GPU_Cluster.glsl";
		char szBinFileName[_MAX_STRING] = { 0 };

		sprintf(szBinFileName, "%x.comp", HashValue(szFileName));
		ShaderCompiler()->Compile(FileManager()->GetFullName(szFileName), szBinFileName, shaderc_compute_shader);

		m_pClusterShaderCompute = GfxRenderer()->CreateShader(szBinFileName, compute_shader);
		m_pClusterPipelineCompute = GfxRenderer()->CreatePipelineCompute(m_pClusterShaderCompute);

		m_ptrClusterDescriptorSet = GfxRenderer()->NewDescriptorSet(HashValue(szFileName), m_pClusterPipelineCompute->GetDescriptorLayout(DESCRIPTOR_SET_PASS));
		m_ptrClusterDescriptorSet->SetStorageBuffer(STORAGE_CLUSTER_DATA_NAME, m_ptrClusterBuffer, 0, m_ptrClusterBuffer->GetSize());
	}

	{
		char szFileName[] = "GPU_ClusterCull.glsl";
		char szBinFileName[_MAX_STRING] = { 0 };

		sprintf(szBinFileName, "%x.comp", HashValue(szFileName));
		ShaderCompiler()->Compile(FileManager()->GetFullName(szFileName), szBinFileName, shaderc_compute_shader);

		m_pClusterCullShaderCompute = GfxRenderer()->CreateShader(szBinFileName, compute_shader);
		m_pClusterCullPipelineCompute = GfxRenderer()->CreatePipelineCompute(m_pClusterCullShaderCompute);

		m_ptrClusterCullDescriptorSet = GfxRenderer()->NewDescriptorSet(HashValue(szFileName), m_pClusterCullPipelineCompute->GetDescriptorLayout(DESCRIPTOR_SET_PASS));
		m_ptrClusterCullDescriptorSet->SetStorageBuffer(STORAGE_SCENE_DATA_NAME, m_pRenderSystem->GetInstanceDataBuffer(), 0, m_pRenderSystem->GetInstanceDataBuffer()->GetSize());
		m_ptrClusterCullDescriptorSet->SetStorageBuffer(STORAGE_CLUSTER_DATA_NAME, m_ptrClusterBuffer, 0, m_ptrClusterBuffer->GetSize());
		m_ptrClusterCullDescriptorSet->SetStorageBuffer(STORAGE_FULL_LIGHT_LIST_DATA_NAME, m_ptrFullLightListBuffer, 0, m_ptrFullLightListBuffer->GetSize());
		m_ptrClusterCullDescriptorSet->SetStorageBuffer(STORAGE_CULL_LIGHT_LIST_DATA_NAME, m_ptrCullLightListBuffer, 0, m_ptrCullLightListBuffer->GetSize());
	}
}

CGPUCluster::~CGPUCluster(void)
{

}

const CGfxStorageBufferPtr CGPUCluster::GetClusterBuffer(void) const
{
	return m_ptrClusterBuffer;
}

const CGfxStorageBufferPtr CGPUCluster::GetCullLightListBuffer(void) const
{
	return m_ptrCullLightListBuffer;
}

void CGPUCluster::SetCamera(CCamera* pCamera)
{
	if (m_pCamera != pCamera) {
		m_pCamera = pCamera;
		m_ptrClusterDescriptorSet->SetUniformBuffer(UNIFORM_CAMERA_NAME, pCamera->GetCameraUniform()->GetUniformBuffer(), 0, pCamera->GetCameraUniform()->GetUniformBuffer()->GetSize());
		m_ptrClusterCullDescriptorSet->SetUniformBuffer(UNIFORM_CAMERA_NAME, pCamera->GetCameraUniform()->GetUniformBuffer(), 0, pCamera->GetCameraUniform()->GetUniformBuffer()->GetSize());
	}
}

void CGPUCluster::Compute(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer)
{
	// Update
	m_pCamera->GetCameraUniform()->Apply();

	// Update Light list
	CGfxMeshPtr ptrMesh = GfxRenderer()->NewMesh("PointLight.mesh", VERTEX_BINDING);
	CGfxMeshDrawPtr ptrMeshDraw = GfxRenderer()->NewMeshDraw(ptrMesh->GetName(), ptrMesh, 0);
	CGfxMaterialPtr ptrMaterialCullFaceBack = GfxRenderer()->NewMaterial("PassDeferredShading_Back.material", VERTEX_BINDING, INSTANCE_BINDING);
	CGfxMaterialPtr ptrMaterialCullFaceFront = GfxRenderer()->NewMaterial("PassDeferredShading_Front.material", VERTEX_BINDING, INSTANCE_BINDING);

	const eastl::vector<int>& instnaces0 = m_pCamera->GetRenderQueue()->GetInstanceBuffer(ptrMaterialCullFaceFront, ptrMeshDraw);
	const eastl::vector<int>& instnaces1 = m_pCamera->GetRenderQueue()->GetInstanceBuffer(ptrMaterialCullFaceBack, ptrMeshDraw);

	eastl::vector<int> instance;
	instance.insert(instance.end(), instnaces0.begin(), instnaces0.end());
	instance.insert(instance.end(), instnaces1.begin(), instnaces1.end());

	m_ptrFullLightListBuffer->BufferData(0, sizeof(int) * std::min((int)instance.size(), MAX_GPUSCENE_INSTANCE_COUNT), instance.data());

	// Cluster
	GfxRenderer()->CmdPushDebugGroup(ptrCommandBuffer, "Cluster");
	{
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_ptrClusterBuffer, GFX_ACCESS_TRANSFER_READ_BIT, GFX_ACCESS_TRANSFER_WRITE_BIT);
		{
			GfxRenderer()->CmdBindPipelineCompute(ptrCommandBuffer, m_pClusterPipelineCompute);
			GfxRenderer()->CmdBindDescriptorSet(ptrCommandBuffer, m_ptrClusterDescriptorSet);
			GfxRenderer()->CmdDispatch(ptrCommandBuffer, CLUSTER_WIDTH_TILE_COUNT, CLUSTER_HEIGHT_TILE_COUNT, CLUSTER_DEPTH_TILE_COUNT);
		}
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_ptrClusterBuffer, GFX_ACCESS_TRANSFER_WRITE_BIT, GFX_ACCESS_TRANSFER_READ_BIT);
	}
	GfxRenderer()->CmdPopDebugGroup(ptrCommandBuffer);

	// Cluster Cull
	GfxRenderer()->CmdPushDebugGroup(ptrCommandBuffer, "ClusterCull");
	{
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_ptrClusterBuffer, GFX_ACCESS_TRANSFER_READ_BIT, GFX_ACCESS_TRANSFER_WRITE_BIT);
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_ptrCullLightListBuffer, GFX_ACCESS_TRANSFER_READ_BIT, GFX_ACCESS_TRANSFER_WRITE_BIT);
		{
			GfxRenderer()->CmdBindPipelineCompute(ptrCommandBuffer, m_pClusterCullPipelineCompute);
			GfxRenderer()->CmdBindDescriptorSet(ptrCommandBuffer, m_ptrClusterCullDescriptorSet);
			GfxRenderer()->CmdUniform1i(ptrCommandBuffer, HashValue("Param.numPointLights"), std::min((int)instance.size(), MAX_GPUSCENE_INSTANCE_COUNT));
			GfxRenderer()->CmdDispatch(ptrCommandBuffer, 1, 1, 1);
		}
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_ptrCullLightListBuffer, GFX_ACCESS_TRANSFER_WRITE_BIT, GFX_ACCESS_TRANSFER_READ_BIT);
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_ptrClusterBuffer, GFX_ACCESS_TRANSFER_WRITE_BIT, GFX_ACCESS_TRANSFER_READ_BIT);
	}
	GfxRenderer()->CmdPopDebugGroup(ptrCommandBuffer);
}
