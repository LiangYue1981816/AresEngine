#pragma once
#include "GfxHeader.h"

#include "GLES3/GL31.h"
#include "GLES3Helper.h"
#include "GLES3Extension.h"
#include "GLES3Definition.h"

#include "GLES3IndexBuffer.h"
#include "GLES3VertexBuffer.h"
#include "GLES3InstanceBuffer.h"
#include "GLES3IndirectBuffer.h"
#include "GLES3VertexArrayObject.h"
#include "GLES3Sampler.h"
#include "GLES3Texture.h"
#include "GLES3Texture2D.h"
#include "GLES3Texture2DArray.h"
#include "GLES3TextureCubeMap.h"
#include "GLES30Shader.h"
#include "GLES31Shader.h"
#include "GLES30Pipeline.h"
#include "GLES31Pipeline.h"
#include "GLES30PipelineCompute.h"
#include "GLES31PipelineCompute.h"
#include "GLES30PipelineGraphics.h"
#include "GLES31PipelineGraphics.h"

#include "GLES3SwapChain.h"
#include "GLES3RenderPass.h"
#include "GLES3FrameBuffer.h"
#include "GLES3RenderTexture.h"
#include "GLES3Mesh.h"
#include "GLES3MeshDraw.h"
#include "GLES3Material.h"
#include "GLES3MaterialPass.h"
#include "GLES3UniformBuffer.h"
#include "GLES3CommandBuffer.h"

#include "GLES3SamplerManager.h"
#include "GLES3TextureManager.h"
#include "GLES3ShaderManager.h"
#include "GLES3PipelineManager.h"
#include "GLES3RenderPassManager.h"
#include "GLES3FrameBufferManager.h"
#include "GLES3MeshManager.h"
#include "GLES3MeshDrawManager.h"
#include "GLES3MaterialManager.h"
#include "GLES3RenderTextureManager.h"
#include "GLES3UniformBufferManager.h"
#include "GLES3CommandBufferManager.h"


#define GLES3Renderer() ((CGLES3Renderer *)CGfxRenderer::GetInstance())


class CALL_API CGLES3Renderer : public CGfxRenderer
{
	friend class CGLES3RenderPass;
	friend class CGLES3FrameBuffer;
	friend class CGLES3Mesh;
	friend class CGLES3Texture2D;
	friend class CGLES3Texture2DArray;
	friend class CGLES3TextureCubeMap;
	friend class CGLES3Material;
	friend class CGLES3MaterialPass;

	friend class CGLES3CommandBindMeshDraw;
	friend class CGLES3CommandBindSubpassInputTexture;
	friend class CGLES3CommandBindPipelineCompute;
	friend class CGLES3CommandBindPipelineGraphics;
	friend class CGLES3CommandBindMaterialPass;
	friend class CGLES3CommandBindUniformBuffer;
	friend class CGLES3CommandUniform1i;
	friend class CGLES3CommandUniform2i;
	friend class CGLES3CommandUniform3i;
	friend class CGLES3CommandUniform4i;
	friend class CGLES3CommandUniform1f;
	friend class CGLES3CommandUniform2f;
	friend class CGLES3CommandUniform3f;
	friend class CGLES3CommandUniform4f;
	friend class CGLES3CommandUniform1iv;
	friend class CGLES3CommandUniform2iv;
	friend class CGLES3CommandUniform3iv;
	friend class CGLES3CommandUniform4iv;
	friend class CGLES3CommandUniform1fv;
	friend class CGLES3CommandUniform2fv;
	friend class CGLES3CommandUniform3fv;
	friend class CGLES3CommandUniform4fv;
	friend class CGLES3CommandUniformMatrix2fv;
	friend class CGLES3CommandUniformMatrix3fv;
	friend class CGLES3CommandUniformMatrix4fv;
	friend class CGLES3CommandDrawElements;
	friend class CGLES3CommandDrawInstance;
	friend class CGLES3CommandDrawIndirect;


public:
	CGLES3Renderer(void *hInstance, void *hWnd, void *hDC, int width, int height, GfxPixelFormat pixelFormat);
	virtual ~CGLES3Renderer(void);


public:
	uint32_t GetLastError(void) const;

public:
	CGfxSwapChain* GetSwapChain(void) const;
	CGfxMaterialPass* GetGlobalMaterialPass(void) const;
	CGfxMaterialPass* GetCurrentMaterialPass(void) const;

public:
	bool IsSupportExtension(const char *extension) const;

public:
	CGfxShader* CreateShader(const char *szFileName, shader_kind kind);
	CGfxPipelineCompute* CreatePipelineCompute(const CGfxShader *pComputeShader);
	CGfxPipelineGraphics* CreatePipelineGraphics(const CGfxRenderPass *pRenderPass, const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state, uint32_t indexSubpass, uint32_t vertexBinding = 0, uint32_t instanceBinding = 1);
	CGfxSampler* CreateSampler(GfxFilter minFilter, GfxFilter magFilter, GfxSamplerMipmapMode mipmapMode, GfxSamplerAddressMode addressMode);

public:
	CGfxFrameBufferPtr NewFrameBuffer(int width, int height, int numAttachments);

