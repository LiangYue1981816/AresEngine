#include "EngineHeader.h"
#include "RenderHeader.h"


CGPUCluster::CGPUCluster(CRenderSystem* pRenderSystem)
	: m_pRenderSystem(pRenderSystem)
	, m_pCamera(nullptr)

	, m_pShaderCompute(nullptr)
	, m_pPipelineCompute(nullptr)
{
	char szFileName[] = "GPU_Cluster.glsl";
	char szBinFileName[_MAX_STRING] = { 0 };

	sprintf(szBinFileName, "%x.comp", HashValue(szFileName));
	ShaderCompiler()->Compile(FileManager()->GetFullName(szFileName), szBinFileName, shaderc_compute_shader);

	m_pShaderCompute = GfxRenderer()->CreateShader(szBinFileName, compute_shader);
	m_pPipelineCompute = GfxRenderer()->CreatePipelineCompute(m_pShaderCompute);

	m_ptrDescriptorSet = GfxRenderer()->NewDescriptorSet(HashValue(szFileName), m_pPipelineCompute->GetDescriptorLayout(DESCRIPTOR_SET_PASS));
	m_ptrDescriptorSet->SetStorageBuffer(STORAGE_CLUSTER_DATA_NAME, m_pRenderSystem->GetClusterBuffer(), 0, m_pRenderSystem->GetClusterBuffer()->GetSize());
}

CGPUCluster::~CGPUCluster(void)
{

}

void CGPUCluster::SetCamera(CCamera* pCamera)
{
	if (m_pCamera != pCamera) {
		m_pCamera = pCamera;
		m_ptrDescriptorSet->SetUniformBuffer(UNIFORM_CAMERA_NAME, pCamera->GetCameraUniform()->GetUniformBuffer(), 0, pCamera->GetCameraUniform()->GetUniformBuffer()->GetSize());
	}
}

void CGPUCluster::Compute(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer)
{
	// Update
	m_pCamera->GetCameraUniform()->Apply();

	// Compute
	GfxRenderer()->CmdPushDebugGroup(ptrCommandBuffer, "Cluster");
	{
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_pRenderSystem->GetClusterBuffer(), GFX_ACCESS_TRANSFER_READ_BIT, GFX_ACCESS_TRANSFER_WRITE_BIT);
		{
			GfxRenderer()->CmdBindPipelineCompute(ptrCommandBuffer, m_pPipelineCompute);
			GfxRenderer()->CmdBindDescriptorSet(ptrCommandBuffer, m_ptrDescriptorSet);
			GfxRenderer()->CmdDispatch(ptrCommandBuffer, CLUSTER_WIDTH_TILE_COUNT, CLUSTER_HEIGHT_TILE_COUNT, CLUSTER_DEPTH_TILE_COUNT);
		}
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_pRenderSystem->GetClusterBuffer(), GFX_ACCESS_TRANSFER_WRITE_BIT, GFX_ACCESS_TRANSFER_READ_BIT);
	}
	GfxRenderer()->CmdPopDebugGroup(ptrCommandBuffer);
}
