#include "GLES3Renderer.h"
#include "./Command/GLES3CommandBeginRenderPass.h"
#include "./Command/GLES3CommandNextSubPass.h"
#include "./Command/GLES3CommandEndRenderPass.h"
#include "./Command/GLES3CommandResolve.h"
#include "./Command/GLES3CommandInvalidateFramebuffer.h"
#include "./Command/GLES3CommandBindFrameBuffer.h"
#include "./Command/GLES3CommandBindPipelineCompute.h"
#include "./Command/GLES3CommandBindPipelineGraphics.h"
#include "./Command/GLES3CommandBindDescriptorSet.h"
#include "./Command/GLES3CommandUniform1i.h"
#include "./Command/GLES3CommandUniform2i.h"
#include "./Command/GLES3CommandUniform3i.h"
#include "./Command/GLES3CommandUniform4i.h"
#include "./Command/GLES3CommandUniform1f.h"
#include "./Command/GLES3CommandUniform2f.h"
#include "./Command/GLES3CommandUniform3f.h"
#include "./Command/GLES3CommandUniform4f.h"
#include "./Command/GLES3CommandUniform1iv.h"
#include "./Command/GLES3CommandUniform2iv.h"
#include "./Command/GLES3CommandUniform3iv.h"
#include "./Command/GLES3CommandUniform4iv.h"
#include "./Command/GLES3CommandUniform1fv.h"
#include "./Command/GLES3CommandUniform2fv.h"
#include "./Command/GLES3CommandUniform3fv.h"
#include "./Command/GLES3CommandUniform4fv.h"
#include "./Command/GLES3CommandUniformMatrix2fv.h"
#include "./Command/GLES3CommandUniformMatrix3fv.h"
#include "./Command/GLES3CommandUniformMatrix4fv.h"
#include "./Command/GLES3CommandSetScissor.h"
#include "./Command/GLES3CommandSetViewport.h"
#include "./Command/GLES3CommandClearDepth.h"
#include "./Command/GLES3CommandClearColor.h"
#include "./Command/GLES3CommandDrawInstance.h"
#include "./Command/GLES3CommandDrawIndirect.h"
#include "./Command/GLES3CommandUpdateInstanceBuffer.h"
#include "./Command/GLES3CommandExecute.h"
#include "./Command/GLES3CommandPushDebugGroup.h"
#include "./Command/GLES3CommandPopDebugGroup.h"


CGLES3CommandBuffer::CGLES3CommandBuffer(CGLES3CommandBufferManager* pManager, bool bMainCommandBuffer)
	: CGfxCommandBuffer(bMainCommandBuffer)
	, m_pManager(pManager)

	, m_indexSubpass(-1)
{
	ASSERT(m_pManager);
}

CGLES3CommandBuffer::~CGLES3CommandBuffer(void)
{
	Clearup();
}

void CGLES3CommandBuffer::Release(void)
{
	m_pManager->Destroy(this);
}

bool CGLES3CommandBuffer::IsInRenderPass(void) const
{
	return m_ptrRenderPass && m_indexSubpass >= 0 && m_indexSubpass < (int)m_ptrRenderPass->GetSubpassCount();
}

int CGLES3CommandBuffer::GetSubpassIndex(void) const
{
	return m_indexSubpass;
}

const CGfxRenderPassPtr CGLES3CommandBuffer::GetRenderPass(void) const
{
	return m_ptrRenderPass;
}

const CGfxFrameBufferPtr CGLES3CommandBuffer::GetFrameBuffer(void) const
{
	return m_ptrFrameBuffer;
}

void CGLES3CommandBuffer::Clearup(void)
{
	for (const auto& itCommand : m_pCommands) {
		delete itCommand;
	}

	m_pCommands.clear();

	m_indexSubpass = -1;
	m_ptrRenderPass.Release();
	m_ptrFrameBuffer.Release();
}

