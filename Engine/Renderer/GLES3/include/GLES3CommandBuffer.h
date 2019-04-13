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
	const CGfxFrameBufferPtr GetFrameBuffer(void) const;
	const CGfxRenderPassPtr GetRenderPass(void) const;

	bool IsInRenderPass(void) const;
	uint32_t GetSubpassIndex(void) const;

public:
	void Clearup(void);
	bool Execute(void) const;

public:
	bool CmdBeginRenderPass(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass);
	bool CmdNextSubpass(void);
	bool CmdEndRenderPass(void);

	bool CmdBindPipelineCompute(const CGfxPipelineCompute *pPipelineCompute);
	bool CmdBindPipelineGraphics(const CGfxPipelineGraphics *pPipelineGraphics);
	bool CmdBindDescriptorSet(const CGfxDescriptorSetPtr ptrDescriptorSet);

	bool CmdUniform1i(uint32_t name, int v0);
	bool CmdUniform2i(uint32_t name, int v0, int v1);
	bool CmdUniform3i(uint32_t name, int v0, int v1, int v2);
	bool CmdUniform4i(uint32_t name, int v0, int v1, int v2, int v3);
	bool CmdUniform1f(uint32_t name, float v0);
	bool CmdUniform2f(uint32_t name, float v0, float v1);
	bool CmdUniform3f(uint32_t name, float v0, float v1, float v2);
	bool CmdUniform4f(uint32_t name, float v0, float v1, float v2, float v3);
	bool CmdUniform1iv(uint32_t name, int count, const int *value);
	bool CmdUniform2iv(uint32_t name, int count, const int *value);
	bool CmdUniform3iv(uint32_t name, int count, const int *value);
	bool CmdUniform4iv(uint32_t name, int count, const int *value);
	bool CmdUniform1fv(uint32_t name, int count, const float *value);
	bool CmdUniform2fv(uint32_t name, int count, const float *value);
	bool CmdUniform3fv(uint32_t name, int count, const float *value);
	bool CmdUniform4fv(uint32_t name, int count, const float *value);
	bool CmdUniformMatrix2fv(uint32_t name, int count, const float *value);
	bool CmdUniformMatrix3fv(uint32_t name, int count, const float *value);
	bool CmdUniformMatrix4fv(uint32_t name, int count, const float *value);

	bool CmdSetScissor(int x, int y, int width, int height);
	bool CmdSetViewport(int x, int y, int width, int height);

	bool CmdClearDepth(float depth);
	bool CmdClearColor(float red, float green, float blue, float alpha);

	bool CmdDrawInstance(const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t *pInstanceBuffer, uint32_t size);
	bool CmdDrawIndirect(const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t *pInstanceBuffer, uint32_t size);

	bool CmdExecute(const CGfxCommandBufferPtr ptrCommandBuffer);
	bool CmdPresent(void);

	bool CmdPushDebugGroup(const char *szMessage);
	bool CmdPopDebugGroup(void);


private:
	bool m_bInRenderPass;
	uint32_t m_indexSubpass;

	CGfxRenderPassPtr m_ptrRenderPass;
	CGfxFrameBufferPtr m_ptrFrameBuffer;

	eastl::vector<CGfxCommandBase*> m_pCommands;

private:
	CGLES3CommandBufferManager *m_pManager;
};