	CGfxRenderPassPtr GetRenderPass(uint32_t name);
	CGfxRenderPassPtr NewRenderPass(uint32_t name, int numAttachments, int numSubpasses);

	CGfxRenderTexturePtr GetRenderTexture(uint32_t name);
	CGfxRenderTexturePtr NewRenderTexture(uint32_t name);

	CGfxMeshPtr GetMesh(uint32_t name);
	CGfxMeshPtr NewMesh(uint32_t name);
	CGfxMeshPtr NewMesh(const char *szFileName, uint32_t vertexBinding = 0);

	CGfxMeshDrawPtr GetMeshDraw(uint32_t name);
	CGfxMeshDrawPtr NewMeshDraw(uint32_t name, const CGfxMeshPtr ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding = 1);

	CGfxMaterialPtr GetMaterial(uint32_t name);
	CGfxMaterialPtr NewMaterial(uint32_t name);
	CGfxMaterialPtr NewMaterial(const char *szFileName, uint32_t vertexBinding = 0, uint32_t instanceBinding = 1);

	CGfxTexture2DPtr GetTexture2D(uint32_t name);
	CGfxTexture2DPtr NewTexture2D(uint32_t name);
	CGfxTexture2DPtr NewTexture2D(const char *szFileName);

	CGfxTexture2DArrayPtr GetTexture2DArray(uint32_t name);
	CGfxTexture2DArrayPtr NewTexture2DArray(uint32_t name);
	CGfxTexture2DArrayPtr NewTexture2DArray(const char *szFileName);

	CGfxTextureCubeMapPtr GetTextureCubeMap(uint32_t name);
	CGfxTextureCubeMapPtr NewTextureCubeMap(uint32_t name);
	CGfxTextureCubeMapPtr NewTextureCubeMap(const char *szFileName);

