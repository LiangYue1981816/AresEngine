#include "VKRenderer.h"


CVKRenderer::CVKRenderer(void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format)
	: CGfxRenderer(hInstance, hWnd, hDC, width, height, format)
	, m_pInstance(nullptr)
	, m_pDevice(nullptr)
{
	m_pInstance = new CVKInstance(hInstance, hWnd);
	m_pDevice = new CVKDevice(m_pInstance);
}

CVKRenderer::~CVKRenderer(void)
{
	delete m_pDevice;
	delete m_pInstance;
}

uint32_t CVKRenderer::GetLastError(void) const
{
	return m_pInstance->GetLastError();
}

CGfxSwapChain* CVKRenderer::GetSwapChain(void) const
{
	return nullptr;
}

bool CVKRenderer::IsSupportExtension(const char* extension) const
{
	return false;
}

CGfxShader* CVKRenderer::CreateShader(const char* szFileName, shader_kind kind)
{
	return nullptr;
}

CGfxPipelineCompute* CVKRenderer::CreatePipelineCompute(const CGfxShader* pComputeShader)
{
	return nullptr;
}

CGfxPipelineGraphics* CVKRenderer::CreatePipelineGraphics(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding)
{
	return nullptr;
}

CGfxSampler* CVKRenderer::CreateSampler(GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode)
{
	return nullptr;
}

CGfxFrameBufferPtr CVKRenderer::NewFrameBuffer(int width, int height, int numAttachments)
{
	return nullptr;
}

CGfxRenderPassPtr CVKRenderer::GetRenderPass(uint32_t name)
{
	return nullptr;
}

CGfxRenderPassPtr CVKRenderer::NewRenderPass(uint32_t name, int numAttachments, int numSubpasses)
{
	return nullptr;
}

CGfxTexture2DPtr CVKRenderer::GetTexture2D(uint32_t name)
{
	return nullptr;
}

CGfxTexture2DPtr CVKRenderer::NewTexture2D(uint32_t name)
{
	return nullptr;
}

CGfxTexture2DPtr CVKRenderer::NewTexture2D(const char* szFileName, int baseLevel, int numLevels)
{
	return nullptr;
}

CGfxTexture2DArrayPtr CVKRenderer::GetTexture2DArray(uint32_t name)
{
	return nullptr;
}

CGfxTexture2DArrayPtr CVKRenderer::NewTexture2DArray(uint32_t name)
{
	return nullptr;
}

CGfxTexture2DArrayPtr CVKRenderer::NewTexture2DArray(const char* szFileName, int baseLevel, int numLevels)
{
	return nullptr;
}

CGfxTextureCubemapPtr CVKRenderer::GetTextureCubemap(uint32_t name)
{
	return nullptr;
}

CGfxTextureCubemapPtr CVKRenderer::NewTextureCubemap(uint32_t name)
{
	return nullptr;
}

CGfxTextureCubemapPtr CVKRenderer::NewTextureCubemap(const char* szFileName, int baseLevel, int numLevels)
{
	return nullptr;
}

CGfxRenderTexturePtr CVKRenderer::GetRenderTexture(uint32_t name)
{
	return nullptr;
}

CGfxRenderTexturePtr CVKRenderer::NewRenderTexture(uint32_t name)
{
	return nullptr;
}

CGfxUniformBufferPtr CVKRenderer::NewUniformBuffer(size_t size)
{
	return nullptr;
}

CGfxMeshPtr CVKRenderer::GetMesh(uint32_t name)
{
	return nullptr;
}

CGfxMeshPtr CVKRenderer::NewMesh(uint32_t name)
{
	return nullptr;
}

CGfxMeshPtr CVKRenderer::NewMesh(const char* szFileName, int vertexBinding)
{
	return nullptr;
}

CGfxMeshDrawPtr CVKRenderer::GetMeshDraw(uint32_t name)
{
	return nullptr;
}

CGfxMeshDrawPtr CVKRenderer::NewMeshDraw(uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding)
{
	return nullptr;
}

CGfxMaterialPtr CVKRenderer::GetMaterial(uint32_t name)
{
	return nullptr;
}

CGfxMaterialPtr CVKRenderer::NewMaterial(uint32_t name)
{
	return nullptr;
}

CGfxMaterialPtr CVKRenderer::NewMaterial(const char* szFileName, int vertexBinding, int instanceBinding, int baseLevel, int numLevels)
{
	return nullptr;
}

