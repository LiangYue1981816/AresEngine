#include "EngineHeader.h"
#include "RenderHeader.h"


CCluster::CCluster(int screenWidth, int screenHeight, int tileSize, int numSlices)
	: m_pCamera(nullptr)

	, m_pShaderCompute(nullptr)
	, m_pPipelineCompute(nullptr)
{
	char szFileName[] = "Cluster.glsl";
	char szBinFileName[_MAX_STRING] = { 0 };

	sprintf(szBinFileName, "%x.comp", HashValue(szFileName));
	ShaderCompiler()->Compile(FileManager()->GetFullName(szFileName), szBinFileName, shaderc_compute_shader);

	m_pShaderCompute = GfxRenderer()->CreateShader(szBinFileName, compute_shader);
	m_pPipelineCompute = GfxRenderer()->CreatePipelineCompute(m_pShaderCompute);

	m_ptrClusterBuffer = GfxRenderer()->NewStorageBuffer(4 * 1024 * 1024);
	m_ptrLightListBuffer = GfxRenderer()->NewStorageBuffer(1 * 1024 * 1024);
}

CCluster::~CCluster(void)
{

}

const CGfxStorageBufferPtr CCluster::GetLightListBuffer(void) const
{
	return m_ptrLightListBuffer;
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
}
