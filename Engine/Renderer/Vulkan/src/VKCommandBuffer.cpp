#include "VKRenderer.h"
#include "./Command/VKCommandBeginRecord.h"
#include "./Command/VKCommandEndRecord.h"
#include "./Command/VKCommandSetImageLayout.h"
#include "./Command/VKCommandBeginRenderPass.h"
#include "./Command/VKCommandNextSubPass.h"
#include "./Command/VKCommandEndRenderPass.h"
#include "./Command/VKCommandBindPipelineCompute.h"
#include "./Command/VKCommandBindPipelineGraphics.h"
#include "./Command/VKCommandBindDescriptorSet.h"
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
#include "./Command/VKCommandDrawInstance.h"
#include "./Command/VKCommandDrawIndirect.h"
#include "./Command/VKCommandUpdateInstanceBuffer.h"
#include "./Command/VKCommandExecute.h"
#include "./Command/VKCommandPushDebugGroup.h"
#include "./Command/VKCommandPopDebugGroup.h"


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

	if (m_vkCommandBuffer) {
		vkResetCommandBuffer(m_vkCommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
	}

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

bool CVKCommandBuffer::IsEmpty(void) const
{
	return m_pCommands.empty();
}

bool CVKCommandBuffer::BeginRecord(void)
{
	if (IsMainCommandBuffer() && IsInRenderPass() == false && m_pCommands.empty()) {
		m_pCommands.emplace_back(new CVKCommandBeginRecord(m_vkCommandBuffer));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::BeginRecord(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass)
{
	ASSERT(ptrRenderPass);
	ASSERT(ptrFrameBuffer);

	if (IsMainCommandBuffer() == false && IsInRenderPass() == false && m_pCommands.empty()) {
		m_indexSubpass = indexSubpass;
		m_ptrRenderPass = ptrRenderPass;
		m_ptrFrameBuffer = ptrFrameBuffer;
		m_pCommands.emplace_back(new CVKCommandBeginRecord(m_vkCommandBuffer, m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::EndRecord(void)
{
	m_pCommands.emplace_back(new CVKCommandEndRecord(m_vkCommandBuffer));
	return true;
}

bool CVKCommandBuffer::CmdSetImageLayout(const CGfxTexture2DPtr ptrTexture, GfxImageLayout imageLayout)
{
	ASSERT(ptrTexture);

	if (IsMainCommandBuffer() == true && IsInRenderPass() == false) {
		m_pCommands.emplace_back(new CVKCommandSetImageLayout(m_vkCommandBuffer, ptrTexture, imageLayout));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdSetImageLayout(const CGfxTexture2DArrayPtr ptrTexture, GfxImageLayout imageLayout)
{
	ASSERT(ptrTexture);

	if (IsMainCommandBuffer() == true && IsInRenderPass() == false) {
		m_pCommands.emplace_back(new CVKCommandSetImageLayout(m_vkCommandBuffer, ptrTexture, imageLayout));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdSetImageLayout(const CGfxTextureCubemapPtr ptrTexture, GfxImageLayout imageLayout)
{
	ASSERT(ptrTexture);

	if (IsMainCommandBuffer() == true && IsInRenderPass() == false) {
		m_pCommands.emplace_back(new CVKCommandSetImageLayout(m_vkCommandBuffer, ptrTexture, imageLayout));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdSetImageLayout(const CGfxRenderTexturePtr ptrTexture, GfxImageLayout imageLayout)
{
	ASSERT(ptrTexture);

	if (IsMainCommandBuffer() == true && IsInRenderPass() == false) {
		m_pCommands.emplace_back(new CVKCommandSetImageLayout(m_vkCommandBuffer, ptrTexture, imageLayout));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdBeginRenderPass(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass)
{
	ASSERT(ptrRenderPass);
	ASSERT(ptrFrameBuffer);

	if (IsMainCommandBuffer() == true && IsInRenderPass() == false) {
		m_indexSubpass = 0;
		m_ptrRenderPass = ptrRenderPass;
		m_ptrFrameBuffer = ptrFrameBuffer;
		m_pCommands.emplace_back(new CVKCommandBeginRenderPass(m_vkCommandBuffer, m_ptrFrameBuffer, m_ptrRenderPass));
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
		m_pCommands.emplace_back(new CVKCommandNextSubPass(m_vkCommandBuffer, m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));
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
		m_pCommands.emplace_back(new CVKCommandEndRenderPass(m_vkCommandBuffer, m_ptrFrameBuffer, m_ptrRenderPass));
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
		m_pCommands.emplace_back(new CVKCommandBindPipelineCompute(m_vkCommandBuffer, pPipelineCompute));
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
		m_pCommands.emplace_back(new CVKCommandBindPipelineGraphics(m_vkCommandBuffer, pPipelineGraphics));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdBindDescriptorSet(const CGfxDescriptorSetPtr ptrDescriptorSet)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandBindDescriptorSet(m_vkCommandBuffer, ptrDescriptorSet));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform1i(uint32_t name, int v0)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform1i(m_vkCommandBuffer, name, v0));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform2i(uint32_t name, int v0, int v1)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform2i(m_vkCommandBuffer, name, v0, v1));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform3i(uint32_t name, int v0, int v1, int v2)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform3i(m_vkCommandBuffer, name, v0, v1, v2));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform4i(uint32_t name, int v0, int v1, int v2, int v3)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform4i(m_vkCommandBuffer, name, v0, v1, v2, v3));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform1f(uint32_t name, float v0)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform1f(m_vkCommandBuffer, name, v0));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform2f(uint32_t name, float v0, float v1)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform2f(m_vkCommandBuffer, name, v0, v1));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform3f(uint32_t name, float v0, float v1, float v2)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform3f(m_vkCommandBuffer, name, v0, v1, v2));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform4f(uint32_t name, float v0, float v1, float v2, float v3)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform4f(m_vkCommandBuffer, name, v0, v1, v2, v3));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform1iv(uint32_t name, int count, const int* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform1iv(m_vkCommandBuffer, name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform2iv(uint32_t name, int count, const int* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform2iv(m_vkCommandBuffer, name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform3iv(uint32_t name, int count, const int* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform3iv(m_vkCommandBuffer, name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform4iv(uint32_t name, int count, const int* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform4iv(m_vkCommandBuffer, name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform1fv(uint32_t name, int count, const float* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform1fv(m_vkCommandBuffer, name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform2fv(uint32_t name, int count, const float* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform2fv(m_vkCommandBuffer, name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform3fv(uint32_t name, int count, const float* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform3fv(m_vkCommandBuffer, name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform4fv(uint32_t name, int count, const float* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform4fv(m_vkCommandBuffer, name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniformMatrix2fv(uint32_t name, int count, const float* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniformMatrix2fv(m_vkCommandBuffer, name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniformMatrix3fv(uint32_t name, int count, const float* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniformMatrix3fv(m_vkCommandBuffer, name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniformMatrix4fv(uint32_t name, int count, const float* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniformMatrix4fv(m_vkCommandBuffer, name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdSetScissor(int x, int y, int width, int height)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandSetScissor(m_vkCommandBuffer, x, y, width, height));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdSetViewport(int x, int y, int width, int height)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandSetViewport(m_vkCommandBuffer, x, y, width, height));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdClearDepth(float depth)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandClearDepth(m_vkCommandBuffer, depth));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdClearColor(float red, float green, float blue, float alpha)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandClearColor(m_vkCommandBuffer, red, green, blue, alpha));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdDrawInstance(const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUpdateInstanceBuffer(m_vkCommandBuffer, ptrMeshDraw, pInstanceBuffer, size));
		m_pCommands.emplace_back(new CVKCommandDrawInstance(m_vkCommandBuffer, ptrMeshDraw));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdDrawIndirect(const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUpdateInstanceBuffer(m_vkCommandBuffer, ptrMeshDraw, pInstanceBuffer, size));
		m_pCommands.emplace_back(new CVKCommandDrawIndirect(m_vkCommandBuffer, ptrMeshDraw));
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
		m_pCommands.emplace_back(new CVKCommandExecute(m_vkCommandBuffer, ptrCommandBuffer));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdPushDebugGroup(const char* szMessage)
{
	m_pCommands.emplace_back(new CVKCommandPushDebugGroup(m_vkCommandBuffer, szMessage));
	return true;
}

bool CVKCommandBuffer::CmdPopDebugGroup(void)
{
	m_pCommands.emplace_back(new CVKCommandPopDebugGroup(m_vkCommandBuffer));
	return true;
}