CGfxDescriptorLayoutPtr CVKRenderer::NewDescriptorLayout(uint32_t set)
{
	return nullptr;
}

CGfxDescriptorSetPtr CVKRenderer::NewDescriptorSet(const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
{
	return nullptr;
}

CGfxDescriptorSetPtr CVKRenderer::NewDescriptorSet(const CGfxPipelineGraphics* pPipelineGraphics, const CGfxFrameBuffer* pFrameBuffer, const CGfxRenderPass* pRenderPass, int indexSubpass)
{
	return nullptr;
}

CGfxCommandBufferPtr CVKRenderer::NewCommandBuffer(uint32_t pool, bool bMainCommandBuffer)
{
	return nullptr;
}

bool CVKRenderer::CmdBeginRenderPass(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass)
{
	return true;
}

bool CVKRenderer::CmdNextSubpass(CGfxCommandBufferPtr ptrCommandBuffer)
{
	return true;
}

bool CVKRenderer::CmdEndRenderPass(CGfxCommandBufferPtr ptrCommandBuffer)
{
	return true;
}

bool CVKRenderer::CmdBindPipelineCompute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineCompute* pPipelineCompute)
{
	return true;
}

bool CVKRenderer::CmdBindPipelineGraphics(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineGraphics* pPipelineGraphics)
{
	return true;
}

bool CVKRenderer::CmdBindDescriptorSet(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxDescriptorSetPtr ptrDescriptorSet)
{
	return true;
}

bool CVKRenderer::CmdUniform1i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0)
{
	return true;
}

bool CVKRenderer::CmdUniform2i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1)
{
	return true;
}

bool CVKRenderer::CmdUniform3i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1, int v2)
{
	return true;
}

bool CVKRenderer::CmdUniform4i(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int v0, int v1, int v2, int v3)
{
	return true;
}

bool CVKRenderer::CmdUniform1f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0)
{
	return true;
}

bool CVKRenderer::CmdUniform2f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1)
{
	return true;
}

bool CVKRenderer::CmdUniform3f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1, float v2)
{
	return true;
}

bool CVKRenderer::CmdUniform4f(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, float v0, float v1, float v2, float v3)
{
	return true;
}

bool CVKRenderer::CmdUniform1iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value)
{
	return true;
}

bool CVKRenderer::CmdUniform2iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value)
{
	return true;
}

bool CVKRenderer::CmdUniform3iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value)
{
	return true;
}

bool CVKRenderer::CmdUniform4iv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const int* value)
{
	return true;
}

bool CVKRenderer::CmdUniform1fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	return true;
}

bool CVKRenderer::CmdUniform2fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	return true;
}

bool CVKRenderer::CmdUniform3fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	return true;
}

bool CVKRenderer::CmdUniform4fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	return true;
}

bool CVKRenderer::CmdUniformMatrix2fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	return true;
}

bool CVKRenderer::CmdUniformMatrix3fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	return true;
}

bool CVKRenderer::CmdUniformMatrix4fv(CGfxCommandBufferPtr ptrCommandBuffer, uint32_t name, int count, const float* value)
{
	return true;
}

bool CVKRenderer::CmdSetScissor(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height)
{
	return true;
}

bool CVKRenderer::CmdSetViewport(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height)
{
	return true;
}

bool CVKRenderer::CmdClearDepth(CGfxCommandBufferPtr ptrCommandBuffer, float depth)
{
	return true;
}

bool CVKRenderer::CmdClearColor(CGfxCommandBufferPtr ptrCommandBuffer, float red, float green, float blue, float alpha)
{
	return true;
}

bool CVKRenderer::CmdDrawInstance(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size)
{
	return true;
}

bool CVKRenderer::CmdDrawIndirect(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size)
{
	return true;
}

bool CVKRenderer::CmdExecute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxCommandBufferPtr ptrSecondaryCommandBuffer)
{
	return true;
}

bool CVKRenderer::CmdPresent(CGfxCommandBufferPtr ptrCommandBuffer)
{
	return true;
}

bool CVKRenderer::CmdPushDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer, const char* szMessage)
{
	return true;
}

bool CVKRenderer::CmdPopDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer)
{
	return true;
}

void CVKRenderer::Submit(const eastl::vector<CGfxCommandBufferPtr>& ptrCommandBuffers)
{

}

void CVKRenderer::AcquireNextFrame(void)
{

}

void CVKRenderer::Present(void)
{

}

uint32_t CVKRenderer::GetFrameIndex(void) const
{
	return 0;
}
