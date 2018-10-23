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
	bool CmdBindMesh(const CGfxMeshPtr &ptrMesh, const eastl::vector<glm::mat4> &mtxTransforms);
	bool CmdBindDrawIndirectBuffer(const CGfxDrawIndirectBufferPtr &ptrDrawIndirectBuffer, uint32_t instanceCount);
	bool CmdBindMaterialPass(const CGfxMaterialPtr &ptrMaterial, uint32_t namePass);
	bool CmdBindInputTexture(const char *szName, uint32_t texture, uint32_t minFilter, uint32_t magFilter, uint32_t addressMode);

	bool CmdUniform1f(const char *szName, float v0);
	bool CmdUniform2f(const char *szName, float v0, float v1);
	bool CmdUniform3f(const char *szName, float v0, float v1, float v2);
	bool CmdUniform4f(const char *szName, float v0, float v1, float v2, float v3);
	bool CmdUniform1fv(const char *szName, uint32_t count, const float *value);
	bool CmdUniform2fv(const char *szName, uint32_t count, const float *value);
	bool CmdUniform3fv(const char *szName, uint32_t count, const float *value);
	bool CmdUniform4fv(const char *szName, uint32_t count, const float *value);
	bool CmdUniformMatrix2fv(const char *szName, uint32_t count, const float *value);
	bool CmdUniformMatrix3fv(const char *szName, uint32_t count, const float *value);
	bool CmdUniformMatrix4fv(const char *szName, uint32_t count, const float *value);

	bool CmdClearDepth(float depth);
	bool CmdClearColor(float red, float green, float blue, float alpha);
	bool CmdDrawInstance(uint32_t mode, uint32_t type, uint32_t offset, int count, int instanceCount);
	bool CmdDrawIndirect(uint32_t mode, uint32_t type, uint32_t offset);
	bool CmdDrawElements(uint32_t mode, uint32_t type, uint32_t offset, int count);

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
