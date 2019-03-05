#include "GLES3Renderer.h"
#include "GLES3CommandBeginRenderPass.h"
#include "GLES3CommandEndRenderPass.h"
#include "GLES3CommandResolve.h"
#include "GLES3CommandInvalidateFramebuffer.h"
#include "GLES3CommandBindFrameBuffer.h"
#include "GLES3CommandBindSubpassInputTexture.h"
#include "GLES3CommandBindPipelineCompute.h"
#include "GLES3CommandBindPipelineGraphics.h"
#include "GLES3CommandBindMaterialPass.h"
#include "GLES3CommandBindUniformBuffer.h"
#include "GLES3CommandBindMeshDraw.h"
#include "GLES3CommandUniform1i.h"
#include "GLES3CommandUniform2i.h"
#include "GLES3CommandUniform3i.h"
#include "GLES3CommandUniform4i.h"
#include "GLES3CommandUniform1f.h"
#include "GLES3CommandUniform2f.h"
#include "GLES3CommandUniform3f.h"
#include "GLES3CommandUniform4f.h"
#include "GLES3CommandUniform1iv.h"
#include "GLES3CommandUniform2iv.h"
#include "GLES3CommandUniform3iv.h"
#include "GLES3CommandUniform4iv.h"
#include "GLES3CommandUniform1fv.h"
#include "GLES3CommandUniform2fv.h"
#include "GLES3CommandUniform3fv.h"
#include "GLES3CommandUniform4fv.h"
#include "GLES3CommandUniformMatrix2fv.h"
#include "GLES3CommandUniformMatrix3fv.h"
#include "GLES3CommandUniformMatrix4fv.h"
#include "GLES3CommandSetInstanceBufferData.h"
#include "GLES3CommandSetScissor.h"
#include "GLES3CommandSetViewport.h"
#include "GLES3CommandClearDepth.h"
#include "GLES3CommandClearColor.h"
#include "GLES3CommandDrawInstance.h"
#include "GLES3CommandDrawIndirect.h"
#include "GLES3CommandDrawElements.h"
#include "GLES3CommandExecute.h"
#include "GLES3CommandPushDebugGroup.h"
#include "GLES3CommandPopDebugGroup.h"


CGLES3CommandBuffer::CGLES3CommandBuffer(CGLES3CommandBufferManager *pManager, bool bMainCommandBuffer)
	: CGfxCommandBuffer(bMainCommandBuffer)
	, m_pManager(pManager)

	, m_bMainCommandBuffer(bMainCommandBuffer)
	, m_bInRenderPass(false)
	, m_indexSubpass(0)
{

}

CGLES3CommandBuffer::~CGLES3CommandBuffer(void)
{
	Clearup();
}

void CGLES3CommandBuffer::Release(void)
{
	m_pManager->Destroy(this);
}

HANDLE CGLES3CommandBuffer::GetFence(void) const
{
	return nullptr;
}

HANDLE CGLES3CommandBuffer::GetCommandBuffer(void) const
{
	return nullptr;
}

bool CGLES3CommandBuffer::IsMainCommandBuffer(void) const
{
	return m_bMainCommandBuffer;
}

void CGLES3CommandBuffer::Clearup(void)
{
	for (const auto &itCommand : m_pCommands) {
		delete itCommand;
	}

	m_pCommands.clear();

	m_indexSubpass = 0;
	m_bInRenderPass = false;
	m_ptrRenderPass.Release();
	m_ptrFrameBuffer.Release();
}

