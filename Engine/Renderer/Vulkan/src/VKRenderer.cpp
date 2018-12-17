#include "VKRenderer.h"


CVKRenderer::CVKRenderer(void *hInstance, void *hWnd, void *hDC, int width, int height, GfxPixelFormat pixelFormat)
	: CGfxRenderer(hInstance, hWnd, hDC, width, height, pixelFormat)
{

}

CVKRenderer::~CVKRenderer(void)
{

}

uint32_t CVKRenderer::GetLastError(void) const
{
	return 0;
}

CGfxSwapChain* CVKRenderer::GetSwapChain(void) const
{
	return nullptr;
}

CGfxMaterialPass* CVKRenderer::GetGlobalPass(void) const
{
	return nullptr;
}

CGfxShader* CVKRenderer::CreateShader(const char *szFileName, shader_kind kind)
{
	return nullptr;
}

CGfxPipelineCompute* CVKRenderer::CreatePipelineCompute(const CGfxShader *pComputeShader)
{
	return nullptr;
}

CGfxPipelineGraphics* CVKRenderer::CreatePipelineGraphics(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state)
{
	return nullptr;
}

CGfxSampler* CVKRenderer::CreateSampler(GfxMinFilter minFilter, GfxMagFilter magFilter, GfxAddressMode addressMode)
{
	return nullptr;
}

CGfxRenderPassPtr CVKRenderer::NewRenderPass(int numAttachments, int numSubpasses)
{
	return nullptr;
}

CGfxFrameBufferPtr CVKRenderer::NewFrameBuffer(int width, int height)
{
	return nullptr;
}

bool CVKRenderer::IsHaveMesh(uint32_t name)
{
	return false;
}

CGfxMeshPtr CVKRenderer::NewMesh(uint32_t name)
{
	return nullptr;
}

CGfxMeshPtr CVKRenderer::NewMesh(const char *szFileName, uint32_t vertexBinding)
{
	return nullptr;
}

bool CVKRenderer::IsHaveMeshDraw(uint32_t name)
{
	return false;
}

CGfxMeshDrawPtr CVKRenderer::NewMeshDraw(uint32_t name, const CGfxMeshPtr &ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding)
{
	return nullptr;
}

bool CVKRenderer::IsHaveMaterial(uint32_t name)
{
	return false;
}

CGfxMaterialPtr CVKRenderer::NewMaterial(uint32_t name)
{
	return nullptr;
}

CGfxMaterialPtr CVKRenderer::NewMaterial(const char *szFileName)
{
	return nullptr;
}

bool CVKRenderer::IsHaveTexture2D(uint32_t name)
{
	return false;
}

bool CVKRenderer::IsHaveTexture2DArray(uint32_t name)
{
	return false;
}

bool CVKRenderer::IsHaveTextureCubeMap(uint32_t name)
{
	return false;
}

CGfxTexture2DPtr CVKRenderer::NewTexture2D(uint32_t name)
{
	return nullptr;
}

CGfxTexture2DPtr CVKRenderer::NewTexture2D(const char *szFileName)
{
	return nullptr;
}

CGfxTexture2DArrayPtr CVKRenderer::NewTexture2DArray(uint32_t name)
{
	return nullptr;
}

CGfxTexture2DArrayPtr CVKRenderer::NewTexture2DArray(const char *szFileName)
{
	return nullptr;
}

CGfxTextureCubeMapPtr CVKRenderer::NewTextureCubeMap(uint32_t name)
{
	return nullptr;
}

CGfxTextureCubeMapPtr CVKRenderer::NewTextureCubeMap(const char *szFileName)
{
	return nullptr;
}

CGfxUniformBufferPtr CVKRenderer::NewUniformBuffer(size_t size, bool bDynamic)
{
	return nullptr;
}

CGfxCommandBufferPtr CVKRenderer::NewCommandBuffer(bool bMainCommandBuffer)
{
	return nullptr;
}

bool CVKRenderer::CmdBeginRenderPass(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass)
{
	return false;
}

bool CVKRenderer::CmdNextSubpass(CGfxCommandBufferPtr &ptrCommandBuffer)
{
	return false;
}

