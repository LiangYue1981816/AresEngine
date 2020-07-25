#include "EngineHeader.h"
#include "RenderHeader.h"


CGPUClusterCull::CGPUClusterCull(CRenderSystem* pRenderSystem)
	: CPassBase(pRenderSystem)
	, m_pShaderCompute(nullptr)
	, m_pPipelineCompute(nullptr)
{
	char szFileName[] = "GPU_ClusterCull.glsl";
	char szBinFileName[_MAX_STRING] = { 0 };

	sprintf(szBinFileName, "%x.comp", HashValue(szFileName));
	ShaderCompiler()->Compile(FileManager()->GetFullName(szFileName), szBinFileName, shaderc_compute_shader);

	m_pShaderCompute = GfxRenderer()->CreateShader(szBinFileName, compute_shader);
	m_pPipelineCompute = GfxRenderer()->CreatePipelineCompute(m_pShaderCompute);

	m_ptrDescriptorSet = GfxRenderer()->NewDescriptorSet(HashValue(szFileName), m_pPipelineCompute->GetDescriptorLayout(DESCRIPTOR_SET_PASS));
	m_ptrDescriptorSet->SetStorageBuffer(STORAGE_SCENE_DATA_NAME, m_pRenderSystem->GetInstanceBuffer(), 0, m_pRenderSystem->GetInstanceBuffer()->GetSize());
	m_ptrDescriptorSet->SetStorageBuffer(STORAGE_CLUSTER_DATA_NAME, m_pRenderSystem->GetClusterBuffer(), 0, m_pRenderSystem->GetClusterBuffer()->GetSize());
	m_ptrDescriptorSet->SetStorageBuffer(STORAGE_FULL_LIGHT_INDEX_DATA_NAME, m_pRenderSystem->GetFullLightIndexBuffer(), 0, m_pRenderSystem->GetFullLightIndexBuffer()->GetSize());
	m_ptrDescriptorSet->SetStorageBuffer(STORAGE_CULL_LIGHT_INDEX_DATA_NAME, m_pRenderSystem->GetCullLightIndexBuffer(), 0, m_pRenderSystem->GetCullLightIndexBuffer()->GetSize());

	m_ptrMesh = GfxRenderer()->NewMesh("PointLight.mesh", VERTEX_BINDING);
	m_ptrMeshDraw = GfxRenderer()->NewMeshDraw(m_ptrMesh->GetName(), m_ptrMesh, 0);
	m_ptrMaterialCullFaceBack = GfxRenderer()->NewMaterial("PassDeferredShading_Back.material", VERTEX_BINDING, INSTANCE_BINDING);
	m_ptrMaterialCullFaceFront = GfxRenderer()->NewMaterial("PassDeferredShading_Front.material", VERTEX_BINDING, INSTANCE_BINDING);
}

CGPUClusterCull::~CGPUClusterCull(void)
{

}

void CGPUClusterCull::SetCamera(CCamera* pCamera)
{
	m_pCamera = pCamera;
}

void CGPUClusterCull::Compute(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer)
{
	// Update
	m_pCamera->Apply();

	// Update DescriptorSet
	m_ptrDescriptorSet->SetUniformBuffer(UNIFORM_CAMERA_NAME, m_pCamera->GetUniformBuffer(), m_pCamera->GetUniformBufferOffset(), m_pCamera->GetUniformBufferSize());

	// Update Buffer
	const eastl::vector<int>& instnaces0 = m_pCamera->GetRenderQueue()->GetInstanceBuffer(m_ptrMaterialCullFaceFront, m_ptrMeshDraw);
	const eastl::vector<int>& instnaces1 = m_pCamera->GetRenderQueue()->GetInstanceBuffer(m_ptrMaterialCullFaceBack, m_ptrMeshDraw);

	eastl::vector<int> instance;
	instance.insert(instance.end(), instnaces0.begin(), instnaces0.end());
	instance.insert(instance.end(), instnaces1.begin(), instnaces1.end());

	m_pRenderSystem->GetFullLightIndexBuffer()->BufferData(0, sizeof(int) * std::min((int)instance.size(), MAX_GPUSCENE_INSTANCE_COUNT), instance.data());

	// Compute
	GfxRenderer()->CmdPushDebugGroup(ptrCommandBuffer, "ClusterCull");
	{
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_pRenderSystem->GetClusterBuffer(), GFX_ACCESS_TRANSFER_READ_BIT, GFX_ACCESS_TRANSFER_WRITE_BIT);
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_pRenderSystem->GetCullLightIndexBuffer(), GFX_ACCESS_TRANSFER_READ_BIT, GFX_ACCESS_TRANSFER_WRITE_BIT);
		{
			GfxRenderer()->CmdBindPipelineCompute(ptrCommandBuffer, m_pPipelineCompute);
			GfxRenderer()->CmdBindDescriptorSet(ptrCommandBuffer, m_ptrDescriptorSet);
			GfxRenderer()->CmdUniform1i(ptrCommandBuffer, HashValue("Param.numPointLights"), std::min((int)instance.size(), MAX_GPUSCENE_INSTANCE_COUNT));
			GfxRenderer()->CmdDispatch(ptrCommandBuffer, 1, 1, 1);
		}
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_pRenderSystem->GetCullLightIndexBuffer(), GFX_ACCESS_TRANSFER_WRITE_BIT, GFX_ACCESS_TRANSFER_READ_BIT);
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_pRenderSystem->GetClusterBuffer(), GFX_ACCESS_TRANSFER_WRITE_BIT, GFX_ACCESS_TRANSFER_READ_BIT);
	}
	GfxRenderer()->CmdPopDebugGroup(ptrCommandBuffer);
}
