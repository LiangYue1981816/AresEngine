#pragma once
#include "PreHeader.h"


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
	virtual const CGfxSemaphore* GetSemaphore(void) const = 0;

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

	virtual void BeginRecord(void) = 0;
	virtual void BeginRecord(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass) = 0;
	virtual void EndRecord(void) = 0;

	// Outside RenderPass
	virtual void CmdBufferData(const CGfxIndexBufferPtr ptrBuffer, size_t offset, size_t size, const void* data) = 0;
	virtual void CmdBufferData(const CGfxVertexBufferPtr ptrBuffer, size_t offset, size_t size, const void* data) = 0;
	virtual void CmdBufferData(const CGfxInstanceBufferPtr ptrBuffer, size_t offset, size_t size, const void* data) = 0;
	virtual void CmdBufferData(const CGfxIndirectBufferPtr ptrBuffer, size_t offset, size_t size, const void* data) = 0;
	virtual void CmdBufferData(const CGfxUniformBufferPtr ptrBuffer, size_t offset, size_t size, const void* data) = 0;
	virtual void CmdBufferData(const CGfxStorageBufferPtr ptrBuffer, size_t offset, size_t size, const void* data) = 0;

	virtual void CmdTexture2DData(const CGfxTexture2DPtr ptrTexture, GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data) = 0;
	virtual void CmdTexture2DArrayData(const CGfxTexture2DArrayPtr ptrTexture, GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data) = 0;
	virtual void CmdTextureCubemapData(const CGfxTextureCubemapPtr ptrTexture, GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data) = 0;

	virtual void CmdSetImageLayout(const CGfxTexture2DPtr ptrTexture, GfxImageLayout imageLayout) = 0;
	virtual void CmdSetImageLayout(const CGfxTexture2DArrayPtr ptrTexture, GfxImageLayout imageLayout) = 0;
	virtual void CmdSetImageLayout(const CGfxTextureCubemapPtr ptrTexture, GfxImageLayout imageLayout) = 0;
	virtual void CmdSetImageLayout(const CGfxRenderTexturePtr ptrTexture, GfxImageLayout imageLayout) = 0;
	virtual void CmdSetBufferBarrier(const CGfxStorageBufferPtr ptrBuffer, GfxAccessFlags srcAccessFlags, GfxAccessFlags dstAccessFlags) = 0;
	
	// Inside RenderPass
	virtual void CmdBeginRenderPass(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass) = 0;
	virtual void CmdNextSubpass(void) = 0;
	virtual void CmdEndRenderPass(void) = 0;

	virtual void CmdBindPipelineCompute(const CGfxPipelineCompute* pPipelineCompute) = 0;
	virtual void CmdBindPipelineGraphics(const CGfxPipelineGraphics* pPipelineGraphics) = 0;
	virtual void CmdBindDescriptorSet(const CGfxDescriptorSetPtr ptrDescriptorSet) = 0;
	virtual void CmdBindIndexBuffer(const CGfxIndexBufferPtr ptrIndexBuffer) = 0;
	virtual void CmdBindVertexBuffer(const CGfxVertexBufferPtr ptrVertexBuffer) = 0;
	virtual void CmdBindInstanceBuffer(const CGfxInstanceBufferPtr ptrInstanceBuffer, int offset) = 0;

	virtual void CmdUniform1i(uint32_t name, int v0) = 0;
	virtual void CmdUniform2i(uint32_t name, int v0, int v1) = 0;
	virtual void CmdUniform3i(uint32_t name, int v0, int v1, int v2) = 0;
	virtual void CmdUniform4i(uint32_t name, int v0, int v1, int v2, int v3) = 0;
	virtual void CmdUniform1f(uint32_t name, float v0) = 0;
	virtual void CmdUniform2f(uint32_t name, float v0, float v1) = 0;
	virtual void CmdUniform3f(uint32_t name, float v0, float v1, float v2) = 0;
	virtual void CmdUniform4f(uint32_t name, float v0, float v1, float v2, float v3) = 0;
	virtual void CmdUniform1iv(uint32_t name, int count, const int* value) = 0;
	virtual void CmdUniform2iv(uint32_t name, int count, const int* value) = 0;
	virtual void CmdUniform3iv(uint32_t name, int count, const int* value) = 0;
	virtual void CmdUniform4iv(uint32_t name, int count, const int* value) = 0;
	virtual void CmdUniform1fv(uint32_t name, int count, const float* value) = 0;
	virtual void CmdUniform2fv(uint32_t name, int count, const float* value) = 0;
	virtual void CmdUniform3fv(uint32_t name, int count, const float* value) = 0;
	virtual void CmdUniform4fv(uint32_t name, int count, const float* value) = 0;
	virtual void CmdUniformMatrix2fv(uint32_t name, int count, const float* value) = 0;
	virtual void CmdUniformMatrix3fv(uint32_t name, int count, const float* value) = 0;
	virtual void CmdUniformMatrix4fv(uint32_t name, int count, const float* value) = 0;

	virtual void CmdSetScissor(int x, int y, int width, int height) = 0;
	virtual void CmdSetViewport(int x, int y, int width, int height, float znear, float zfar) = 0;

	virtual void CmdClearDepth(float depth) = 0;
	virtual void CmdClearColor(float red, float green, float blue, float alpha) = 0;

	virtual void CmdDrawInstance(int indexType, int indexOffset, int indexCount, int instanceCount) = 0;
	virtual void CmdDispatch(int numLocalWorkGroupX, int numLocalWorkGroupY, int numLocalWorkGroupZ) = 0;

	virtual void CmdPushDebugGroup(const char* szMessage) = 0;
	virtual void CmdPopDebugGroup(void) = 0;

	virtual void CmdExecute(const CGfxCommandBufferPtr ptrCommandBuffer) = 0;


private:
	bool m_bMainCommandBuffer;
};
