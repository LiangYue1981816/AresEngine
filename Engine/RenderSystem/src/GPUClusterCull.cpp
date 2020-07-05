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
	m_ptrDescriptorSet->SetStorageBuffer(STORAGE_SCENE_DATA_NAME, m_pRenderSystem->GetInstanceDataBuffer(), 0, m_pRenderSystem->GetInstanceDataBuffer()->GetSize());
	m_ptrDescriptorSet->SetStorageBuffer(STORAGE_CLUSTER_DATA_NAME, m_pRenderSystem->GetClusterBuffer(), 0, m_pRenderSystem->GetClusterBuffer()->GetSize());
	m_ptrDescriptorSet->SetStorageBuffer(STORAGE_FULL_LIGHT_LIST_DATA_NAME, m_pRenderSystem->GetFullLightListBuffer(), 0, m_pRenderSystem->GetFullLightListBuffer()->GetSize());
	m_ptrDescriptorSet->SetStorageBuffer(STORAGE_CULL_LIGHT_LIST_DATA_NAME, m_pRenderSystem->GetCullLightListBuffer(), 0, m_pRenderSystem->GetCullLightListBuffer()->GetSize());
}

CGPUClusterCull::~CGPUClusterCull(void)
{

}

void CGPUClusterCull::SetCamera(CCamera* pCamera)
{
	if (m_pCamera != pCamera) {
		m_pCamera = pCamera;
		m_ptrDescriptorSet->SetUniformBuffer(UNIFORM_CAMERA_NAME, pCamera->GetCameraUniform()->GetUniformBuffer(), 0, pCamera->GetCameraUniform()->GetUniformBuffer()->GetSize());
	}
}

void CGPUClusterCull::Compute(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer)
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

	m_pRenderSystem->GetFullLightListBuffer()->BufferData(0, sizeof(int) * std::min((int)instance.size(), MAX_GPUSCENE_INSTANCE_COUNT), instance.data());

	// Compute
	GfxRenderer()->CmdPushDebugGroup(ptrCommandBuffer, "ClusterCull");
	{
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_pRenderSystem->GetClusterBuffer(), GFX_ACCESS_TRANSFER_READ_BIT, GFX_ACCESS_TRANSFER_WRITE_BIT);
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_pRenderSystem->GetCullLightListBuffer(), GFX_ACCESS_TRANSFER_READ_BIT, GFX_ACCESS_TRANSFER_WRITE_BIT);
		{
			GfxRenderer()->CmdBindPipelineCompute(ptrCommandBuffer, m_pPipelineCompute);
			GfxRenderer()->CmdBindDescriptorSet(ptrCommandBuffer, m_ptrDescriptorSet);
			GfxRenderer()->CmdUniform1i(ptrCommandBuffer, HashValue("Param.numPointLights"), std::min((int)instance.size(), MAX_GPUSCENE_INSTANCE_COUNT));
			GfxRenderer()->CmdDispatch(ptrCommandBuffer, 1, 1, 1);
		}
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_pRenderSystem->GetCullLightListBuffer(), GFX_ACCESS_TRANSFER_WRITE_BIT, GFX_ACCESS_TRANSFER_READ_BIT);
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_pRenderSystem->GetClusterBuffer(), GFX_ACCESS_TRANSFER_WRITE_BIT, GFX_ACCESS_TRANSFER_READ_BIT);
	}
	GfxRenderer()->CmdPopDebugGroup(ptrCommandBuffer);
}
