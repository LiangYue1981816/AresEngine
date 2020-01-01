#pragma once
#include "VKRenderer.h"


class CVKCommandBuffer : public CGfxCommandBuffer
{
	friend class CVKCommandBufferManager;


private:
	CVKCommandBuffer(CVKDevice* pDevice, CVKCommandBufferManager* pManager, VkCommandPool vkCommandPool, bool bMainCommandBuffer);
	virtual ~CVKCommandBuffer(void);
	virtual void Release(void);


private:
	bool Create(VkCommandPool vkCommandPool, bool bMainCommandBuffer);
	void Destroy(void);

public:
	VkFence GetFence(void) const;
	VkCommandPool GetCommandPool(void) const;
	VkCommandBuffer GetCommandBuffer(void) const;

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
	void ResetCommandBuffer(void) const;

public:
	bool IsEmpty(void) const;

	bool BeginRecord(void);
	bool BeginRecord(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass);
	bool EndRecord(void);

	// Outside RenderPass
	bool CmdSetImageLayout(const CGfxTexture2DPtr ptrTexture, GfxImageLayout imageLayout);
	bool CmdSetImageLayout(const CGfxTexture2DArrayPtr ptrTexture, GfxImageLayout imageLayout);
	bool CmdSetImageLayout(const CGfxTextureCubemapPtr ptrTexture, GfxImageLayout imageLayout);
	bool CmdSetImageLayout(const CGfxRenderTexturePtr ptrTexture, GfxImageLayout imageLayout);
	bool CmdSetBufferBarrier(const CGfxStorageBufferPtr ptrBuffer, GfxAccessFlags srcAccessFlags, GfxAccessFlags dstAccessFlags);

	// Inside RenderPass
	bool CmdBeginRenderPass(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass);
	bool CmdNextSubpass(void);
	bool CmdEndRenderPass(void);

	bool CmdBindPipelineCompute(const CGfxPipelineCompute* pPipelineCompute);
	bool CmdBindPipelineGraphics(const CGfxPipelineGraphics* pPipelineGraphics);
	bool CmdBindDescriptorSet(const CGfxDescriptorSetPtr ptrDescriptorSet);
	bool CmdBindMesh(const CGfxMeshPtr ptrMesh);
	bool CmdBindMeshDraw(const CGfxMeshDrawPtr ptrMeshDraw);

	bool CmdUniform1i(uint32_t name, int v0);
	bool CmdUniform2i(uint32_t name, int v0, int v1);
	bool CmdUniform3i(uint32_t name, int v0, int v1, int v2);
	bool CmdUniform4i(uint32_t name, int v0, int v1, int v2, int v3);
	bool CmdUniform1f(uint32_t name, float v0);
	bool CmdUniform2f(uint32_t name, float v0, float v1);
	bool CmdUniform3f(uint32_t name, float v0, float v1, float v2);
	bool CmdUniform4f(uint32_t name, float v0, float v1, float v2, float v3);
	bool CmdUniform1iv(uint32_t name, int count, const int* value);
	bool CmdUniform2iv(uint32_t name, int count, const int* value);
	bool CmdUniform3iv(uint32_t name, int count, const int* value);
	bool CmdUniform4iv(uint32_t name, int count, const int* value);
	bool CmdUniform1fv(uint32_t name, int count, const float* value);
	bool CmdUniform2fv(uint32_t name, int count, const float* value);
	bool CmdUniform3fv(uint32_t name, int count, const float* value);
	bool CmdUniform4fv(uint32_t name, int count, const float* value);
	bool CmdUniformMatrix2fv(uint32_t name, int count, const float* value);
	bool CmdUniformMatrix3fv(uint32_t name, int count, const float* value);
	bool CmdUniformMatrix4fv(uint32_t name, int count, const float* value);

	bool CmdSetScissor(int x, int y, int width, int height);
	bool CmdSetViewport(int x, int y, int width, int height);

	bool CmdClearDepth(float depth);
	bool CmdClearColor(float red, float green, float blue, float alpha);

	bool CmdDrawInstance(const CGfxMeshDrawPtr ptrMeshDraw);
	bool CmdUpdateInstanceBuffer(const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size);

	bool CmdDispatch(int numLocalWorkGroupX, int numLocalWorkGroupY, int numLocalWorkGroupZ);

	bool CmdPushDebugGroup(const char* szMessage);
	bool CmdPopDebugGroup(void);

	bool CmdExecute(const CGfxCommandBufferPtr ptrCommandBuffer);


private:
	VkFence m_vkFence;

	VkCommandPool m_vkCommandPool;
	VkCommandBuffer m_vkCommandBuffer;

private:
	int m_indexSubpass;

	CGfxRenderPassPtr m_ptrRenderPass;
	CGfxFrameBufferPtr m_ptrFrameBuffer;

	eastl::vector<CGfxCommandBase*> m_pCommands;

private:
	CGfxPipelineCompute* m_pCurrentPipelineCompute;
	CGfxPipelineGraphics* m_pCurrentPipelineGraphics;

private:
	CVKSemaphore* m_pSemaphore;

private:
	CVKDevice* m_pDevice;

private:
	CVKCommandBufferManager* m_pManager;
};
