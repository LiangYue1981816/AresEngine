#include "GLES3Renderer.h"
#include "./Command/GLES3CommandBeginRenderPass.h"
#include "./Command/GLES3CommandNextSubPass.h"
#include "./Command/GLES3CommandEndRenderPass.h"
#include "./Command/GLES3CommandResolve.h"
#include "./Command/GLES3CommandInvalidateFramebuffer.h"
#include "./Command/GLES3CommandBindFrameBuffer.h"
#include "./Command/GLES3CommandBindPipelineCompute.h"
#include "./Command/GLES3CommandBindPipelineGraphics.h"
#include "./Command/GLES3CommandPushDebugGroup.h"
#include "./Command/GLES3CommandPopDebugGroup.h"


CGLES3CommandBuffer::CGLES3CommandBuffer(CGLES3CommandBufferManager *pManager, bool bMainCommandBuffer)
	: CGfxCommandBuffer(bMainCommandBuffer)
	, m_pManager(pManager)

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

HANDLE CGLES3CommandBuffer::GetCommandBuffer(void) const
{
	return nullptr;
}

CGfxFrameBufferPtr CGLES3CommandBuffer::GetFrameBuffer(void) const
{
	return m_ptrFrameBuffer;
}

CGfxRenderPassPtr CGLES3CommandBuffer::GetRenderPass(void) const
{
	return m_ptrRenderPass;
}

bool CGLES3CommandBuffer::IsInRenderPass(void) const
{
	return m_bInRenderPass;
}

uint32_t CGLES3CommandBuffer::GetSubpassIndex(void) const
{
	return m_indexSubpass;
}

void CGLES3CommandBuffer::Clearup(void)
{
	for (const auto &itCommand : m_pCommands) {
		delete itCommand;
	}

	m_pCommands.clear();

	m_bInRenderPass = false;
	m_indexSubpass = 0;

	m_ptrRenderPass.Release();
	m_ptrFrameBuffer.Release();
}