bool CGLES3CommandBuffer::Execute(void) const
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == false)) {
		for (const auto &itCommand : m_pCommands) {
			itCommand->Execute();
		}

		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdBeginRenderPass(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass)
{
	if (m_bMainCommandBuffer == true && m_bInRenderPass == false && m_pCommands.empty()) {
		m_indexSubpass = 0;
		m_bInRenderPass = true;
		m_ptrRenderPass = ptrRenderPass;
		m_ptrFrameBuffer = ptrFrameBuffer;

		m_pCommands.emplace_back(new CGLES3CommandBeginRenderPass(m_ptrFrameBuffer, m_ptrRenderPass));
		m_pCommands.emplace_back(new CGLES3CommandBindFrameBuffer(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));
		m_pCommands.emplace_back(new CGLES3CommandBindSubpassInputTexture(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));

		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdNextSubpass(void)
{
	if (m_bMainCommandBuffer == true && m_bInRenderPass == true) {
		m_pCommands.emplace_back(new CGLES3CommandInvalidateFramebuffer(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));
		m_pCommands.emplace_back(new CGLES3CommandResolve(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));
		m_indexSubpass += 1;
		m_pCommands.emplace_back(new CGLES3CommandBindFrameBuffer(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));
		m_pCommands.emplace_back(new CGLES3CommandBindSubpassInputTexture(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));

		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdEndRenderPass(void)
{
	if (m_bMainCommandBuffer == true && m_bInRenderPass == true) {
		m_bInRenderPass = false;

		m_pCommands.emplace_back(new CGLES3CommandInvalidateFramebuffer(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));
		m_pCommands.emplace_back(new CGLES3CommandResolve(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));
		m_pCommands.emplace_back(new CGLES3CommandEndRenderPass(m_ptrFrameBuffer, m_ptrRenderPass));

		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdBindPipelineCompute(const CGfxPipelineCompute *pPipelineCompute)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandBindPipelineCompute(pPipelineCompute));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdBindPipelineGraphics(const CGfxPipelineGraphics *pPipelineGraphics)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandBindPipelineGraphics(pPipelineGraphics));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdBindMaterialPass(const CGfxMaterialPtr ptrMaterial, uint32_t namePass)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandBindMaterialPass(ptrMaterial, namePass));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdBindUniformBuffer(const CGfxUniformBufferPtr ptrUniformBuffer, uint32_t nameUniform)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandBindUniformBuffer(ptrUniformBuffer, nameUniform));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdBindMeshDraw(const CGfxMeshDrawPtr ptrMeshDraw)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandBindMeshDraw(ptrMeshDraw));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform1i(const char *szName, int v0)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform1i(szName, v0));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform2i(const char *szName, int v0, int v1)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform2i(szName, v0, v1));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform3i(const char *szName, int v0, int v1, int v2)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform3i(szName, v0, v1, v2));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform4i(const char *szName, int v0, int v1, int v2, int v3)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform4i(szName, v0, v1, v2, v3));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform1f(const char *szName, float v0)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform1f(szName, v0));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform2f(const char *szName, float v0, float v1)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform2f(szName, v0, v1));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform3f(const char *szName, float v0, float v1, float v2)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform3f(szName, v0, v1, v2));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform4f(const char *szName, float v0, float v1, float v2, float v3)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform4f(szName, v0, v1, v2, v3));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform1iv(const char *szName, int count, const int *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform1iv(szName, count, value));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform2iv(const char *szName, int count, const int *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform2iv(szName, count, value));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform3iv(const char *szName, int count, const int *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform3iv(szName, count, value));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform4iv(const char *szName, int count, const int *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform4iv(szName, count, value));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform1fv(const char *szName, int count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform1fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform2fv(const char *szName, int count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform2fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform3fv(const char *szName, int count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform3fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform4fv(const char *szName, int count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform4fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniformMatrix2fv(const char *szName, int count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniformMatrix2fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniformMatrix3fv(const char *szName, int count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniformMatrix3fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniformMatrix4fv(const char *szName, int count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniformMatrix4fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdClearDepth(float depth)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandClearDepth(depth));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdClearColor(float red, float green, float blue, float alpha)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandClearColor(red, green, blue, alpha));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdSetScissor(int x, int y, int width, int height)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandSetScissor(x, y, width, height));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdSetViewport(int x, int y, int width, int height)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandSetViewport(x, y, width, height));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdSetInstanceBufferData(const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t *pInstanceBuffer, uint32_t size)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandSetInstanceBufferData(ptrMeshDraw, pInstanceBuffer, size));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdDrawElements(const CGfxMeshDrawPtr ptrMeshDraw)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandDrawElements(ptrMeshDraw));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdDrawInstance(const CGfxMeshDrawPtr ptrMeshDraw)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandDrawInstance(ptrMeshDraw));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdDrawIndirect(const CGfxMeshDrawPtr ptrMeshDraw)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandDrawIndirect(ptrMeshDraw));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdExecute(const CGfxCommandBufferPtr ptrCommandBuffer)
{
	if (m_bMainCommandBuffer == true && m_bInRenderPass == true && ptrCommandBuffer->IsMainCommandBuffer() == false) {
		m_pCommands.emplace_back(new CGLES3CommandExecute(ptrCommandBuffer));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdPresent(void)
{
	if (m_bMainCommandBuffer == true && m_bInRenderPass == false) {
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdPushDebugGroup(const char *szMessage)
{
	m_pCommands.emplace_back(new CGLES3CommandPushDebugGroup(szMessage));
	return true;
}

bool CGLES3CommandBuffer::CmdPopDebugGroup(void)
{
	m_pCommands.emplace_back(new CGLES3CommandPopDebugGroup());
	return true;
}
