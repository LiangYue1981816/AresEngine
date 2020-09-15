#include "GLES3Renderer.h"
#include "./Command/GLES3CommandSetBufferBarrier.h"
#include "./Command/GLES3CommandBeginRenderPass.h"
#include "./Command/GLES3CommandNextSubPass.h"
#include "./Command/GLES3CommandEndRenderPass.h"
#include "./Command/GLES3CommandResolve.h"
#include "./Command/GLES3CommandInvalidateFramebuffer.h"
#include "./Command/GLES3CommandBindFrameBuffer.h"
#include "./Command/GLES3CommandBindPipelineCompute.h"
#include "./Command/GLES3CommandBindPipelineGraphics.h"
#include "./Command/GLES3CommandBindDescriptorSet.h"
#include "./Command/GLES3CommandBindIndexBuffer.h"
#include "./Command/GLES3CommandBindVertexBuffer.h"
#include "./Command/GLES3CommandBindInstanceBuffer.h"
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
#include "./Command/GLES3CommandDispatch.h"
#include "./Command/GLES3CommandDrawInstance.h"
#include "./Command/GLES3CommandDrawIndirect.h"
#include "./Command/GLES3CommandPushDebugGroup.h"
#include "./Command/GLES3CommandPopDebugGroup.h"
#include "./Command/GLES3CommandExecute.h"


CGLES3CommandBuffer::CGLES3CommandBuffer(CGLES3CommandBufferManager* pManager, bool bMainCommandBuffer)
	: CGfxCommandBuffer(bMainCommandBuffer)
	, m_pManager(pManager)

	, m_indexSubpass(-1)
	, m_pCurrentPipelineCompute(nullptr)
	, m_pCurrentPipelineGraphics(nullptr)
{

}

CGLES3CommandBuffer::~CGLES3CommandBuffer(void)
{

}

void CGLES3CommandBuffer::Release(void)
{
	Clearup();
	m_pManager->Destroy(this);
}

const CGfxSemaphore* CGLES3CommandBuffer::GetSemaphore(void) const
{
	return nullptr;
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
	m_ptrRenderPass.Release();
	m_ptrFrameBuffer.Release();

	m_indexSubpass = -1;
	m_pCurrentPipelineCompute = nullptr;
	m_pCurrentPipelineGraphics = nullptr;
}

void CGLES3CommandBuffer::Execute(void) const
{
	for (const auto& itCommand : m_pCommands) {
		itCommand->Execute();
	}
}

void CGLES3CommandBuffer::WaitForFinish(void) const
{

}

bool CGLES3CommandBuffer::IsEmpty(void) const
{
	return m_pCommands.empty();
}

void CGLES3CommandBuffer::BeginRecord(void)
{

}

void CGLES3CommandBuffer::BeginRecord(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass)
{
	m_indexSubpass = indexSubpass;
	m_ptrRenderPass = ptrRenderPass;
	m_ptrFrameBuffer = ptrFrameBuffer;
}

void CGLES3CommandBuffer::EndRecord(void)
{

}