bool CVKRenderer::CmdEndRenderPass(CGfxCommandBufferPtr &ptrCommandBuffer)
{
	return false;
}

bool CVKRenderer::CmdBindPipelineCompute(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxPipelineCompute *pPipelineCompute)
{
	return false;
}

bool CVKRenderer::CmdBindPipelineGraphics(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxPipelineGraphics *pPipelineGraphics)
{
	return false;
}

bool CVKRenderer::CmdBindMaterialPass(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMaterialPtr &ptrMaterial, uint32_t namePass)
{
	return false;
}

bool CVKRenderer::CmdBindUniformBuffer(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxUniformBufferPtr &ptrUniformBuffer, uint32_t nameUniform)
{
	return false;
}

bool CVKRenderer::CmdUniform1i(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int v0)
{
	return false;
}

bool CVKRenderer::CmdUniform2i(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int v0, int v1)
{
	return false;
}

bool CVKRenderer::CmdUniform3i(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int v0, int v1, int v2)
{
	return false;
}

bool CVKRenderer::CmdUniform4i(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int v0, int v1, int v2, int v3)
{
	return false;
}

bool CVKRenderer::CmdUniform1f(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, float v0)
{
	return false;
}

bool CVKRenderer::CmdUniform2f(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, float v0, float v1)
{
	return false;
}

bool CVKRenderer::CmdUniform3f(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, float v0, float v1, float v2)
{
	return false;
}

bool CVKRenderer::CmdUniform4f(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, float v0, float v1, float v2, float v3)
{
	return false;
}

bool CVKRenderer::CmdUniform1iv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const int *value)
{
	return false;
}

bool CVKRenderer::CmdUniform2iv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const int *value)
{
	return false;
}

bool CVKRenderer::CmdUniform3iv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const int *value)
{
	return false;
}

bool CVKRenderer::CmdUniform4iv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const int *value)
{
	return false;
}

bool CVKRenderer::CmdUniform1fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return false;
}

bool CVKRenderer::CmdUniform2fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return false;
}

bool CVKRenderer::CmdUniform3fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return false;
}

bool CVKRenderer::CmdUniform4fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return false;
}

bool CVKRenderer::CmdUniformMatrix2fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return false;
}

bool CVKRenderer::CmdUniformMatrix3fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return false;
}

bool CVKRenderer::CmdUniformMatrix4fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value)
{
	return false;
}

bool CVKRenderer::CmdClearDepth(CGfxCommandBufferPtr &ptrCommandBuffer, float depth)
{
	return false;
}

bool CVKRenderer::CmdClearColor(CGfxCommandBufferPtr &ptrCommandBuffer, float red, float green, float blue, float alpha)
{
	return false;
}

bool CVKRenderer::CmdSetScissor(CGfxCommandBufferPtr &ptrCommandBuffer, int x, int y, int width, int height)
{
	return false;
}

bool CVKRenderer::CmdSetViewport(CGfxCommandBufferPtr &ptrCommandBuffer, int x, int y, int width, int height)
{
	return false;
}

bool CVKRenderer::CmdSetInstanceBufferData(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMeshDrawPtr &ptrMeshDraw, const uint8_t *pInstanceBuffer, uint32_t size)
{
	return false;
}

bool CVKRenderer::CmdDrawInstance(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMeshDrawPtr &ptrMeshDraw, int instanceCount)
{
	return false;
}

bool CVKRenderer::CmdDrawIndirect(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMeshDrawPtr &ptrMeshDraw)
{
	return false;
}

bool CVKRenderer::CmdDrawScreen(CGfxCommandBufferPtr &ptrCommandBuffer)
{
	return false;
}

bool CVKRenderer::CmdExecute(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxCommandBufferPtr &ptrSecondaryCommandBuffer)
{
	return false;
}

bool CVKRenderer::CmdPresent(CGfxCommandBufferPtr &ptrCommandBuffer)
{
	return false;
}

void CVKRenderer::Submit(const CGfxCommandBufferPtr &ptrCommandBuffer)
{

}

void CVKRenderer::Present(void)
{

}