#include "VKRenderer.h"


CVKCommandBuffer::CVKCommandBuffer(CVKDevice* pDevice, CVKCommandBufferManager* pManager, VkCommandPool vkCommandPool, bool bMainCommandBuffer)
	: CGfxCommandBuffer(bMainCommandBuffer)
	, m_pDevice(pDevice)
	, m_pManager(pManager)

	, m_vkFence(VK_NULL_HANDLE)
	, m_vkCommandPool(VK_NULL_HANDLE)
	, m_vkCommandBuffer(VK_NULL_HANDLE)

	, m_indexSubpass(-1)
{
	ASSERT(m_pDevice);
	ASSERT(m_pManager);

	Create(vkCommandPool, bMainCommandBuffer);
}

CVKCommandBuffer::~CVKCommandBuffer(void)
{
	Destroy();
}

void CVKCommandBuffer::Release(void)
{
	m_pManager->Destroy(this);
}

bool CVKCommandBuffer::Create(VkCommandPool vkCommandPool, bool bMainCommandBuffer)
{
	Destroy();
	{
		do {
			ASSERT(vkCommandPool);

			m_vkCommandPool = vkCommandPool;

			if (bMainCommandBuffer) {
				VkFenceCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
				createInfo.pNext = nullptr;
				createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
				CALL_VK_FUNCTION_BREAK(vkCreateFence(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkFence));
			}

			VkCommandBufferAllocateInfo allocateInfo = {};
			allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocateInfo.pNext = nullptr;
			allocateInfo.commandPool = vkCommandPool;
			allocateInfo.level = bMainCommandBuffer ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
			allocateInfo.commandBufferCount = 1;
			CALL_VK_FUNCTION_BREAK(vkAllocateCommandBuffers(m_pDevice->GetDevice(), &allocateInfo, &m_vkCommandBuffer));

			return true;
		} while (false);
	}
	Destroy();
	return false;
}

void CVKCommandBuffer::Destroy(void)
{
	Clearup();

	if (m_vkFence) {
		vkDestroyFence(m_pDevice->GetDevice(), m_vkFence, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (m_vkCommandBuffer) {
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

	for (const auto& itCommand : m_pCommands) {
		delete itCommand;
	}

	m_pCommands.clear();

	m_indexSubpass = -1;
	m_ptrRenderPass.Release();
	m_ptrFrameBuffer.Release();
}

bool CVKCommandBuffer::Execute(void) const
{
	return true;
}

bool CVKCommandBuffer::WaitForFinish(void) const
{
	if (m_vkFence) {
		CALL_VK_FUNCTION_RETURN_BOOL(vkWaitForFences(m_pDevice->GetDevice(), 1, &m_vkFence, VK_TRUE, UINT64_MAX));
	}

	return true;
}

bool CVKCommandBuffer::CmdBeginRenderPass(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass)
{
	ASSERT(ptrRenderPass);
	ASSERT(ptrFrameBuffer);

	if (IsMainCommandBuffer() == true && IsInRenderPass() == false && m_pCommands.empty()) {
		m_indexSubpass = 0;
		m_ptrRenderPass = ptrRenderPass;
		m_ptrFrameBuffer = ptrFrameBuffer;

		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdNextSubpass(void)
{
	if (IsMainCommandBuffer() == true && IsInRenderPass() == true && m_indexSubpass < (int)m_ptrRenderPass->GetSubpassCount() - 1) {
		m_indexSubpass += 1;
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdEndRenderPass(void)
{
	if (IsMainCommandBuffer() == true && IsInRenderPass() == true) {
		m_indexSubpass = -1;
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdBindPipelineCompute(const CGfxPipelineCompute* pPipelineCompute)
{
	ASSERT(pPipelineCompute);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdBindPipelineGraphics(const CGfxPipelineGraphics* pPipelineGraphics)
{
	ASSERT(pPipelineGraphics);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdBindDescriptorSet(const CGfxDescriptorSetPtr ptrDescriptorSet)
{
	ASSERT(ptrDescriptorSet);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform1i(uint32_t name, int v0)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform2i(uint32_t name, int v0, int v1)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform3i(uint32_t name, int v0, int v1, int v2)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform4i(uint32_t name, int v0, int v1, int v2, int v3)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform1f(uint32_t name, float v0)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform2f(uint32_t name, float v0, float v1)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform3f(uint32_t name, float v0, float v1, float v2)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform4f(uint32_t name, float v0, float v1, float v2, float v3)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform1iv(uint32_t name, int count, const int* value)
{
	ASSERT(value);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform2iv(uint32_t name, int count, const int* value)
{
	ASSERT(value);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform3iv(uint32_t name, int count, const int* value)
{
	ASSERT(value);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform4iv(uint32_t name, int count, const int* value)
{
	ASSERT(value);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform1fv(uint32_t name, int count, const float* value)
{
	ASSERT(value);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform2fv(uint32_t name, int count, const float* value)
{
	ASSERT(value);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform3fv(uint32_t name, int count, const float* value)
{
	ASSERT(value);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform4fv(uint32_t name, int count, const float* value)
{
	ASSERT(value);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniformMatrix2fv(uint32_t name, int count, const float* value)
{
	ASSERT(value);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniformMatrix3fv(uint32_t name, int count, const float* value)
{
	ASSERT(value);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniformMatrix4fv(uint32_t name, int count, const float* value)
{
	ASSERT(value);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdSetScissor(int x, int y, int width, int height)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdSetViewport(int x, int y, int width, int height)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdClearDepth(float depth)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdClearColor(float red, float green, float blue, float alpha)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdDrawInstance(const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdDrawIndirect(const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdExecute(const CGfxCommandBufferPtr ptrCommandBuffer)
{
	ASSERT(ptrCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdPresent(void)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdPushDebugGroup(const char* szMessage)
{
	return true;
}

bool CVKCommandBuffer::CmdPopDebugGroup(void)
{
	return true;
}