void CGLES3CommandBuffer::CmdBufferData(const CGfxIndexBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
{

}

void CGLES3CommandBuffer::CmdBufferData(const CGfxVertexBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
{

}

void CGLES3CommandBuffer::CmdBufferData(const CGfxInstanceBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
{

}

void CGLES3CommandBuffer::CmdBufferData(const CGfxIndirectBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
{

}

void CGLES3CommandBuffer::CmdBufferData(const CGfxUniformBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
{

}

void CGLES3CommandBuffer::CmdBufferData(const CGfxStorageBufferPtr ptrBuffer, size_t offset, size_t size, const void* data)
{

}

void CGLES3CommandBuffer::CmdTextureData(const CGfxTexture2DPtr ptrTexture, GfxPixelFormat format, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{

}

void CGLES3CommandBuffer::CmdTextureData(const CGfxTexture2DArrayPtr ptrTexture, GfxPixelFormat format, int layer, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{

}

void CGLES3CommandBuffer::CmdTextureData(const CGfxTextureCubemapPtr ptrTexture, GfxPixelFormat format, GfxCubemapFace face, int level, int xoffset, int yoffset, int width, int height, uint32_t size, const void* data)
{

}

void CGLES3CommandBuffer::CmdSetImageLayout(const CGfxTexture2DPtr ptrTexture, GfxImageLayout imageLayout)
{

}

void CGLES3CommandBuffer::CmdSetImageLayout(const CGfxTexture2DArrayPtr ptrTexture, GfxImageLayout imageLayout)
{

}

void CGLES3CommandBuffer::CmdSetImageLayout(const CGfxTextureCubemapPtr ptrTexture, GfxImageLayout imageLayout)
{

}

void CGLES3CommandBuffer::CmdSetImageLayout(const CGfxRenderTexturePtr ptrTexture, GfxImageLayout imageLayout)
{

}

void CGLES3CommandBuffer::CmdSetBufferBarrier(const CGfxStorageBufferPtr ptrBuffer, GfxAccessFlags srcAccessFlags, GfxAccessFlags dstAccessFlags)
{
	m_pCommands.emplace_back(new CGLES3CommandSetBufferBarrier(ptrBuffer, srcAccessFlags, dstAccessFlags));
}

void CGLES3CommandBuffer::CmdBeginRenderPass(const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass)
{
	m_indexSubpass = 0;
	m_ptrRenderPass = ptrRenderPass;
	m_ptrFrameBuffer = ptrFrameBuffer;

	m_pCommands.emplace_back(new CGLES3CommandBeginRenderPass(m_ptrFrameBuffer, m_ptrRenderPass));
	m_pCommands.emplace_back(new CGLES3CommandBindFrameBuffer(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass, true));
}

void CGLES3CommandBuffer::CmdNextSubpass(void)
{
	if (m_indexSubpass < (int)m_ptrRenderPass->GetSubpassCount() - 1) {
		m_pCommands.emplace_back(new CGLES3CommandInvalidateFramebuffer(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));
		m_pCommands.emplace_back(new CGLES3CommandResolve(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));
		m_indexSubpass += 1;
		m_pCommands.emplace_back(new CGLES3CommandNextSubPass(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));
		m_pCommands.emplace_back(new CGLES3CommandBindFrameBuffer(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass, false));
	}
}

void CGLES3CommandBuffer::CmdEndRenderPass(void)
{
	if (m_indexSubpass >= 0) {
		m_pCommands.emplace_back(new CGLES3CommandInvalidateFramebuffer(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));
		m_pCommands.emplace_back(new CGLES3CommandResolve(m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass));
		m_pCommands.emplace_back(new CGLES3CommandEndRenderPass(m_ptrFrameBuffer, m_ptrRenderPass));
		m_indexSubpass = -1;
	}
}

void CGLES3CommandBuffer::CmdBindPipelineCompute(const CGfxPipelineCompute* pPipelineCompute)
{
	m_pCurrentPipelineCompute = (CGLES3PipelineCompute*)pPipelineCompute;
	m_pCommands.emplace_back(new CGLES3CommandBindPipelineCompute(pPipelineCompute));
}

void CGLES3CommandBuffer::CmdBindPipelineGraphics(const CGfxPipelineGraphics* pPipelineGraphics)
{
	m_pCurrentPipelineGraphics = (CGLES3PipelineGraphics*)pPipelineGraphics;
	m_pCommands.emplace_back(new CGLES3CommandBindPipelineGraphics(pPipelineGraphics));
}

void CGLES3CommandBuffer::CmdBindDescriptorSet(const CGfxDescriptorSetPtr ptrDescriptorSet)
{
	m_pCommands.emplace_back(new CGLES3CommandBindDescriptorSet(m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, ptrDescriptorSet));
}

void CGLES3CommandBuffer::CmdBindIndexBuffer(const CGfxIndexBufferPtr ptrIndexBuffer)
{
	m_pCommands.emplace_back(new CGLES3CommandBindIndexBuffer(ptrIndexBuffer));
}

void CGLES3CommandBuffer::CmdBindVertexBuffer(const CGfxVertexBufferPtr ptrVertexBuffer)
{
	m_pCommands.emplace_back(new CGLES3CommandBindVertexBuffer(m_pCurrentPipelineGraphics, ptrVertexBuffer));
}

void CGLES3CommandBuffer::CmdBindInstanceBuffer(const CGfxInstanceBufferPtr ptrInstanceBuffer, int offset)
{
	m_pCommands.emplace_back(new CGLES3CommandBindInstanceBuffer(m_pCurrentPipelineGraphics, ptrInstanceBuffer, offset));
}

void CGLES3CommandBuffer::CmdUniform1i(uint32_t name, int v0)
{
	m_pCommands.emplace_back(new CGLES3CommandUniform1i(m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0));
}

void CGLES3CommandBuffer::CmdUniform2i(uint32_t name, int v0, int v1)
{
	m_pCommands.emplace_back(new CGLES3CommandUniform2i(m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0, v1));
}

void CGLES3CommandBuffer::CmdUniform3i(uint32_t name, int v0, int v1, int v2)
{
	m_pCommands.emplace_back(new CGLES3CommandUniform3i(m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0, v1, v2));
}

void CGLES3CommandBuffer::CmdUniform4i(uint32_t name, int v0, int v1, int v2, int v3)
{
	m_pCommands.emplace_back(new CGLES3CommandUniform4i(m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0, v1, v2, v3));
}

void CGLES3CommandBuffer::CmdUniform1f(uint32_t name, float v0)
{
	m_pCommands.emplace_back(new CGLES3CommandUniform1f(m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0));
}

void CGLES3CommandBuffer::CmdUniform2f(uint32_t name, float v0, float v1)
{
	m_pCommands.emplace_back(new CGLES3CommandUniform2f(m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0, v1));
}

void CGLES3CommandBuffer::CmdUniform3f(uint32_t name, float v0, float v1, float v2)
{
	m_pCommands.emplace_back(new CGLES3CommandUniform3f(m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0, v1, v2));
}

void CGLES3CommandBuffer::CmdUniform4f(uint32_t name, float v0, float v1, float v2, float v3)
{
	m_pCommands.emplace_back(new CGLES3CommandUniform4f(m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0, v1, v2, v3));
}

void CGLES3CommandBuffer::CmdUniform1iv(uint32_t name, int count, const int* value)
{
	m_pCommands.emplace_back(new CGLES3CommandUniform1iv(m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
}

void CGLES3CommandBuffer::CmdUniform2iv(uint32_t name, int count, const int* value)
{
	m_pCommands.emplace_back(new CGLES3CommandUniform2iv(m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
}

void CGLES3CommandBuffer::CmdUniform3iv(uint32_t name, int count, const int* value)
{
	m_pCommands.emplace_back(new CGLES3CommandUniform3iv(m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
}

void CGLES3CommandBuffer::CmdUniform4iv(uint32_t name, int count, const int* value)
{
	m_pCommands.emplace_back(new CGLES3CommandUniform4iv(m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
}

void CGLES3CommandBuffer::CmdUniform1fv(uint32_t name, int count, const float* value)
{
	m_pCommands.emplace_back(new CGLES3CommandUniform1fv(m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
}

void CGLES3CommandBuffer::CmdUniform2fv(uint32_t name, int count, const float* value)
{
	m_pCommands.emplace_back(new CGLES3CommandUniform2fv(m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
}

void CGLES3CommandBuffer::CmdUniform3fv(uint32_t name, int count, const float* value)
{
	m_pCommands.emplace_back(new CGLES3CommandUniform3fv(m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
}

void CGLES3CommandBuffer::CmdUniform4fv(uint32_t name, int count, const float* value)
{
	m_pCommands.emplace_back(new CGLES3CommandUniform4fv(m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
}

void CGLES3CommandBuffer::CmdUniformMatrix2fv(uint32_t name, int count, const float* value)
{
	m_pCommands.emplace_back(new CGLES3CommandUniformMatrix2fv(m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
}

void CGLES3CommandBuffer::CmdUniformMatrix3fv(uint32_t name, int count, const float* value)
{
	m_pCommands.emplace_back(new CGLES3CommandUniformMatrix3fv(m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
}

void CGLES3CommandBuffer::CmdUniformMatrix4fv(uint32_t name, int count, const float* value)
{
	m_pCommands.emplace_back(new CGLES3CommandUniformMatrix4fv(m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
}

void CGLES3CommandBuffer::CmdSetScissor(int x, int y, int width, int height)
{
	m_pCommands.emplace_back(new CGLES3CommandSetScissor(x, y, width, height));
}

void CGLES3CommandBuffer::CmdSetViewport(int x, int y, int width, int height, float znear, float zfar)
{
	m_pCommands.emplace_back(new CGLES3CommandSetViewport(x, y, width, height, znear, zfar));
}

void CGLES3CommandBuffer::CmdClearDepth(float depth)
{
	m_pCommands.emplace_back(new CGLES3CommandClearDepth(depth));
}

void CGLES3CommandBuffer::CmdClearColor(float red, float green, float blue, float alpha)
{
	m_pCommands.emplace_back(new CGLES3CommandClearColor(red, green, blue, alpha));
}

void CGLES3CommandBuffer::CmdDrawInstance(int indexType, int indexOffset, int indexCount, int instanceCount)
{
	m_pCommands.emplace_back(new CGLES3CommandDrawInstance(indexType, indexOffset, indexCount, instanceCount));
}

void CGLES3CommandBuffer::CmdDispatch(int numLocalWorkGroupX, int numLocalWorkGroupY, int numLocalWorkGroupZ)
{
	m_pCommands.emplace_back(new CGLES3CommandDispatch(numLocalWorkGroupX, numLocalWorkGroupY, numLocalWorkGroupZ));
}

void CGLES3CommandBuffer::CmdPushDebugGroup(const char* szMessage)
{
	m_pCommands.emplace_back(new CGLES3CommandPushDebugGroup(szMessage));
}

void CGLES3CommandBuffer::CmdPopDebugGroup(void)
{
	m_pCommands.emplace_back(new CGLES3CommandPopDebugGroup());
}

void CGLES3CommandBuffer::CmdExecute(const CGfxCommandBufferPtr ptrCommandBuffer)
{
	m_pCommands.emplace_back(new CGLES3CommandExecute(ptrCommandBuffer));
}
