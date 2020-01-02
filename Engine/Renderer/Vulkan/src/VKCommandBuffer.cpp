#include "VKRenderer.h"
#include "./Command/VKCommandBeginRecord.h"
#include "./Command/VKCommandEndRecord.h"
#include "./Command/VKCommandSetImageLayout.h"
#include "./Command/VKCommandSetBufferBarrier.h"
#include "./Command/VKCommandBeginRenderPass.h"
#include "./Command/VKCommandNextSubPass.h"
#include "./Command/VKCommandEndRenderPass.h"
#include "./Command/VKCommandBindPipelineCompute.h"
#include "./Command/VKCommandBindPipelineGraphics.h"
#include "./Command/VKCommandBindDescriptorSet.h"
#include "./Command/VKCommandBindMesh.h"
#include "./Command/VKCommandBindMeshDraw.h"
#include "./Command/VKCommandUniform1i.h"
#include "./Command/VKCommandUniform2i.h"
#include "./Command/VKCommandUniform3i.h"
#include "./Command/VKCommandUniform4i.h"
#include "./Command/VKCommandUniform1f.h"
#include "./Command/VKCommandUniform2f.h"
#include "./Command/VKCommandUniform3f.h"
#include "./Command/VKCommandUniform4f.h"
#include "./Command/VKCommandUniform1iv.h"
#include "./Command/VKCommandUniform2iv.h"
#include "./Command/VKCommandUniform3iv.h"
#include "./Command/VKCommandUniform4iv.h"
#include "./Command/VKCommandUniform1fv.h"
#include "./Command/VKCommandUniform2fv.h"
#include "./Command/VKCommandUniform3fv.h"
#include "./Command/VKCommandUniform4fv.h"
#include "./Command/VKCommandUniformMatrix2fv.h"
#include "./Command/VKCommandUniformMatrix3fv.h"
#include "./Command/VKCommandUniformMatrix4fv.h"
#include "./Command/VKCommandSetScissor.h"
#include "./Command/VKCommandSetViewport.h"
#include "./Command/VKCommandClearDepth.h"
#include "./Command/VKCommandClearColor.h"
#include "./Command/VKCommandDispatch.h"
#include "./Command/VKCommandDrawInstance.h"
#include "./Command/VKCommandDrawIndirect.h"
#include "./Command/VKCommandUpdateInstanceBuffer.h"
#include "./Command/VKCommandPushDebugGroup.h"
#include "./Command/VKCommandPopDebugGroup.h"
#include "./Command/VKCommandExecute.h"


CVKCommandBuffer::CVKCommandBuffer(CVKDevice* pDevice, CVKCommandBufferManager* pManager, VkCommandPool vkCommandPool, bool bMainCommandBuffer)
	: CGfxCommandBuffer(bMainCommandBuffer)
	, m_pDevice(pDevice)
	, m_pManager(pManager)

	, m_vkFence(VK_NULL_HANDLE)
	, m_vkCommandPool(VK_NULL_HANDLE)
	, m_vkCommandBuffer(VK_NULL_HANDLE)

	, m_indexSubpass(-1)
	, m_pCurrentPipelineCompute(nullptr)
	, m_pCurrentPipelineGraphics(nullptr)

	, m_pSemaphore(nullptr)
{
	m_pSemaphore = new CVKSemaphore(m_pDevice);
	Create(vkCommandPool, bMainCommandBuffer);
}

CVKCommandBuffer::~CVKCommandBuffer(void)
{
	Destroy();
	delete m_pSemaphore;
}

void CVKCommandBuffer::Release(void)
{
	Clearup();
	m_pManager->Destroy(this);
}

bool CVKCommandBuffer::Create(VkCommandPool vkCommandPool, bool bMainCommandBuffer)
{
	m_vkCommandPool = vkCommandPool;

	if (bMainCommandBuffer) {
		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.pNext = nullptr;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		CALL_VK_FUNCTION_RETURN_BOOL(vkCreateFence(m_pDevice->GetDevice(), &fenceCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkFence));
	}

	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.pNext = nullptr;
	commandBufferAllocateInfo.commandPool = vkCommandPool;
	commandBufferAllocateInfo.level = bMainCommandBuffer ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
	commandBufferAllocateInfo.commandBufferCount = 1;
	CALL_VK_FUNCTION_RETURN_BOOL(vkAllocateCommandBuffers(m_pDevice->GetDevice(), &commandBufferAllocateInfo, &m_vkCommandBuffer));

	return true;
}

