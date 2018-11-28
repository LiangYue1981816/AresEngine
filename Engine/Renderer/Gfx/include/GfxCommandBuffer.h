#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxCommandBase
{
public:
	CGfxCommandBase(void)
	{

	}
	virtual ~CGfxCommandBase(void)
	{

	}

public:
	virtual void Execute(void) const = 0;
};

class CGfxCommandBuffer : public CGfxResource
{
public:
	CGfxCommandBuffer(bool bMainCommandBuffer)
		: m_bMainCommandBuffer(bMainCommandBuffer)
	{

	}
	virtual ~CGfxCommandBuffer(void)
	{

	}

	bool IsMainCommandBuffer(void) const
	{
		return m_bMainCommandBuffer;
	}


public:
	virtual void Clearup(void) = 0;
	virtual bool Execute(void) const = 0;

public:
	virtual bool CmdBeginRenderPass(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass) = 0;
	virtual bool CmdNextSubpass(void) = 0;
	virtual bool CmdEndRenderPass(void) = 0;

	virtual bool CmdBindPipelineCompute(const CGfxPipelineCompute *pPipelineCompute) = 0;
	virtual bool CmdBindPipelineGraphics(const CGfxPipelineGraphics *pPipelineGraphics) = 0;
	virtual bool CmdBindMaterialPass(const CGfxMaterialPtr &ptrMaterial, uint32_t namePass) = 0;
	virtual bool CmdBindUniformBuffer(const CGfxUniformBufferPtr &ptrUniformBuffer, uint32_t nameUniform) = 0;
	virtual bool CmdBindMesh(const CGfxMeshPtr &ptrMesh, const int indexDraw) = 0;

	virtual bool CmdUniform1i(const char *szName, int v0) = 0;
	virtual bool CmdUniform2i(const char *szName, int v0, int v1) = 0;
	virtual bool CmdUniform3i(const char *szName, int v0, int v1, int v2) = 0;
	virtual bool CmdUniform4i(const char *szName, int v0, int v1, int v2, int v3) = 0;
	virtual bool CmdUniform1f(const char *szName, float v0) = 0;
	virtual bool CmdUniform2f(const char *szName, float v0, float v1) = 0;
	virtual bool CmdUniform3f(const char *szName, float v0, float v1, float v2) = 0;
	virtual bool CmdUniform4f(const char *szName, float v0, float v1, float v2, float v3) = 0;
	virtual bool CmdUniform1iv(const char *szName, int count, const int *value) = 0;
	virtual bool CmdUniform2iv(const char *szName, int count, const int *value) = 0;
	virtual bool CmdUniform3iv(const char *szName, int count, const int *value) = 0;
	virtual bool CmdUniform4iv(const char *szName, int count, const int *value) = 0;
	virtual bool CmdUniform1fv(const char *szName, int count, const float *value) = 0;
	virtual bool CmdUniform2fv(const char *szName, int count, const float *value) = 0;
	virtual bool CmdUniform3fv(const char *szName, int count, const float *value) = 0;
	virtual bool CmdUniform4fv(const char *szName, int count, const float *value) = 0;
	virtual bool CmdUniformMatrix2fv(const char *szName, int count, const float *value) = 0;
	virtual bool CmdUniformMatrix3fv(const char *szName, int count, const float *value) = 0;
	virtual bool CmdUniformMatrix4fv(const char *szName, int count, const float *value) = 0;

	virtual bool CmdClearDepth(float depth) = 0;
	virtual bool CmdClearColor(float red, float green, float blue, float alpha) = 0;

	virtual bool CmdSetScissor(int x, int y, int width, int height) = 0;
	virtual bool CmdSetViewport(int x, int y, int width, int height) = 0;

	virtual bool CmdSetInstanceBufferData(const CGfxMeshPtr &ptrMesh, const int indexDraw, const uint8_t *pInstanceBuffer, uint32_t size) = 0;
	virtual bool CmdSetDrawIndirectBufferData(const CGfxMeshPtr &ptrMesh, const int indexDraw, int instanceCount) = 0;

	virtual bool CmdDrawElements(uint32_t mode, uint32_t type, int offset, int count) = 0;
	virtual bool CmdDrawInstance(uint32_t mode, uint32_t type, int offset, int count, int instanceCount) = 0;
	virtual bool CmdDrawIndirect(uint32_t mode, uint32_t type, int offset) = 0;

	virtual bool CmdExecute(const CGfxCommandBufferPtr &ptrCommandBuffer) = 0;


protected:
	bool m_bMainCommandBuffer;
};
