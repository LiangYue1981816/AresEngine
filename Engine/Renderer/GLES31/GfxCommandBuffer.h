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

class CGfxMesh;
class CGfxMaterial;
class CGfxFrameBuffer;
class CGfxUniformCamera;
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
	bool CmdBeginPass(CGfxFrameBuffer *pFrameBuffer);
	bool CmdEndPass(void);

	bool CmdSetScissor(int x, int y, int width, int height);
	bool CmdSetViewport(int x, int y, int width, int height);
	bool CmdSetCullFace(bool bEnable, GLenum cullFace, GLenum frontFace);
	bool CmdSetDepthTest(bool bEnable, GLenum depthFunc);
	bool CmdSetDepthWrite(bool bEnable);
	bool CmdSetColorWrite(bool bEnableRed, bool bEnableGreen, bool bEnableBlue, bool bEnableAlpha);
	bool CmdSetBlend(bool bEnable, GLenum srcFactor, GLenum dstFactor);
	bool CmdSetPolygonOffset(bool bEnable, GLfloat factor, GLfloat units);

	bool CmdBindMesh(CGfxMesh *pMesh);
	bool CmdBindMesh(CGfxMesh *pMesh, eastl::vector<glm::mat4> &mtxTransforms);
	bool CmdBindCamera(CGfxCamera *pCamera);
	bool CmdBindMaterial(CGfxMaterial *pMaterial);
	bool CmdBindInputTexture(const char *szName, GLuint texture, GLenum minFilter, GLenum magFilter, GLenum addressMode);

	bool CmdClearDepth(float depth);
	bool CmdClearColor(float red, float green, float blue, float alpha);
	bool CmdDrawInstance(GLenum mode, GLenum type, GLsizei count, GLsizei baseIndex, GLsizei instanceCount);
	bool CmdDrawIndirect(GLenum mode, GLenum type, GLsizei count, GLsizei baseIndex, GLsizei baseVertex, GLsizei instanceCount);
	bool CmdDrawElements(GLenum mode, GLenum type, GLsizei count, GLsizei baseIndex);

	bool CmdExecute(CGfxCommandBuffer *pCommandBuffer);


private:
	bool m_bMainCommandBuffer;
	bool m_bInPassScope;

private:
	CGfxFrameBuffer *m_pFrameBuffer;
	eastl::vector<CGfxCommandBase*> m_commands;
};
