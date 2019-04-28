#include "VKRenderer.h"


CVKCommandBuffer::CVKCommandBuffer(CVKDevice* pDevice, CVKCommandPool* pCommandPool, bool bMainCommandBuffer)
	: CGfxCommandBuffer(bMainCommandBuffer)
	, m_pDevice(pDevice)
	, m_pCommandPool(pCommandPool)

	, m_vkFence(VK_NULL_HANDLE)
	, m_vkCommandBuffer(VK_NULL_HANDLE)

	, m_bInRenderPass(false)
	, m_indexSubpass(0)
{

}

CVKCommandBuffer::~CVKCommandBuffer(void)
{

}

void CVKCommandBuffer::Release(void)
{

}

VkFence CVKCommandBuffer::GetFence(void) const
{
	return m_vkFence;
}

VkCommandBuffer CVKCommandBuffer::GetCommandBuffer(void) const
{
	return m_vkCommandBuffer;
}

const CGfxFrameBufferPtr CVKCommandBuffer::GetFrameBuffer(void) const
{
	return m_ptrFrameBuffer;
}

const CGfxRenderPassPtr CVKCommandBuffer::GetRenderPass(void) const
{
	return m_ptrRenderPass;
}

bool CVKCommandBuffer::IsInRenderPass(void) const
{
	return m_bInRenderPass;
}

int CVKCommandBuffer::GetSubpassIndex(void) const
{
	return m_indexSubpass;
}

void CVKCommandBuffer::Clearup(void)
{

}

bool CVKCommandBuffer::Execute(void) const
{

}

bool CVKCommandBuffer::CmdBeginRenderPass(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass)
{
	if (IsMainCommandBuffer() == true && IsInRenderPass() == false && m_pCommands.empty()) {
		m_bInRenderPass = true;
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
	if (IsMainCommandBuffer() == true && IsInRenderPass() == true) {
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
		m_bInRenderPass = false;
		m_indexSubpass = 0;
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdBindPipelineCompute(const CGfxPipelineCompute* pPipelineCompute)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdBindPipelineGraphics(const CGfxPipelineGraphics* pPipelineGraphics)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdBindDescriptorSet(const CGfxDescriptorSetPtr ptrDescriptorSet)
{
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
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform2iv(uint32_t name, int count, const int* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform3iv(uint32_t name, int count, const int* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform4iv(uint32_t name, int count, const int* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform1fv(uint32_t name, int count, const float* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform2fv(uint32_t name, int count, const float* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform3fv(uint32_t name, int count, const float* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform4fv(uint32_t name, int count, const float* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniformMatrix2fv(uint32_t name, int count, const float* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniformMatrix3fv(uint32_t name, int count, const float* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniformMatrix4fv(uint32_t name, int count, const float* value)
{
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

}

bool CVKCommandBuffer::CmdPopDebugGroup(void)
{

}
