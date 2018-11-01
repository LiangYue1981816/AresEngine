#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxCommandBuffer.h"
#include "GfxCommandBeginRenderPass.h"
#include "GfxCommandEndRenderPass.h"
#include "GfxCommandResolve.h"
#include "GfxCommandSetScissor.h"
#include "GfxCommandSetViewport.h"
#include "GfxCommandBindFrameBuffer.h"
#include "GfxCommandBindMesh.h"
#include "GfxCommandBindDrawIndirectBuffer.h"
#include "GfxCommandBindCamera.h"
#include "GfxCommandBindPipeline.h"
#include "GfxCommandBindMaterialPass.h"
#include "GfxCommandBindInputTexture.h"
#include "GfxCommandBindSubPassInputTexture.h"
#include "GfxCommandInvalidateFramebuffer.h"
#include "GfxCommandUniform1i.h"
#include "GfxCommandUniform2i.h"
#include "GfxCommandUniform3i.h"
#include "GfxCommandUniform4i.h"
#include "GfxCommandUniform1f.h"
#include "GfxCommandUniform2f.h"
#include "GfxCommandUniform3f.h"
#include "GfxCommandUniform4f.h"
#include "GfxCommandUniform1iv.h"
#include "GfxCommandUniform2iv.h"
#include "GfxCommandUniform3iv.h"
#include "GfxCommandUniform4iv.h"
#include "GfxCommandUniform1fv.h"
#include "GfxCommandUniform2fv.h"
#include "GfxCommandUniform3fv.h"
#include "GfxCommandUniform4fv.h"
#include "GfxCommandUniformMatrix2fv.h"
#include "GfxCommandUniformMatrix3fv.h"
#include "GfxCommandUniformMatrix4fv.h"
#include "GfxCommandClearDepth.h"
#include "GfxCommandClearColor.h"
#include "GfxCommandDrawInstance.h"
#include "GfxCommandDrawIndirect.h"
#include "GfxCommandDrawElements.h"
#include "GfxCommandExecute.h"


CGfxCommandBuffer::CGfxCommandBuffer(bool bMainCommandBuffer)
	: m_bMainCommandBuffer(bMainCommandBuffer)
	, m_bInPassScope(false)
	, m_indexSubPass(0)
{

}

CGfxCommandBuffer::~CGfxCommandBuffer(void)
{
	Clearup();
}

void CGfxCommandBuffer::Clearup(void)
{
	for (const auto &itCommand : m_pCommands) {
		delete itCommand;
	}

	m_pCommands.clear();

	m_bInPassScope = false;
	m_indexSubPass = 0;
	m_ptrRenderPass.Release();
	m_ptrFrameBuffer.Release();
}