void CVKCommandBuffer::Destroy(void)
{
	Clearup();

	if (m_vkFence) {
		vkDestroyFence(m_pDevice->GetDevice(), m_vkFence, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (m_vkCommandPool && m_vkCommandBuffer) {
		vkFreeCommandBuffers(m_pDevice->GetDevice(), m_vkCommandPool, 1, &m_vkCommandBuffer);
	}

	m_vkFence = VK_NULL_HANDLE;
	m_vkCommandPool = VK_NULL_HANDLE;
	m_vkCommandBuffer = VK_NULL_HANDLE;
}

VkFence CVKCommandBuffer::GetFence(void) const
{
	return m_vkFence;
}

VkCommandPool CVKCommandBuffer::GetCommandPool(void) const
{
	return m_vkCommandPool;
}

VkCommandBuffer CVKCommandBuffer::GetCommandBuffer(void) const
{
	return m_vkCommandBuffer;
}

const CGfxSemaphore* CVKCommandBuffer::GetSemaphore(void) const
{
	return m_pSemaphore;
}

bool CVKCommandBuffer::IsInRenderPass(void) const
{
	return m_ptrRenderPass && m_indexSubpass >= 0 && m_indexSubpass < (int)m_ptrRenderPass->GetSubpassCount();
}

int CVKCommandBuffer::GetSubpassIndex(void) const
{
	return m_indexSubpass;
}

const CGfxRenderPassPtr CVKCommandBuffer::GetRenderPass(void) const
{
	return m_ptrRenderPass;
}

const CGfxFrameBufferPtr CVKCommandBuffer::GetFrameBuffer(void) const
{
	return m_ptrFrameBuffer;
}

void CVKCommandBuffer::Clearup(void)
{
	WaitForFinish();
	ResetCommandBuffer();

	for (const auto& itCommand : m_pCommands) {
		delete itCommand;
	}

	m_pCommands.clear();
	m_ptrRenderPass.Release();
	m_ptrFrameBuffer.Release();

	m_indexSubpass = -1;
	m_pCurrentPipelineCompute = nullptr;
	m_pCurrentPipelineGraphics = nullptr;
}

void CVKCommandBuffer::Execute(void) const
{

}

void CVKCommandBuffer::WaitForFinish(void) const
{
	if (m_vkFence) {
		vkWaitForFences(m_pDevice->GetDevice(), 1, &m_vkFence, VK_TRUE, UINT64_MAX);
	}
}

void CVKCommandBuffer::ResetCommandBuffer(void) const
{
	vkResetCommandBuffer(m_vkCommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
}

bool CVKCommandBuffer::IsEmpty(void) const
{
	return m_pCommands.empty();
}

void CVKCommandBuffer::BeginRecord(void)
{
	m_pCommands.emplace_back(new CVKCommandBeginRecord(m_vkCommandBuffer));
}

void CVKCommandBuffer::BeginRecord(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass)
{
	m_indexSubpass = indexSubpass;
	m_ptrRenderPass = ptrRenderPass;
	m_ptrFrameBuffer = ptrFrameBuffer;
	m_pCommands.emplace_back(new CVKCommandBeginRecord(m_vkCommandBuffer, m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));
}

void CVKCommandBuffer::EndRecord(void)
{
	m_pCommands.emplace_back(new CVKCommandEndRecord(m_vkCommandBuffer));
}

void CVKCommandBuffer::CmdSetImageLayout(const CGfxTexture2DPtr ptrTexture, GfxImageLayout imageLayout)
{
	m_pCommands.emplace_back(new CVKCommandSetImageLayout(m_vkCommandBuffer, ptrTexture, imageLayout));
}

void CVKCommandBuffer::CmdSetImageLayout(const CGfxTexture2DArrayPtr ptrTexture, GfxImageLayout imageLayout)
{
	m_pCommands.emplace_back(new CVKCommandSetImageLayout(m_vkCommandBuffer, ptrTexture, imageLayout));
}

void CVKCommandBuffer::CmdSetImageLayout(const CGfxTextureCubemapPtr ptrTexture, GfxImageLayout imageLayout)
{
	m_pCommands.emplace_back(new CVKCommandSetImageLayout(m_vkCommandBuffer, ptrTexture, imageLayout));
}

void CVKCommandBuffer::CmdSetImageLayout(const CGfxRenderTexturePtr ptrTexture, GfxImageLayout imageLayout)
{
	m_pCommands.emplace_back(new CVKCommandSetImageLayout(m_vkCommandBuffer, ptrTexture, imageLayout));
}

void CVKCommandBuffer::CmdSetBufferBarrier(const CGfxStorageBufferPtr ptrBuffer, GfxAccessFlags srcAccessFlags, GfxAccessFlags dstAccessFlags)
{
	m_pCommands.emplace_back(new CVKCommandSetBufferBarrier(m_vkCommandBuffer, ptrBuffer, srcAccessFlags, dstAccessFlags));
}

void CVKCommandBuffer::CmdBeginRenderPass(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass)
{
	m_indexSubpass = 0;
	m_ptrRenderPass = ptrRenderPass;
	m_ptrFrameBuffer = ptrFrameBuffer;
	m_pCommands.emplace_back(new CVKCommandBeginRenderPass(m_vkCommandBuffer, m_ptrFrameBuffer, m_ptrRenderPass));
}

void CVKCommandBuffer::CmdNextSubpass(void)
{
	if (IsMainCommandBuffer() == true && IsInRenderPass() == true && m_indexSubpass < (int)m_ptrRenderPass->GetSubpassCount() - 1) {
		m_indexSubpass += 1;
		m_pCommands.emplace_back(new CVKCommandNextSubPass(m_vkCommandBuffer, m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));
	}
}

void CVKCommandBuffer::CmdEndRenderPass(void)
{
	if (m_indexSubpass >= 0) {
		m_indexSubpass = -1;
		m_pCommands.emplace_back(new CVKCommandEndRenderPass(m_vkCommandBuffer, m_ptrFrameBuffer, m_ptrRenderPass));
	}
}

void CVKCommandBuffer::CmdBindPipelineCompute(const CGfxPipelineCompute* pPipelineCompute)
{
	m_pCurrentPipelineCompute = (CVKPipelineCompute*)pPipelineCompute;
	m_pCommands.emplace_back(new CVKCommandBindPipelineCompute(m_vkCommandBuffer, pPipelineCompute));
}

void CVKCommandBuffer::CmdBindPipelineGraphics(const CGfxPipelineGraphics* pPipelineGraphics)
{
	m_pCurrentPipelineGraphics = (CVKPipelineGraphics*)pPipelineGraphics;
	m_pCommands.emplace_back(new CVKCommandBindPipelineGraphics(m_vkCommandBuffer, pPipelineGraphics));
}

void CVKCommandBuffer::CmdBindDescriptorSet(const CGfxDescriptorSetPtr ptrDescriptorSet)
{
	m_pCommands.emplace_back(new CVKCommandBindDescriptorSet(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, ptrDescriptorSet));
}

void CVKCommandBuffer::CmdBindMesh(const CGfxMeshPtr ptrMesh)
{
	m_pCommands.emplace_back(new CVKCommandBindMesh(m_vkCommandBuffer, m_pCurrentPipelineGraphics, ptrMesh));
}

void CVKCommandBuffer::CmdBindMeshDraw(const CGfxMeshDrawPtr ptrMeshDraw)
{
	m_pCommands.emplace_back(new CVKCommandBindMeshDraw(m_vkCommandBuffer, m_pCurrentPipelineGraphics, ptrMeshDraw));
}

void CVKCommandBuffer::CmdUniform1i(uint32_t name, int v0)
{
	m_pCommands.emplace_back(new CVKCommandUniform1i(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0));
}

void CVKCommandBuffer::CmdUniform2i(uint32_t name, int v0, int v1)
{
	m_pCommands.emplace_back(new CVKCommandUniform2i(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0, v1));
}

void CVKCommandBuffer::CmdUniform3i(uint32_t name, int v0, int v1, int v2)
{
	m_pCommands.emplace_back(new CVKCommandUniform3i(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0, v1, v2));
}

void CVKCommandBuffer::CmdUniform4i(uint32_t name, int v0, int v1, int v2, int v3)
{
	m_pCommands.emplace_back(new CVKCommandUniform4i(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0, v1, v2, v3));
}

void CVKCommandBuffer::CmdUniform1f(uint32_t name, float v0)
{
	m_pCommands.emplace_back(new CVKCommandUniform1f(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0));
}

void CVKCommandBuffer::CmdUniform2f(uint32_t name, float v0, float v1)
{
	m_pCommands.emplace_back(new CVKCommandUniform2f(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0, v1));
}

void CVKCommandBuffer::CmdUniform3f(uint32_t name, float v0, float v1, float v2)
{
	m_pCommands.emplace_back(new CVKCommandUniform3f(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0, v1, v2));
}

void CVKCommandBuffer::CmdUniform4f(uint32_t name, float v0, float v1, float v2, float v3)
{
	m_pCommands.emplace_back(new CVKCommandUniform4f(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0, v1, v2, v3));
}

void CVKCommandBuffer::CmdUniform1iv(uint32_t name, int count, const int* value)
{
	m_pCommands.emplace_back(new CVKCommandUniform1iv(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
}

void CVKCommandBuffer::CmdUniform2iv(uint32_t name, int count, const int* value)
{
	m_pCommands.emplace_back(new CVKCommandUniform2iv(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
}

void CVKCommandBuffer::CmdUniform3iv(uint32_t name, int count, const int* value)
{
	m_pCommands.emplace_back(new CVKCommandUniform3iv(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
}

void CVKCommandBuffer::CmdUniform4iv(uint32_t name, int count, const int* value)
{
	m_pCommands.emplace_back(new CVKCommandUniform4iv(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
}

void CVKCommandBuffer::CmdUniform1fv(uint32_t name, int count, const float* value)
{
	m_pCommands.emplace_back(new CVKCommandUniform1fv(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
}

void CVKCommandBuffer::CmdUniform2fv(uint32_t name, int count, const float* value)
{
	m_pCommands.emplace_back(new CVKCommandUniform2fv(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
}

void CVKCommandBuffer::CmdUniform3fv(uint32_t name, int count, const float* value)
{
	m_pCommands.emplace_back(new CVKCommandUniform3fv(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
}

void CVKCommandBuffer::CmdUniform4fv(uint32_t name, int count, const float* value)
{
	m_pCommands.emplace_back(new CVKCommandUniform4fv(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
}

void CVKCommandBuffer::CmdUniformMatrix2fv(uint32_t name, int count, const float* value)
{
	m_pCommands.emplace_back(new CVKCommandUniformMatrix2fv(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
}

void CVKCommandBuffer::CmdUniformMatrix3fv(uint32_t name, int count, const float* value)
{
	m_pCommands.emplace_back(new CVKCommandUniformMatrix3fv(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
}

void CVKCommandBuffer::CmdUniformMatrix4fv(uint32_t name, int count, const float* value)
{
	m_pCommands.emplace_back(new CVKCommandUniformMatrix4fv(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
}

void CVKCommandBuffer::CmdSetScissor(int x, int y, int width, int height)
{
	m_pCommands.emplace_back(new CVKCommandSetScissor(m_vkCommandBuffer, x, y, width, height));
}

void CVKCommandBuffer::CmdSetViewport(int x, int y, int width, int height)
{
	m_pCommands.emplace_back(new CVKCommandSetViewport(m_vkCommandBuffer, x, y, width, height));
}

void CVKCommandBuffer::CmdClearDepth(float depth)
{
	m_pCommands.emplace_back(new CVKCommandClearDepth(m_vkCommandBuffer, depth));
}

void CVKCommandBuffer::CmdClearColor(float red, float green, float blue, float alpha)
{
	m_pCommands.emplace_back(new CVKCommandClearColor(m_vkCommandBuffer, red, green, blue, alpha));
}

void CVKCommandBuffer::CmdDrawInstance(const CGfxMeshDrawPtr ptrMeshDraw)
{
	m_pCommands.emplace_back(new CVKCommandDrawInstance(m_vkCommandBuffer, m_pCurrentPipelineGraphics, ptrMeshDraw));
}

void CVKCommandBuffer::CmdUpdateInstanceBuffer(const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size)
{
	m_pCommands.emplace_back(new CVKCommandUpdateInstanceBuffer(m_vkCommandBuffer, ptrMeshDraw, pInstanceBuffer, size));
}

void CVKCommandBuffer::CmdDispatch(int numLocalWorkGroupX, int numLocalWorkGroupY, int numLocalWorkGroupZ)
{
	m_pCommands.emplace_back(new CVKCommandDispatch(m_vkCommandBuffer, numLocalWorkGroupX, numLocalWorkGroupY, numLocalWorkGroupZ));
}

void CVKCommandBuffer::CmdPushDebugGroup(const char* szMessage)
{
	m_pCommands.emplace_back(new CVKCommandPushDebugGroup(m_vkCommandBuffer, szMessage));
}

void CVKCommandBuffer::CmdPopDebugGroup(void)
{
	m_pCommands.emplace_back(new CVKCommandPopDebugGroup(m_vkCommandBuffer));
}

void CVKCommandBuffer::CmdExecute(const CGfxCommandBufferPtr ptrCommandBuffer)
{
	m_pCommands.emplace_back(new CVKCommandExecute(m_vkCommandBuffer, ptrCommandBuffer));
}
