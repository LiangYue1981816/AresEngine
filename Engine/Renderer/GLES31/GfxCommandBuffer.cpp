#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxCommandBuffer.h"
#include "GfxCommandBeginRenderPass.h"
#include "GfxCommandEndRenderPass.h"
#include "GfxCommandBindMesh.h"
#include "GfxCommandBindCamera.h"
#include "GfxCommandBindPipeline.h"
#include "GfxCommandBindMaterialPass.h"
#include "GfxCommandBindInputTexture.h"
#include "GfxCommandSetScissor.h"
#include "GfxCommandSetViewport.h"
#include "GfxCommandSetCullFace.h"
#include "GfxCommandSetDepthTest.h"
#include "GfxCommandSetDepthWrite.h"
#include "GfxCommandSetColorWrite.h"
#include "GfxCommandSetBlend.h"
#include "GfxCommandSetPolygonOffset.h"
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

bool CGfxCommandBuffer::CmdSetCullFace(bool bEnable, GLenum cullFace, GLenum frontFace)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandSetCullFace(bEnable, cullFace, frontFace));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdSetDepthTest(bool bEnable, GLenum depthFunc)
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

bool CGfxCommandBuffer::CmdSetBlend(bool bEnable, GLenum srcFactor, GLenum dstFactor)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandSetBlend(bEnable, srcFactor, dstFactor));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdSetPolygonOffset(bool bEnable, GLfloat factor, GLfloat units)
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

bool CGfxCommandBuffer::CmdBindInputTexture(const char *szName, GLuint texture, GLenum minFilter, GLenum magFilter, GLenum addressMode)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandBindInputTexture(szName, texture, minFilter, magFilter, addressMode));
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

bool CGfxCommandBuffer::CmdDrawInstance(GLenum mode, GLenum type, GLsizei count, GLsizei baseIndex, GLsizei instanceCount)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandDrawInstance(mode, type, count, baseIndex, instanceCount));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdDrawIndirect(GLenum mode, GLenum type, GLsizei count, GLsizei baseIndex, GLsizei baseVertex, GLsizei instanceCount)
{
	if ((m_bMainCommandBuffer == false) || (m_bMainCommandBuffer == true && m_bInPassScope == true)) {
		m_commands.push_back(new CGfxCommandDrawIndirect(mode, type, count, baseIndex, baseVertex, instanceCount));
		return true;
	}

	return false;
}

bool CGfxCommandBuffer::CmdDrawElements(GLenum mode, GLenum type, GLsizei count, GLsizei baseIndex)
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