bool CGfxCommandBuffer::Execute(void) const
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == false)) {
		for (const auto &itCommand : m_pCommands) {
			itCommand->Execute();
		}
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdBeginRenderPass(const CGfxFrameBufferPtr &ptrFrameBuffer, const CGfxRenderPassPtr &ptrRenderPass)
{
	if (m_bMainCommandBuffer == true && m_bInPassScope == false && m_pCommands.empty()) {
		m_bInPassScope = true;
		m_indexSubPass = 0;
		m_ptrRenderPass = ptrRenderPass;
		m_ptrFrameBuffer = ptrFrameBuffer;

		m_pCommands.emplace_back(new CGfxCommandBeginRenderPass(m_ptrFrameBuffer, m_ptrRenderPass));
		m_pCommands.emplace_back(new CGfxCommandBindFrameBuffer(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubPass));
		m_pCommands.emplace_back(new CGfxCommandBindSubPassInputTexture(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubPass));

		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdNextSubpass(void)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandInvalidateFramebuffer(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubPass));
		m_pCommands.emplace_back(new CGfxCommandResolve(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubPass));
		m_indexSubPass += 1;
		m_pCommands.emplace_back(new CGfxCommandBindFrameBuffer(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubPass));
		m_pCommands.emplace_back(new CGfxCommandBindSubPassInputTexture(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubPass));

		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdEndRenderPass(void)
{
	if (m_bMainCommandBuffer == true && m_bInPassScope == true) {
		m_bInPassScope = false;

		m_pCommands.emplace_back(new CGfxCommandInvalidateFramebuffer(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubPass));
		m_pCommands.emplace_back(new CGfxCommandResolve(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubPass));
		m_pCommands.emplace_back(new CGfxCommandEndRenderPass(m_ptrFrameBuffer, m_ptrRenderPass));

		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdSetScissor(int x, int y, int width, int height)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandSetScissor(x, y, width, height));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdSetViewport(int x, int y, int width, int height)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandSetViewport(x, y, width, height));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdBindCamera(CGfxCamera *pCamera)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandBindCamera(pCamera));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdBindPipeline(CGfxPipelineBase *pPipeline)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandBindPipeline(pPipeline));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdBindMesh(const CGfxMeshPtr &ptrMesh)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandBindMesh(ptrMesh));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdBindMesh(const CGfxMeshPtr &ptrMesh, const uint8_t *pInstanceBuffer, uint32_t size)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandBindMesh(ptrMesh, pInstanceBuffer, size));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdBindDrawIndirectBuffer(const CGfxDrawIndirectBufferPtr &ptrDrawIndirectBuffer, uint32_t instanceCount)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandBindDrawIndirectBuffer(ptrDrawIndirectBuffer, instanceCount));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdBindMaterialPass(const CGfxMaterialPtr &ptrMaterial, uint32_t namePass)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandBindMaterialPass(ptrMaterial, namePass));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdBindInputTexture(const char *szName, uint32_t texture, uint32_t minFilter, uint32_t magFilter, uint32_t addressMode)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandBindInputTexture(szName, texture, minFilter, magFilter, addressMode));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform1i(const char *szName, int v0)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandUniform1i(szName, v0));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform2i(const char *szName, int v0, int v1)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandUniform2i(szName, v0, v1));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform3i(const char *szName, int v0, int v1, int v2)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandUniform3i(szName, v0, v1, v2));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform4i(const char *szName, int v0, int v1, int v2, int v3)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandUniform4i(szName, v0, v1, v2, v3));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform1f(const char *szName, float v0)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandUniform1f(szName, v0));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform2f(const char *szName, float v0, float v1)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandUniform2f(szName, v0, v1));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform3f(const char *szName, float v0, float v1, float v2)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandUniform3f(szName, v0, v1, v2));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform4f(const char *szName, float v0, float v1, float v2, float v3)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandUniform4f(szName, v0, v1, v2, v3));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform1iv(const char *szName, int count, const int *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandUniform1iv(szName, count, value));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform2iv(const char *szName, int count, const int *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandUniform2iv(szName, count, value));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform3iv(const char *szName, int count, const int *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandUniform3iv(szName, count, value));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform4iv(const char *szName, int count, const int *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandUniform4iv(szName, count, value));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform1fv(const char *szName, int count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandUniform1fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform2fv(const char *szName, int count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandUniform2fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform3fv(const char *szName, int count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandUniform3fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform4fv(const char *szName, int count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandUniform4fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniformMatrix2fv(const char *szName, int count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandUniformMatrix2fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniformMatrix3fv(const char *szName, int count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandUniformMatrix3fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniformMatrix4fv(const char *szName, int count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandUniformMatrix4fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdClearDepth(float depth)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandClearDepth(depth));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdClearColor(float red, float green, float blue, float alpha)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandClearColor(red, green, blue, alpha));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdDrawElements(uint32_t mode, uint32_t type, uint32_t offset, int count)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandDrawElements(mode, type, offset, count));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdDrawInstance(uint32_t mode, uint32_t type, uint32_t offset, int count, int instanceCount)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandDrawInstance(mode, type, offset, count, instanceCount));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdDrawIndirect(uint32_t mode, uint32_t type, uint32_t offset)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandDrawIndirect(mode, type, offset));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdDrawIndirect(uint32_t mode, uint32_t type, int baseVertex, int firstIndex, int count, int instanceCount)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_pCommands.emplace_back(new CGfxCommandDrawIndirect(mode, type, baseVertex, firstIndex, count, instanceCount));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdExecute(CGfxCommandBuffer *pCommandBuffer)
{
	if (m_bMainCommandBuffer == true && m_bInPassScope == true && pCommandBuffer->m_bMainCommandBuffer == false) {
		m_pCommands.emplace_back(new CGfxCommandExecute(pCommandBuffer));
		return true;
	}

	return false;
}
