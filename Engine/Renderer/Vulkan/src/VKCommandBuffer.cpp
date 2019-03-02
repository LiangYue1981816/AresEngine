#include "VKRenderer.h"


CVKCommandBuffer::CVKCommandBuffer(CVKDevice *pDevice, CVKCommandPool *pCommandPool, bool bMainCommandBuffer)
	: CGfxCommandBuffer(bMainCommandBuffer)
	, m_pDevice(pDevice)
	, m_pCommandPool(pCommandPool)

	, m_bMainCommandBuffer(bMainCommandBuffer)
	, m_bInRenderPass(false)

	, m_vkFence(VK_NULL_HANDLE)
	, m_vkCommandBuffer(VK_NULL_HANDLE)
{
	VkFenceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	vkCreateFence(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkFence);

	VkCommandBufferAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocateInfo.pNext = nullptr;
	allocateInfo.commandPool = m_pCommandPool->GetCommandPool();
	allocateInfo.level = m_bMainCommandBuffer ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
	allocateInfo.commandBufferCount = 1;
	vkAllocateCommandBuffers(m_pDevice->GetDevice(), &allocateInfo, &m_vkCommandBuffer);
}

CVKCommandBuffer::~CVKCommandBuffer(void)
{
	Clearup();

	if (m_vkFence) {
		vkDestroyFence(m_pDevice->GetDevice(), m_vkFence, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (m_vkCommandBuffer) {
		vkFreeCommandBuffers(m_pDevice->GetDevice(), m_pCommandPool->GetCommandPool(), 1, &m_vkCommandBuffer);
	}

	m_vkFence = VK_NULL_HANDLE;
	m_vkCommandBuffer = VK_NULL_HANDLE;
}

void CVKCommandBuffer::Release(void)
{
	m_pCommandPool->FreeCommandBuffer(this);
}

bool CVKCommandBuffer::IsMainCommandBuffer(void) const
{
	return m_bMainCommandBuffer;
}

void CVKCommandBuffer::Clearup(void)
{
	for (const auto &itCommand : m_pCommands) {
		delete itCommand;
	}

	m_pCommands.clear();
	m_bInRenderPass = false;
}

bool CVKCommandBuffer::Execute(void) const
{
	vkResetFences(m_pDevice->GetDevice(), 1, &m_vkFence);
	return true;
}

bool CVKCommandBuffer::Wait(uint64_t timeout) const
{
	return vkWaitForFences(m_pDevice->GetDevice(), 1, &m_vkFence, VK_TRUE, timeout) == VK_SUCCESS;
}

bool CVKCommandBuffer::CmdBeginRenderPass(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass)
{
	if (m_bMainCommandBuffer == true && m_bInRenderPass == false && m_pCommands.empty()) {
		m_bInRenderPass = true;
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdNextSubpass(void)
{
	if (m_bMainCommandBuffer == true && m_bInRenderPass == true) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdEndRenderPass(void)
{
	if (m_bMainCommandBuffer == true && m_bInRenderPass == true) {
		m_bInRenderPass = false;
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdBindPipelineCompute(const CGfxPipelineCompute *pPipelineCompute)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdBindPipelineGraphics(const CGfxPipelineGraphics *pPipelineGraphics)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdBindMaterialPass(const CGfxMaterialPtr ptrMaterial, uint32_t namePass)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdBindUniformBuffer(const CGfxUniformBufferPtr ptrUniformBuffer, uint32_t nameUniform)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdBindMeshDraw(const CGfxMeshDrawPtr ptrMeshDraw)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdUniform1i(const char *szName, int v0)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdUniform2i(const char *szName, int v0, int v1)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdUniform3i(const char *szName, int v0, int v1, int v2)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdUniform4i(const char *szName, int v0, int v1, int v2, int v3)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdUniform1f(const char *szName, float v0)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdUniform2f(const char *szName, float v0, float v1)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdUniform3f(const char *szName, float v0, float v1, float v2)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdUniform4f(const char *szName, float v0, float v1, float v2, float v3)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdUniform1iv(const char *szName, int count, const int *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdUniform2iv(const char *szName, int count, const int *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdUniform3iv(const char *szName, int count, const int *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdUniform4iv(const char *szName, int count, const int *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdUniform1fv(const char *szName, int count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdUniform2fv(const char *szName, int count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdUniform3fv(const char *szName, int count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdUniform4fv(const char *szName, int count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdUniformMatrix2fv(const char *szName, int count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdUniformMatrix3fv(const char *szName, int count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdUniformMatrix4fv(const char *szName, int count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdClearDepth(float depth)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdClearColor(float red, float green, float blue, float alpha)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdSetScissor(int x, int y, int width, int height)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdSetViewport(int x, int y, int width, int height)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdSetInstanceBufferData(const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t *pInstanceBuffer, uint32_t size)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdDrawElements(GfxIndexType type, int offset, int count)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdDrawInstance(GfxIndexType type, int offset, int count, int instanceCount)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdDrawIndirect(GfxIndexType type, int offset)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdExecute(const CGfxCommandBufferPtr ptrCommandBuffer)
{
	if (m_bMainCommandBuffer == true && m_bInRenderPass == true && ptrCommandBuffer->IsMainCommandBuffer() == false) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdPresent(void)
{
	if (m_bMainCommandBuffer == true && m_bInRenderPass == false) {
		return true;
	}

	return false;
}

bool CVKCommandBuffer::CmdPushDebugGroup(const char *szMessage)
{
	return true;
}

bool CVKCommandBuffer::CmdPopDebugGroup(void)
{
	return true;
}