bool CGLES3CommandBuffer::Execute(void) const
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == false)) {
		for (const auto& itCommand : m_pCommands) {
			itCommand->Execute();
		}

		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::WaitForFinish(void) const
{
	return true;
}

bool CGLES3CommandBuffer::BeginRecord(void)
{
	if (IsMainCommandBuffer() && IsInRenderPass() == false && m_pCommands.empty()) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::BeginRecord(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass)
{
	ASSERT(ptrRenderPass);
	ASSERT(ptrFrameBuffer);

	if (IsMainCommandBuffer() == false && IsInRenderPass() == false && m_pCommands.empty()) {
		m_indexSubpass = indexSubpass;
		m_ptrRenderPass = ptrRenderPass;
		m_ptrFrameBuffer = ptrFrameBuffer;
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::EndRecord(void)
{
	return true;
}

bool CGLES3CommandBuffer::CmdBeginRenderPass(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass)
{
	ASSERT(ptrRenderPass);
	ASSERT(ptrFrameBuffer);

	if (IsMainCommandBuffer() == true && IsInRenderPass() == false) {
		m_indexSubpass = 0;
		m_ptrRenderPass = ptrRenderPass;
		m_ptrFrameBuffer = ptrFrameBuffer;

		m_pCommands.emplace_back(new CGLES3CommandBeginRenderPass(m_ptrFrameBuffer, m_ptrRenderPass));
		m_pCommands.emplace_back(new CGLES3CommandBindFrameBuffer(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));

		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdNextSubpass(void)
{
	if (IsMainCommandBuffer() == true && IsInRenderPass() == true && m_indexSubpass < (int)m_ptrRenderPass->GetSubpassCount() - 1) {
		m_pCommands.emplace_back(new CGLES3CommandInvalidateFramebuffer(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));
		m_pCommands.emplace_back(new CGLES3CommandResolve(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));

		m_indexSubpass += 1;

		m_pCommands.emplace_back(new CGLES3CommandNextSubPass(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));
		m_pCommands.emplace_back(new CGLES3CommandBindFrameBuffer(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));

		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdEndRenderPass(void)
{
	if (IsMainCommandBuffer() == true && IsInRenderPass() == true) {
		m_pCommands.emplace_back(new CGLES3CommandInvalidateFramebuffer(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));
		m_pCommands.emplace_back(new CGLES3CommandResolve(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));
		m_pCommands.emplace_back(new CGLES3CommandEndRenderPass(m_ptrFrameBuffer, m_ptrRenderPass));

		m_indexSubpass = -1;

		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdBindPipelineCompute(const CGfxPipelineCompute* pPipelineCompute)
{
	ASSERT(pPipelineCompute);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandBindPipelineCompute(pPipelineCompute));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdBindPipelineGraphics(const CGfxPipelineGraphics* pPipelineGraphics)
{
	ASSERT(pPipelineGraphics);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandBindPipelineGraphics(pPipelineGraphics));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdBindDescriptorSet(const CGfxDescriptorSetPtr ptrDescriptorSet)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandBindDescriptorSet(ptrDescriptorSet));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform1i(uint32_t name, int v0)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform1i(name, v0));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform2i(uint32_t name, int v0, int v1)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform2i(name, v0, v1));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform3i(uint32_t name, int v0, int v1, int v2)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform3i(name, v0, v1, v2));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform4i(uint32_t name, int v0, int v1, int v2, int v3)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform4i(name, v0, v1, v2, v3));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform1f(uint32_t name, float v0)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform1f(name, v0));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform2f(uint32_t name, float v0, float v1)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform2f(name, v0, v1));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform3f(uint32_t name, float v0, float v1, float v2)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform3f(name, v0, v1, v2));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform4f(uint32_t name, float v0, float v1, float v2, float v3)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform4f(name, v0, v1, v2, v3));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform1iv(uint32_t name, int count, const int* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform1iv(name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform2iv(uint32_t name, int count, const int* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform2iv(name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform3iv(uint32_t name, int count, const int* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform3iv(name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform4iv(uint32_t name, int count, const int* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform4iv(name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform1fv(uint32_t name, int count, const float* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform1fv(name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform2fv(uint32_t name, int count, const float* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform2fv(name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform3fv(uint32_t name, int count, const float* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform3fv(name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform4fv(uint32_t name, int count, const float* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform4fv(name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniformMatrix2fv(uint32_t name, int count, const float* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniformMatrix2fv(name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniformMatrix3fv(uint32_t name, int count, const float* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniformMatrix3fv(name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniformMatrix4fv(uint32_t name, int count, const float* value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniformMatrix4fv(name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdSetScissor(int x, int y, int width, int height)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandSetScissor(x, y, width, height));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdSetViewport(int x, int y, int width, int height)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandSetViewport(x, y, width, height));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdClearDepth(float depth)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandClearDepth(depth));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdClearColor(float red, float green, float blue, float alpha)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandClearColor(red, green, blue, alpha));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdDrawInstance(const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUpdateInstanceBuffer(ptrMeshDraw, pInstanceBuffer, size));
		m_pCommands.emplace_back(new CGLES3CommandDrawInstance(ptrMeshDraw));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdDrawIndirect(const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUpdateInstanceBuffer(ptrMeshDraw, pInstanceBuffer, size));
		m_pCommands.emplace_back(new CGLES3CommandDrawIndirect(ptrMeshDraw));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdExecute(const CGfxCommandBufferPtr ptrCommandBuffer)
{
	if (IsMainCommandBuffer() == true && IsInRenderPass() == true && ptrCommandBuffer->IsMainCommandBuffer() == false) {
		m_pCommands.emplace_back(new CGLES3CommandExecute(ptrCommandBuffer));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdPushDebugGroup(const char* szMessage)
{
	m_pCommands.emplace_back(new CGLES3CommandPushDebugGroup(szMessage));
	return true;
}

bool CGLES3CommandBuffer::CmdPopDebugGroup(void)
{
	m_pCommands.emplace_back(new CGLES3CommandPopDebugGroup());
	return true;
}
