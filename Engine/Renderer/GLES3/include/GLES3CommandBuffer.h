#pragma once
#include "GLES3Renderer.h"


class CGLES3CommandBuffer : public CGfxCommandBuffer
{
	friend class CGLES3CommandBufferManager;


private:
	CGLES3CommandBuffer(CGLES3CommandBufferManager* pManager, bool bMainCommandBuffer);
	virtual ~CGLES3CommandBuffer(void);
	virtual void Release(void);


public:
	const CGfxSemaphore* GetSemaphore(void) const;

public:
	bool IsInRenderPass(void) const;
	int GetSubpassIndex(void) const;

	const CGfxRenderPassPtr GetRenderPass(void) const;
	const CGfxFrameBufferPtr GetFrameBuffer(void) const;

public:
	void Clearup(void);
	void Execute(void) const;
	void WaitForFinish(void) const;

public:
	bool IsEmpty(void) const;

	void BeginRecord(void);
	void BeginRecord(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass);
	void EndRecord(void);

	// Outside RenderPass
	void CmdSetImageLayout(const CGfxTexture2DPtr ptrTexture, GfxImageLayout imageLayout);
	void CmdSetImageLayout(const CGfxTexture2DArrayPtr ptrTexture, GfxImageLayout imageLayout);
	void CmdSetImageLayout(const CGfxTextureCubemapPtr ptrTexture, GfxImageLayout imageLayout);
	void CmdSetImageLayout(const CGfxRenderTexturePtr ptrTexture, GfxImageLayout imageLayout);
	void CmdSetBufferBarrier(const CGfxStorageBufferPtr ptrBuffer, GfxAccessFlags srcAccessFlags, GfxAccessFlags dstAccessFlags);

	// Inside RenderPass
	void CmdBeginRenderPass(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass);
	void CmdNextSubpass(void);
	void CmdEndRenderPass(void);

	void CmdBindPipelineCompute(const CGfxPipelineCompute* pPipelineCompute);
	void CmdBindPipelineGraphics(const CGfxPipelineGraphics* pPipelineGraphics);
	void CmdBindDescriptorSet(const CGfxDescriptorSetPtr ptrDescriptorSet);
	void CmdBindIndexBuffer(const CGfxIndexBufferPtr ptrIndexBuffer);
	void CmdBindVertexBuffer(const CGfxVertexBufferPtr ptrVertexBuffer);
	void CmdBindInstanceBuffer(const CGfxInstanceBufferPtr ptrInstanceBuffer);

	void CmdSetIndexBuffer(const CGfxIndexBufferPtr ptrIndexBuffer, const uint8_t* pBuffer, uint32_t size);
	void CmdSetVertexBuffer(const CGfxVertexBufferPtr ptrVertexBuffer, const uint8_t* pBuffer, uint32_t size);
	void CmdSetInstanceBuffer(const CGfxInstanceBufferPtr ptrInstanceBuffer, const uint8_t* pBuffer, uint32_t size);

	void CmdUniform1i(uint32_t name, int v0);
	void CmdUniform2i(uint32_t name, int v0, int v1);
	void CmdUniform3i(uint32_t name, int v0, int v1, int v2);
	void CmdUniform4i(uint32_t name, int v0, int v1, int v2, int v3);
	void CmdUniform1f(uint32_t name, float v0);
	void CmdUniform2f(uint32_t name, float v0, float v1);
	void CmdUniform3f(uint32_t name, float v0, float v1, float v2);
	void CmdUniform4f(uint32_t name, float v0, float v1, float v2, float v3);
	void CmdUniform1iv(uint32_t name, int count, const int* value);
	void CmdUniform2iv(uint32_t name, int count, const int* value);
	void CmdUniform3iv(uint32_t name, int count, const int* value);
	void CmdUniform4iv(uint32_t name, int count, const int* value);
	void CmdUniform1fv(uint32_t name, int count, const float* value);
	void CmdUniform2fv(uint32_t name, int count, const float* value);
	void CmdUniform3fv(uint32_t name, int count, const float* value);
	void CmdUniform4fv(uint32_t name, int count, const float* value);
	void CmdUniformMatrix2fv(uint32_t name, int count, const float* value);
	void CmdUniformMatrix3fv(uint32_t name, int count, const float* value);
	void CmdUniformMatrix4fv(uint32_t name, int count, const float* value);

	void CmdSetScissor(int x, int y, int width, int height);
	void CmdSetViewport(int x, int y, int width, int height);

	void CmdClearDepth(float depth);
	void CmdClearColor(float red, float green, float blue, float alpha);

	void CmdDrawInstance(int indexType, int indexOffset, int indexCount, int instanceCount);
	void CmdDispatch(int numLocalWorkGroupX, int numLocalWorkGroupY, int numLocalWorkGroupZ);

	void CmdPushDebugGroup(const char* szMessage);
	void CmdPopDebugGroup(void);

	void CmdExecute(const CGfxCommandBufferPtr ptrCommandBuffer);


private:
	int m_indexSubpass;

	CGfxRenderPassPtr m_ptrRenderPass;
	CGfxFrameBufferPtr m_ptrFrameBuffer;

	eastl::vector<CGfxCommandBase*> m_pCommands;

private:
	CGfxPipelineCompute* m_pCurrentPipelineCompute;
	CGfxPipelineGraphics* m_pCurrentPipelineGraphics;

private:
	CGLES3CommandBufferManager* m_pManager;
};
