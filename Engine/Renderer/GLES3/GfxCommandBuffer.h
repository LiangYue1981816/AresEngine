#pragma once
#include "GfxRenderer.h"


class CGfxCommandBase
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

class CGfxCommandBuffer
{
	friend class CGfxRenderer;


public:
	CGfxCommandBuffer(bool bMainCommandBuffer);
	virtual ~CGfxCommandBuffer(void);


public:
	void Clearup(void);
	bool Execute(void) const;

public:
	bool CmdBeginRenderPass(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass);
	bool CmdNextSubpass(void);
	bool CmdEndRenderPass(void);

	bool CmdSetScissor(int x, int y, int width, int height);
	bool CmdSetViewport(int x, int y, int width, int height);

	bool CmdBindCamera(CGfxCamera *pCamera);
	bool CmdBindPipeline(CGfxPipelineBase *pPipeline);
	bool CmdBindMesh(const CGfxMeshPtr &ptrMesh);
	bool CmdBindMesh(const CGfxMeshPtr &ptrMesh, const uint8_t *pInstanceBuffer, uint32_t size);
	bool CmdBindDrawIndirectBuffer(const CGfxDrawIndirectBufferPtr &ptrDrawIndirectBuffer, uint32_t instanceCount);
	bool CmdBindMaterialPass(const CGfxMaterialPtr &ptrMaterial, uint32_t namePass);
	bool CmdBindInputTexture(const char *szName, uint32_t texture, uint32_t minFilter, uint32_t magFilter, uint32_t addressMode);

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

	bool CmdDrawElements(uint32_t mode, uint32_t type, uint32_t offset, int count);
	bool CmdDrawInstance(uint32_t mode, uint32_t type, uint32_t offset, int count, int instanceCount);
	bool CmdDrawIndirect(uint32_t mode, uint32_t type, uint32_t offset);
	bool CmdDrawIndirect(uint32_t mode, uint32_t type, int baseVertex, int firstIndex, int count, int instanceCount);

	bool CmdExecute(CGfxCommandBuffer *pCommandBuffer);


private:
	bool m_bMainCommandBuffer;

private:
	bool m_bInPassScope;
	uint32_t m_indexSubPass;
	CGfxRenderPassPtr m_ptrRenderPass;
	CGfxFrameBufferPtr m_ptrFrameBuffer;

private:
	eastl::vector<CGfxCommandBase*> m_pCommands;
};
