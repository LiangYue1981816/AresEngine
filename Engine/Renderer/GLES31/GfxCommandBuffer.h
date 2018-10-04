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
	bool CmdBeginRenderPass(const CGfxFrameBufferPtr &ptrFrameBuffer);
	bool CmdEndRenderPass(void);

	bool CmdSetScissor(int x, int y, int width, int height);
	bool CmdSetViewport(int x, int y, int width, int height);
	bool CmdSetCullFace(bool bEnable, uint32_t cullFace, uint32_t frontFace);
	bool CmdSetDepthTest(bool bEnable, uint32_t depthFunc);
	bool CmdSetDepthWrite(bool bEnable);
	bool CmdSetColorWrite(bool bEnableRed, bool bEnableGreen, bool bEnableBlue, bool bEnableAlpha);
	bool CmdSetBlend(bool bEnable, uint32_t srcFactor, uint32_t dstFactor);
	bool CmdSetPolygonOffset(bool bEnable, float factor, float units);

	bool CmdBindCamera(CGfxCamera *pCamera);
	bool CmdBindPipeline(CGfxPipelineBase *pPipeline);
	bool CmdBindMesh(const CGfxMeshPtr &ptrMesh, const eastl::vector<glm::mat4> &mtxTransforms);
	bool CmdBindMaterialPass(const CGfxMaterialPtr &ptrMaterial, uint32_t namePass);
	bool CmdBindInputTexture(const char *szName, uint32_t texture, uint32_t minFilter, uint32_t magFilter, uint32_t addressMode);

	bool CmdClearDepth(float depth);
	bool CmdClearColor(float red, float green, float blue, float alpha);
	bool CmdDrawInstance(uint32_t mode, uint32_t type, int count, int baseIndex, int instanceCount);
	bool CmdDrawIndirect(uint32_t mode, uint32_t type, int count, int baseIndex, int baseVertex, int instanceCount);
	bool CmdDrawElements(uint32_t mode, uint32_t type, int count, int baseIndex);

	bool CmdExecute(CGfxCommandBuffer *pCommandBuffer);


private:
	bool m_bMainCommandBuffer;
	bool m_bInPassScope;

private:
	CGfxFrameBufferPtr m_ptrFrameBuffer;
	eastl::vector<CGfxCommandBase*> m_commands;
};
