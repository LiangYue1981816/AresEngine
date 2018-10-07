#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxCommandBuffer.h"
#include "GfxCommandBeginRenderPass.h"
#include "GfxCommandEndRenderPass.h"
#include "GfxCommandSetScissor.h"
#include "GfxCommandSetViewport.h"
#include "GfxCommandSetCullFace.h"
#include "GfxCommandSetDepthTest.h"
#include "GfxCommandSetDepthWrite.h"
#include "GfxCommandSetColorWrite.h"
#include "GfxCommandSetBlend.h"
#include "GfxCommandSetPolygonOffset.h"
#include "GfxCommandBindMesh.h"
#include "GfxCommandBindCamera.h"
#include "GfxCommandBindPipeline.h"
#include "GfxCommandBindMaterialPass.h"
#include "GfxCommandBindInputTexture.h"
#include "GfxCommandUniform1f.h"
#include "GfxCommandUniform2f.h"
#include "GfxCommandUniform3f.h"
#include "GfxCommandUniform4f.h"
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
{

}

CGfxCommandBuffer::~CGfxCommandBuffer(void)
{
	Clearup();
}

void CGfxCommandBuffer::Clearup(void)
{
	for (const auto &itCommand : m_commands) {
		delete itCommand;
	}

	m_commands.clear();
	m_bInPassScope = false;
	m_ptrFrameBuffer.Release();
}

bool CGfxCommandBuffer::Execute(void) const
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == false)) {
		for (const auto &itCommand : m_commands) {
			itCommand->Execute();
		}
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdBeginRenderPass(const CGfxFrameBufferPtr &ptrFrameBuffer)
{
	if (m_bMainCommandBuffer == true && m_bInPassScope == false) {
		m_bInPassScope = true;
		m_ptrFrameBuffer = ptrFrameBuffer;
		m_commands.push_back(new CGfxCommandBeginRenderPass(m_ptrFrameBuffer));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdEndRenderPass(void)
{
	if (m_bMainCommandBuffer == true && m_bInPassScope == true) {
		m_commands.push_back(new CGfxCommandEndRenderPass(m_ptrFrameBuffer));
		m_bInPassScope = false;
		m_ptrFrameBuffer.Release();
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdSetScissor(int x, int y, int width, int height)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandSetScissor(x, y, width, height));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdSetViewport(int x, int y, int width, int height)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandSetViewport(x, y, width, height));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdSetCullFace(bool bEnable, uint32_t cullFace, uint32_t frontFace)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandSetCullFace(bEnable, cullFace, frontFace));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdSetDepthTest(bool bEnable, uint32_t depthFunc)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandSetDepthTest(bEnable, depthFunc));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdSetDepthWrite(bool bEnable)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandSetDepthWrite(bEnable));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdSetColorWrite(bool bEnableRed, bool bEnableGreen, bool bEnableBlue, bool bEnableAlpha)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandSetColorWrite(bEnableRed, bEnableGreen, bEnableBlue, bEnableAlpha));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdSetBlend(bool bEnable, uint32_t srcFactor, uint32_t dstFactor)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandSetBlend(bEnable, srcFactor, dstFactor));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdSetPolygonOffset(bool bEnable, float factor, float units)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandSetPolygonOffset(bEnable, factor, units));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdBindCamera(CGfxCamera *pCamera)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandBindCamera(pCamera));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdBindPipeline(CGfxPipelineBase *pPipeline)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandBindPipeline(pPipeline));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdBindMesh(const CGfxMeshPtr &ptrMesh, const eastl::vector<glm::mat4> &mtxTransforms)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandBindMesh(ptrMesh, mtxTransforms));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdBindMaterialPass(const CGfxMaterialPtr &ptrMaterial, uint32_t namePass)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandBindMaterialPass(ptrMaterial, namePass));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdBindInputTexture(const char *szName, uint32_t texture, uint32_t minFilter, uint32_t magFilter, uint32_t addressMode)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandBindInputTexture(szName, texture, minFilter, magFilter, addressMode));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform1f(const char *szName, float v0)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandUniform1f(szName, v0));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform2f(const char *szName, float v0, float v1)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandUniform2f(szName, v0, v1));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform3f(const char *szName, float v0, float v1, float v2)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandUniform3f(szName, v0, v1, v2));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform4f(const char *szName, float v0, float v1, float v2, float v3)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandUniform4f(szName, v0, v1, v2, v3));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform1fv(const char *szName, uint32_t count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandUniform1fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform2fv(const char *szName, uint32_t count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandUniform2fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform3fv(const char *szName, uint32_t count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandUniform3fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniform4fv(const char *szName, uint32_t count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandUniform4fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniformMatrix2fv(const char *szName, uint32_t count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandUniformMatrix2fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniformMatrix3fv(const char *szName, uint32_t count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandUniformMatrix3fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdUniformMatrix4fv(const char *szName, uint32_t count, const float *value)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandUniformMatrix4fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdClearDepth(float depth)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandClearDepth(depth));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdClearColor(float red, float green, float blue, float alpha)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandClearColor(red, green, blue, alpha));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdDrawInstance(uint32_t mode, uint32_t type, int count, int baseIndex, int instanceCount)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandDrawInstance(mode, type, count, baseIndex, instanceCount));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdDrawIndirect(uint32_t mode, uint32_t type, int count, int baseIndex, int baseVertex, int instanceCount)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandDrawIndirect(mode, type, count, baseIndex, baseVertex, instanceCount));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdDrawElements(uint32_t mode, uint32_t type, int count, int baseIndex)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandDrawElements(mode, type, count, baseIndex));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdExecute(CGfxCommandBuffer *pCommandBuffer)
{
	if (m_bMainCommandBuffer == true && m_bInPassScope == true && pCommandBuffer->m_bMainCommandBuffer == false) {
		m_commands.push_back(new CGfxCommandExecute(pCommandBuffer));
		return true;
	}

	return false;
}
