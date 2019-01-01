#pragma once
#include "GLES3Renderer.h"


class CGLES3CommandBuffer : public CGfxCommandBuffer
{
	friend class CGLES3CommandBufferManager;


private:
	CGLES3CommandBuffer(CGLES3CommandBufferManager *pManager, bool bMainCommandBuffer);
	virtual ~CGLES3CommandBuffer(void);
	virtual void Release(void);


public:
	void Clearup(void);
	bool Execute(void) const;

public:
	bool CmdBeginRenderPass(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass);
	bool CmdNextSubpass(void);
	bool CmdEndRenderPass(void);

	bool CmdBindPipelineCompute(const CGfxPipelineCompute *pPipelineCompute);
	bool CmdBindPipelineGraphics(const CGfxPipelineGraphics *pPipelineGraphics);
	bool CmdBindMaterialPass(const CGfxMaterialPtr &ptrMaterial, uint32_t namePass);
	bool CmdBindUniformBuffer(const CGfxUniformBufferPtr &ptrUniformBuffer, uint32_t nameUniform);
	bool CmdBindMeshDraw(const CGfxMeshDrawPtr &ptrMeshDraw);

	bool CmdUniform1i(const char *szName, int v0);
	bool CmdUniform2i(const char *szName, int v0, int v1);
	bool CmdUniform3i(const char *szName, int v0, int v1, int v2);
	bool CmdUniform4i(const char *szName, int v0, int v1, int v2, int v3);
	bool CmdUniform1f(const char *szName, float v0);
	bool CmdUniform2f(const char *szName, float v0, float v1);
	bool CmdUniform3f(const char *szName, float v0, float v1, float v2);
	bool CmdUniform4f(const char *szName, float v0, float v1, float v2, float v3);
	bool CmdUniform1iv(const char *szName, int count, const int *value);
	bool CmdUniform2iv(const char *szName, int count, const int *value);
	bool CmdUniform3iv(const char *szName, int count, const int *value);
	bool CmdUniform4iv(const char *szName, int count, const int *value);
	bool CmdUniform1fv(const char *szName, int count, const float *value);
	bool CmdUniform2fv(const char *szName, int count, const float *value);
	bool CmdUniform3fv(const char *szName, int count, const float *value);
	bool CmdUniform4fv(const char *szName, int count, const float *value);
	bool CmdUniformMatrix2fv(const char *szName, int count, const float *value);
	bool CmdUniformMatrix3fv(const char *szName, int count, const float *value);
	bool CmdUniformMatrix4fv(const char *szName, int count, const float *value);

	bool CmdClearDepth(float depth);
	bool CmdClearColor(float red, float green, float blue, float alpha);

	bool CmdSetScissor(int x, int y, int width, int height);
	bool CmdSetViewport(int x, int y, int width, int height);

	bool CmdSetInstanceBufferData(const CGfxMeshDrawPtr &ptrMeshDraw, const uint8_t *pInstanceBuffer, uint32_t size);

	bool CmdDrawElements(GfxDrawMode mode, GfxIndexType type, int offset, int count);
	bool CmdDrawInstance(GfxDrawMode mode, GfxIndexType type, int offset, int count, int instanceCount);
	bool CmdDrawIndirect(GfxDrawMode mode, GfxIndexType type, int offset);

	bool CmdExecute(const CGfxCommandBufferPtr &ptrCommandBuffer);

	bool CmdPresent(void);

	bool CmdPushDebugGroup(const char *szMessage);
	bool CmdPopDebugGroup(void);


private:
	bool m_bInPassScope;
	uint32_t m_indexSubPass;
	CGfxRenderPassPtr m_ptrRenderPass;
	CGfxFrameBufferPtr m_ptrFrameBuffer;

private:
	eastl::vector<CGfxCommandBase*> m_pCommands;

private:
	CGLES3CommandBufferManager *m_pManager;
};