bool CGLES3CommandBuffer::Execute(void) const
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == false)) {
		for (const auto &itCommand : m_pCommands) {
			itCommand->Execute();
		}

		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdBeginRenderPass(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass)
{
	if (IsMainCommandBuffer() == true && IsInRenderPass() == false && m_pCommands.empty()) {
		m_bInRenderPass = true;
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
	if (IsMainCommandBuffer() == true && IsInRenderPass() == true) {
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

		m_bInRenderPass = false;
		m_indexSubpass = 0;

		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdBindPipelineCompute(const CGfxPipelineCompute *pPipelineCompute)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CGLES3CommandBindPipelineCompute(pPipelineCompute));
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdBindPipelineGraphics(const CGfxPipelineGraphics *pPipelineGraphics)
{
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
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform1i(uint32_t name, int v0)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform2i(uint32_t name, int v0, int v1)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform3i(uint32_t name, int v0, int v1, int v2)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform4i(uint32_t name, int v0, int v1, int v2, int v3)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform1f(uint32_t name, float v0)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform2f(uint32_t name, float v0, float v1)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform3f(uint32_t name, float v0, float v1, float v2)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform4f(uint32_t name, float v0, float v1, float v2, float v3)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform1iv(uint32_t name, int count, const int *value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform2iv(uint32_t name, int count, const int *value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform3iv(uint32_t name, int count, const int *value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform4iv(uint32_t name, int count, const int *value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform1fv(uint32_t name, int count, const float *value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform2fv(uint32_t name, int count, const float *value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform3fv(uint32_t name, int count, const float *value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniform4fv(uint32_t name, int count, const float *value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniformMatrix2fv(uint32_t name, int count, const float *value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniformMatrix3fv(uint32_t name, int count, const float *value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdUniformMatrix4fv(uint32_t name, int count, const float *value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdSetScissor(int x, int y, int width, int height)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdSetViewport(int x, int y, int width, int height)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdClearDepth(float depth)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdClearColor(float red, float green, float blue, float alpha)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdDrawInstance(const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t *pInstanceBuffer, uint32_t size)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdDrawIndirect(const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t *pInstanceBuffer, uint32_t size)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdExecute(const CGfxCommandBufferPtr ptrCommandBuffer)
{
	if (IsMainCommandBuffer() == true && IsInRenderPass() == true && ptrCommandBuffer->IsMainCommandBuffer() == false) {
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3CommandBuffer::CmdPresent(void)
{
	if (IsMainCommandBuffer() == true && IsInRenderPass() == false) {
		return true;
	}
	else {
		return false;
	}
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

/*
CGLES3CommandBuffer::CGLES3CommandBuffer(CGLES3CommandBufferManager *pManager, bool bMainCommandBuffer)
	: CGfxCommandBuffer(bMainCommandBuffer)
	, m_pManager(pManager)

	, IsMainCommandBuffer()(bMainCommandBuffer)
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

HANDLE CGLES3CommandBuffer::GetCommandBuffer(void) const
{
	return nullptr;
}

bool CGLES3CommandBuffer::IsMainCommandBuffer(void) const
{
	return IsMainCommandBuffer();
}

uint32_t CGLES3CommandBuffer::GetCommandCount(void) const
{
	return m_pCommands.size();
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
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == false)) {
		for (const auto &itCommand : m_pCommands) {
			itCommand->Execute();
		}

		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdBeginRenderPass(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass)
{
	if (IsMainCommandBuffer() == true && m_bInRenderPass == false && m_pCommands.empty()) {
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
	if (IsMainCommandBuffer() == true && m_bInRenderPass == true) {
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
	if (IsMainCommandBuffer() == true && m_bInRenderPass == true) {
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
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandBindPipelineCompute(pPipelineCompute));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdBindPipelineGraphics(const CGfxPipelineGraphics *pPipelineGraphics)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandBindPipelineGraphics(pPipelineGraphics));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdBindMaterialPass(const CGfxMaterialPtr ptrMaterial, uint32_t namePass)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandBindMaterialPass(ptrMaterial, namePass));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdBindUniformBuffer(const CGfxUniformBufferPtr ptrUniformBuffer, uint32_t nameUniform)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandBindUniformBuffer(ptrUniformBuffer, nameUniform));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdBindMeshDraw(const CGfxMeshDrawPtr ptrMeshDraw)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandBindMeshDraw(ptrMeshDraw));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform1i(const char *szName, int v0)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform1i(szName, v0));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform2i(const char *szName, int v0, int v1)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform2i(szName, v0, v1));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform3i(const char *szName, int v0, int v1, int v2)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform3i(szName, v0, v1, v2));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform4i(const char *szName, int v0, int v1, int v2, int v3)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform4i(szName, v0, v1, v2, v3));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform1f(const char *szName, float v0)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform1f(szName, v0));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform2f(const char *szName, float v0, float v1)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform2f(szName, v0, v1));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform3f(const char *szName, float v0, float v1, float v2)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform3f(szName, v0, v1, v2));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform4f(const char *szName, float v0, float v1, float v2, float v3)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform4f(szName, v0, v1, v2, v3));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform1iv(const char *szName, int count, const int *value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform1iv(szName, count, value));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform2iv(const char *szName, int count, const int *value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform2iv(szName, count, value));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform3iv(const char *szName, int count, const int *value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform3iv(szName, count, value));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform4iv(const char *szName, int count, const int *value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform4iv(szName, count, value));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform1fv(const char *szName, int count, const float *value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform1fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform2fv(const char *szName, int count, const float *value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform2fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform3fv(const char *szName, int count, const float *value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform3fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniform4fv(const char *szName, int count, const float *value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniform4fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniformMatrix2fv(const char *szName, int count, const float *value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniformMatrix2fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniformMatrix3fv(const char *szName, int count, const float *value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniformMatrix3fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdUniformMatrix4fv(const char *szName, int count, const float *value)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandUniformMatrix4fv(szName, count, value));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdClearDepth(float depth)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandClearDepth(depth));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdClearColor(float red, float green, float blue, float alpha)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandClearColor(red, green, blue, alpha));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdSetScissor(int x, int y, int width, int height)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandSetScissor(x, y, width, height));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdSetViewport(int x, int y, int width, int height)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandSetViewport(x, y, width, height));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdSetInstanceBufferData(const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t *pInstanceBuffer, uint32_t size)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandSetInstanceBufferData(ptrMeshDraw, pInstanceBuffer, size));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdDrawElements(const CGfxMeshDrawPtr ptrMeshDraw)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandDrawElements(ptrMeshDraw));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdDrawInstance(const CGfxMeshDrawPtr ptrMeshDraw)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandDrawInstance(ptrMeshDraw));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdDrawIndirect(const CGfxMeshDrawPtr ptrMeshDraw)
{
	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && m_bInRenderPass == true)) {
		m_pCommands.emplace_back(new CGLES3CommandDrawIndirect(ptrMeshDraw));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdExecute(const CGfxCommandBufferPtr ptrCommandBuffer)
{
	if (IsMainCommandBuffer() == true && m_bInRenderPass == true && ptrCommandBuffer->IsMainCommandBuffer() == false) {
		m_pCommands.emplace_back(new CGLES3CommandExecute(ptrCommandBuffer));
		return true;
	}

	return false;
}

bool CGLES3CommandBuffer::CmdPresent(void)
{
	if (IsMainCommandBuffer() == true && m_bInRenderPass == false) {
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
*/