	CGfxUniformBufferPtr NewUniformBuffer(size_t size);
	CGfxCommandBufferPtr NewCommandBuffer(bool bMainCommandBuffer);

public:
	bool CmdBeginRenderPass(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass);
	bool CmdNextSubpass(CGfxCommandBufferPtr ptrCommandBuffer);
	bool CmdEndRenderPass(CGfxCommandBufferPtr ptrCommandBuffer);
	bool CmdBindPipelineCompute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineCompute *pPipelineCompute);
	bool CmdBindPipelineGraphics(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxPipelineGraphics *pPipelineGraphics);
	bool CmdBindMaterialPass(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMaterialPtr ptrMaterial, uint32_t namePass);
	bool CmdBindUniformBuffer(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxUniformBufferPtr ptrUniformBuffer, uint32_t nameUniform);
	bool CmdUniform1i(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int v0);
	bool CmdUniform2i(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int v0, int v1);
	bool CmdUniform3i(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int v0, int v1, int v2);
	bool CmdUniform4i(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int v0, int v1, int v2, int v3);
	bool CmdUniform1f(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, float v0);
	bool CmdUniform2f(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, float v0, float v1);
	bool CmdUniform3f(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, float v0, float v1, float v2);
	bool CmdUniform4f(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, float v0, float v1, float v2, float v3);
	bool CmdUniform1iv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const int *value);
	bool CmdUniform2iv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const int *value);
	bool CmdUniform3iv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const int *value);
	bool CmdUniform4iv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const int *value);
	bool CmdUniform1fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniform2fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniform3fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniform4fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniformMatrix2fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniformMatrix3fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniformMatrix4fv(CGfxCommandBufferPtr ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdClearDepth(CGfxCommandBufferPtr ptrCommandBuffer, float depth);
	bool CmdClearColor(CGfxCommandBufferPtr ptrCommandBuffer, float red, float green, float blue, float alpha);
	bool CmdSetScissor(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height);
	bool CmdSetViewport(CGfxCommandBufferPtr ptrCommandBuffer, int x, int y, int width, int height);
	bool CmdSetInstanceBufferData(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t *pInstanceBuffer, uint32_t size);
	bool CmdDrawInstance(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw, int instanceCount);
	bool CmdDrawIndirect(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxMeshDrawPtr ptrMeshDraw);
	bool CmdDrawScreen(CGfxCommandBufferPtr ptrCommandBuffer);
	bool CmdExecute(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxCommandBufferPtr ptrSecondaryCommandBuffer);
	bool CmdPresent(CGfxCommandBufferPtr ptrCommandBuffer);
	bool CmdPushDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer, const char *szMessage);
	bool CmdPopDebugGroup(CGfxCommandBufferPtr ptrCommandBuffer);

public:
	void Submit(const CGfxCommandBufferPtr ptrCommandBuffer);
	void Present(void);

private:
	void BindMeshDraw(CGfxMeshDraw *pMeshDraw);
	void BindPipelineCompute(CGfxPipelineCompute *pPipelineCompute);
	void BindPipelineGraphics(CGfxPipelineGraphics *pPipelineGraphics);
	void BindUniformBuffer(CGfxUniformBuffer *pUniformBuffer, uint32_t nameUniform);
	void BindMaterialPass(CGfxMaterialPass *pPass);
	void BindInputTexture(const char *szName, CGfxRenderTexture *pTexture);

private:
	CGLES3PipelineCompute* GetCurrentPipelineCompute(void) const;
	CGLES3PipelineGraphics* GetCurrentPipelineGraphics(void) const;


private:
	CGLES3SwapChain *m_pSwapChain;

private:
	CGLES3MeshManager *m_pMeshManager;
	CGLES3MeshDrawManager *m_pMeshDrawManager;
	CGLES3ShaderManager *m_pShaderManager;
	CGLES3SamplerManager *m_pSamplerManager;
	CGLES3Texture2DManager *m_pTexture2DManager;
	CGLES3Texture2DArrayManager *m_pTexture2DArrayManager;
	CGLES3TextureCubeMapManager *m_pTextureCubeMapManager;
	CGLES3PipelineComputeManager *m_pPipelineComputeManager;
	CGLES3PipelineGraphicsManager *m_pPipelineGraphicsManager;
	CGLES3MaterialManager *m_pMaterialManager;
	CGLES3RenderPassManager *m_pRenderPassManager;
	CGLES3FrameBufferManager *m_pFrameBufferManager;
	CGLES3RenderTextureManager *m_pRenderTextureManager;
	CGLES3UniformBufferManager *m_pUniformBufferManager;
	CGLES3CommandBufferManager *m_pCommandBufferManager;

private:
	CGLES3MaterialPass *m_pGlobalMaterialPass;
	CGLES3MaterialPass *m_pCurrentMaterialPass;
	CGLES3PipelineCompute *m_pCurrentPipelineCompute;
	CGLES3PipelineGraphics *m_pCurrentPipelineGraphics;
};
