#pragma once
#include "GfxRenderer.h"

#include "GLES3Extension.h"
#include "GLES3Definition.h"

#include "GLES3SwapChain.h"

#include "GLES3RenderPass.h"
#include "GLES3RenderPassManager.h"

#include "GLES3FrameBuffer.h"
#include "GLES3FrameBufferManager.h"

#include "GLES3Mesh.h"
#include "GLES3MeshManager.h"

#include "GLES3Sampler.h"
#include "GLES3SamplerManager.h"

#include "GLES3TextureBase.h"
#include "GLES3Texture2D.h"
#include "GLES3Texture2DArray.h"
#include "GLES3TextureCubeMap.h"
#include "GLES3TextureManager.h"

#include "GLES3Shader.h"
#include "GLES3ShaderManager.h"

#include "GLES3PipelineBase.h"
#include "GLES3PipelineCompute.h"
#include "GLES3PipelineGraphics.h"
#include "GLES3PipelineManager.h"

#include "GLES3Material.h"
#include "GLES3MaterialPass.h"
#include "GLES3MaterialManager.h"

#include "GLES3UniformBase.h"
#include "GLES3UniformVec1.h"
#include "GLES3UniformVec2.h"
#include "GLES3UniformVec3.h"
#include "GLES3UniformVec4.h"
#include "GLES3UniformMat4.h"
#include "GLES3UniformEngine.h"
#include "GLES3UniformCamera.h"
#include "GLES3UniformBuffer.h"
#include "GLES3UniformManager.h"

#include "GLES3CommandBuffer.h"
#include "GLES3CommandBufferManager.h"


#define GLES3Renderer() ((CGLES3Renderer *)CGfxRenderer::GetInstance())


class CGLES3Renderer : public CGfxRenderer
{
	friend class CGfxRenderer;

	friend class CGLES3RenderPass;
	friend class CGLES3FrameBuffer;
	friend class CGLES3Mesh;
	friend class CGLES3Texture2D;
	friend class CGLES3Texture2DArray;
	friend class CGLES3TextureCubeMap;
	friend class CGLES3Material;
	friend class CGLES3MaterialPass;

	friend class CGLES3CommandBindSubPassInputTexture;
	friend class CGLES3CommandBindPipelineCompute;
	friend class CGLES3CommandBindPipelineGraphics;
	friend class CGLES3CommandBindMaterialPass;
	friend class CGLES3CommandBindEngine;
	friend class CGLES3CommandBindCamera;
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


private:
	CGLES3Renderer(void *hDC, int width, int height, uint32_t format);
	virtual ~CGLES3Renderer(void);


public:
	CGfxSwapChain* GetSwapChain(void) const;

private:
	CGLES3Shader* CreateShader(const char *szFileName, shaderc_shader_kind kind);
	CGLES3PipelineCompute* CreatePipelineCompute(const CGfxShader *pComputeShader);
	CGLES3PipelineGraphics* CreatePipelineGraphics(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state);
	CGLES3Sampler* CreateSampler(uint32_t minFilter, uint32_t magFilter, uint32_t addressMode);

public:
	CGfxRenderPassPtr NewRenderPass(int numAttachments, int numSubpasses);
	CGfxFrameBufferPtr NewFrameBuffer(int width, int height);

	CGfxMaterialPtr NewMaterial(const char *szFileName);

	CGfxMeshPtr NewMesh(uint32_t name, uint32_t instanceFormat);
	CGfxMeshPtr NewMesh(const char *szFileName, uint32_t instanceFormat);

	CGfxTexture2DPtr NewTexture2D(uint32_t name);
	CGfxTexture2DPtr NewTexture2D(const char *szFileName);

	CGfxTexture2DArrayPtr NewTexture2DArray(uint32_t name);
	CGfxTexture2DArrayPtr NewTexture2DArray(const char *szFileName);

	CGfxTextureCubeMapPtr NewTextureCubeMap(uint32_t name);
	CGfxTextureCubeMapPtr NewTextureCubeMap(const char *szFileName);

	CGfxUniformEnginePtr NewUniformEngine(bool bDynamic);
	CGfxUniformCameraPtr NewUniformCamera(bool bDynamic);

	CGfxCommandBufferPtr NewCommandBuffer(bool bMainCommandBuffer);

public:
	bool CmdBeginRenderPass(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass);
	bool CmdNextSubpass(CGfxCommandBufferPtr &ptrCommandBuffer);
	bool CmdEndRenderPass(CGfxCommandBufferPtr &ptrCommandBuffer);

	bool CmdBindPipelineCompute(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxPipelineCompute *pPipelineCompute);
	bool CmdBindPipelineGraphics(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxPipelineGraphics *pPipelineGraphics);
	bool CmdBindMaterialPass(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMaterialPtr &ptrMaterial, uint32_t namePass);
	bool CmdBindUniformEngine(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxUniformEnginePtr &ptrUniformEngine);
	bool CmdBindUniformCamera(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxUniformCameraPtr &ptrUniformCamera);

