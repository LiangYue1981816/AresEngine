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

class CALL_API CGfxCommandBuffer : public CGfxResource
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
	virtual bool IsInRenderPass(void) const = 0;
	virtual int GetSubpassIndex(void) const = 0;

	virtual const CGfxRenderPassPtr GetRenderPass(void) const = 0;
	virtual const CGfxFrameBufferPtr GetFrameBuffer(void) const = 0;

public:
	virtual void Clearup(void) = 0;
	virtual void Execute(void) const = 0;
	virtual void WaitForFinish(void) const = 0;

public:
	virtual bool IsEmpty(void) const = 0;

	virtual bool BeginRecord(void) = 0;
	virtual bool BeginRecord(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass) = 0;
	virtual bool EndRecord(void) = 0;

	// Outside RenderPass
	virtual bool CmdSetImageLayout(const CGfxTexture2DPtr ptrTexture, GfxImageLayout imageLayout) = 0;
	virtual bool CmdSetImageLayout(const CGfxTexture2DArrayPtr ptrTexture, GfxImageLayout imageLayout) = 0;
	virtual bool CmdSetImageLayout(const CGfxTextureCubemapPtr ptrTexture, GfxImageLayout imageLayout) = 0;
	virtual bool CmdSetImageLayout(const CGfxRenderTexturePtr ptrTexture, GfxImageLayout imageLayout) = 0;
	
	// Inside RenderPass
	virtual bool CmdBeginRenderPass(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass) = 0;
	virtual bool CmdNextSubpass(void) = 0;
	virtual bool CmdEndRenderPass(void) = 0;

	virtual bool CmdBindPipelineCompute(const CGfxPipelineCompute* pPipelineCompute) = 0;
	virtual bool CmdBindPipelineGraphics(const CGfxPipelineGraphics* pPipelineGraphics) = 0;
	virtual bool CmdBindDescriptorSet(const CGfxDescriptorSetPtr ptrDescriptorSet) = 0;

	virtual bool CmdUniform1i(uint32_t name, int v0) = 0;
	virtual bool CmdUniform2i(uint32_t name, int v0, int v1) = 0;
	virtual bool CmdUniform3i(uint32_t name, int v0, int v1, int v2) = 0;
	virtual bool CmdUniform4i(uint32_t name, int v0, int v1, int v2, int v3) = 0;
	virtual bool CmdUniform1f(uint32_t name, float v0) = 0;
	virtual bool CmdUniform2f(uint32_t name, float v0, float v1) = 0;
	virtual bool CmdUniform3f(uint32_t name, float v0, float v1, float v2) = 0;
	virtual bool CmdUniform4f(uint32_t name, float v0, float v1, float v2, float v3) = 0;
	virtual bool CmdUniform1iv(uint32_t name, int count, const int* value) = 0;
	virtual bool CmdUniform2iv(uint32_t name, int count, const int* value) = 0;
	virtual bool CmdUniform3iv(uint32_t name, int count, const int* value) = 0;
	virtual bool CmdUniform4iv(uint32_t name, int count, const int* value) = 0;
	virtual bool CmdUniform1fv(uint32_t name, int count, const float* value) = 0;
	virtual bool CmdUniform2fv(uint32_t name, int count, const float* value) = 0;
	virtual bool CmdUniform3fv(uint32_t name, int count, const float* value) = 0;
	virtual bool CmdUniform4fv(uint32_t name, int count, const float* value) = 0;
	virtual bool CmdUniformMatrix2fv(uint32_t name, int count, const float* value) = 0;
	virtual bool CmdUniformMatrix3fv(uint32_t name, int count, const float* value) = 0;
	virtual bool CmdUniformMatrix4fv(uint32_t name, int count, const float* value) = 0;

	virtual bool CmdSetScissor(int x, int y, int width, int height) = 0;
	virtual bool CmdSetViewport(int x, int y, int width, int height) = 0;

	virtual bool CmdClearDepth(float depth) = 0;
	virtual bool CmdClearColor(float red, float green, float blue, float alpha) = 0;

	virtual bool CmdDrawInstance(const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size) = 0;
	virtual bool CmdDrawIndirect(const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size) = 0;

	virtual bool CmdExecute(const CGfxCommandBufferPtr ptrCommandBuffer) = 0;

	// Other
	virtual bool CmdPushDebugGroup(const char* szMessage) = 0;
	virtual bool CmdPopDebugGroup(void) = 0;


private:
	bool m_bMainCommandBuffer;
};
