#include "EngineHeader.h"
#include "RenderHeader.h"


CGPUEyeHistogram::CGPUEyeHistogram(CRenderSystem* pRenderSystem)
	: CPassBase(pRenderSystem)
	, m_pShaderCompute(nullptr)
	, m_pPipelineCompute(nullptr)
{
	char szFileName[] = "GPU_EyeHistogram.glsl";
	char szBinFileName[_MAX_STRING] = { 0 };

	sprintf(szBinFileName, "%x.comp", HashValue(szFileName));
	ShaderCompiler()->Compile(FileManager()->GetFullName(szFileName), szBinFileName, shaderc_compute_shader);

	m_pShaderCompute = GfxRenderer()->CreateShader(szBinFileName, compute_shader);
	m_pPipelineCompute = GfxRenderer()->CreatePipelineCompute(m_pShaderCompute);

	m_ptrDescriptorSet = GfxRenderer()->NewDescriptorSet(HashValue(szFileName), m_pPipelineCompute->GetDescriptorLayout(DESCRIPTOR_SET_PASS));
	m_ptrDescriptorSet->SetStorageBuffer(STORAGE_HISTOGRAM_DATA_NAME, m_pRenderSystem->GetHistogramBuffer(), 0, m_pRenderSystem->GetHistogramBuffer()->GetSize());
}

CGPUEyeHistogram::~CGPUEyeHistogram(void)
{

}

void CGPUEyeHistogram::SetInputTexture(CGfxRenderTexturePtr ptrColorTexture)
{
	if (m_ptrInputColorTexture != ptrColorTexture) {
		m_ptrInputColorTexture  = ptrColorTexture;
		m_ptrDescriptorSet->SetImageRenderTexture(UNIFORM_COLOR_IMAGE_NAME, ptrColorTexture);
	}
}

void CGPUEyeHistogram::Compute(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer)
{
	// Update Buffer
	static const int data[HISTOGRAM_SIZE] = { 0 };
	m_pRenderSystem->GetHistogramBuffer()->BufferData(0, m_pRenderSystem->GetHistogramBuffer()->GetSize(), data);

	// Compute
	GfxRenderer()->CmdPushDebugGroup(ptrCommandBuffer, "EyeHistogram");
	{
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_pRenderSystem->GetHistogramBuffer(), GFX_ACCESS_TRANSFER_READ_BIT, GFX_ACCESS_TRANSFER_WRITE_BIT);
		{
			GfxRenderer()->CmdBindPipelineCompute(ptrCommandBuffer, m_pPipelineCompute);
			GfxRenderer()->CmdBindDescriptorSet(ptrCommandBuffer, m_ptrDescriptorSet);
			GfxRenderer()->CmdDispatch(ptrCommandBuffer, m_ptrInputColorTexture->GetWidth() / HISTOGRAM_WORKGROUP_SIZE, m_ptrInputColorTexture->GetHeight() / HISTOGRAM_WORKGROUP_SIZE, 1);
		}
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_pRenderSystem->GetHistogramBuffer(), GFX_ACCESS_TRANSFER_WRITE_BIT, GFX_ACCESS_TRANSFER_READ_BIT);
	}
	GfxRenderer()->CmdPopDebugGroup(ptrCommandBuffer);
}