	bool CmdUniform1i(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int v0);
	bool CmdUniform2i(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int v0, int v1);
	bool CmdUniform3i(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int v0, int v1, int v2);
	bool CmdUniform4i(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int v0, int v1, int v2, int v3);
	bool CmdUniform1f(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, float v0);
	bool CmdUniform2f(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, float v0, float v1);
	bool CmdUniform3f(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, float v0, float v1, float v2);
	bool CmdUniform4f(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, float v0, float v1, float v2, float v3);
	bool CmdUniform1iv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const int *value);
	bool CmdUniform2iv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const int *value);
	bool CmdUniform3iv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const int *value);
	bool CmdUniform4iv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const int *value);
	bool CmdUniform1fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniform2fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniform3fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniform4fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniformMatrix2fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniformMatrix3fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value);
	bool CmdUniformMatrix4fv(CGfxCommandBufferPtr &ptrCommandBuffer, const char *szName, int count, const float *value);

	bool CmdSetScissor(CGfxCommandBufferPtr &ptrCommandBuffer, int x, int y, int width, int height);
	bool CmdSetViewport(CGfxCommandBufferPtr &ptrCommandBuffer, int x, int y, int width, int height);

	bool CmdClearDepth(CGfxCommandBufferPtr &ptrCommandBuffer, float depth);
	bool CmdClearColor(CGfxCommandBufferPtr &ptrCommandBuffer, float red, float green, float blue, float alpha);

	bool CmdDrawInstance(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMeshPtr &ptrMesh, const uint8_t *pInstanceBuffer, uint32_t size);
	bool CmdDrawIndirect(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxMeshPtr &ptrMesh, const uint8_t *pInstanceBuffer, uint32_t size);
	bool CmdDrawScreen(CGfxCommandBufferPtr &ptrCommandBuffer);

	bool CmdExecute(CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxCommandBufferPtr &ptrSecondaryCommandBuffer);

public:
	void Submit(const CGfxCommandBufferPtr &ptrCommandBuffer);
	void Present(void);

private:
	void BindPipelineCompute(CGfxPipelineCompute *pPipelineCompute);
	void BindPipelineGraphics(CGfxPipelineGraphics *pPipelineGraphics);
	void BindUniformEngine(CGfxUniformEngine *pUniformEngine);
	void BindUniformCamera(CGfxUniformCamera *pUniformCamera);
	void BindUniformBuffer(CGfxUniformBuffer *pUniformBuffer, uint32_t name);
	void BindMaterialPass(CGfxMaterialPass *pPass);
	void BindInputTexture(const char *szName, CGfxTextureBase *pTexture);

private:
	void Uniform1i(uint32_t name, int v0) const;
	void Uniform2i(uint32_t name, int v0, int v1) const;
	void Uniform3i(uint32_t name, int v0, int v1, int v2) const;
	void Uniform4i(uint32_t name, int v0, int v1, int v2, int v3) const;
	void Uniform1f(uint32_t name, float v0) const;
	void Uniform2f(uint32_t name, float v0, float v1) const;
	void Uniform3f(uint32_t name, float v0, float v1, float v2) const;
	void Uniform4f(uint32_t name, float v0, float v1, float v2, float v3) const;
	void Uniform1iv(uint32_t name, int count, const int *value) const;
	void Uniform2iv(uint32_t name, int count, const int *value) const;
	void Uniform3iv(uint32_t name, int count, const int *value) const;
	void Uniform4iv(uint32_t name, int count, const int *value) const;
	void Uniform1fv(uint32_t name, int count, const float *value) const;
	void Uniform2fv(uint32_t name, int count, const float *value) const;
	void Uniform3fv(uint32_t name, int count, const float *value) const;
	void Uniform4fv(uint32_t name, int count, const float *value) const;
	void UniformMatrix2fv(uint32_t name, int count, const float *value) const;
	void UniformMatrix3fv(uint32_t name, int count, const float *value) const;
	void UniformMatrix4fv(uint32_t name, int count, const float *value) const;


private:
	CGLES3SwapChain *m_pSwapChain;

private:
	CGLES3MeshManager *m_pMeshManager;
	CGLES3ShaderManager *m_pShaderManager;
	CGLES3SamplerManager *m_pSamplerManager;
	CGLES3TextureManager *m_pTextureManager;
	CGLES3UniformManager *m_pUniformManager;
	CGLES3PipelineManager *m_pPipelineManager;
	CGLES3MaterialManager *m_pMaterialManager;
	CGLES3RenderPassManager *m_pRenderPassManager;
	CGLES3FrameBufferManager *m_pFrameBufferManager;
	CGLES3CommandBufferManager *m_pCommandBufferManager;

private:
	CGLES3MaterialPass *m_pGlobalMaterialPass;
	CGLES3MaterialPass *m_pCurrentMaterialPass;
	CGLES3PipelineCompute *m_pCurrentPipelineCompute;
	CGLES3PipelineGraphics *m_pCurrentPipelineGraphics;
};
