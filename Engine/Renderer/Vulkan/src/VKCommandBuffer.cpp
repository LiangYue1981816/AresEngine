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
#include "./Command/VKCommandBindMesh.h"
#include "./Command/VKCommandBindMeshDraw.h"
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
	, m_pCurrentPipelineCompute(nullptr)
	, m_pCurrentPipelineGraphics(nullptr)

	, m_pSemaphore(nullptr)
{
	Create(vkCommandPool, bMainCommandBuffer);
	m_pSemaphore = new CVKSemaphore(m_pDevice);
}

CVKCommandBuffer::~CVKCommandBuffer(void)
{
	Destroy();
	delete m_pSemaphore;
}

void CVKCommandBuffer::Release(void)
{
	Clearup();
	m_pManager->Destroy(this);
}

bool CVKCommandBuffer::Create(VkCommandPool vkCommandPool, bool bMainCommandBuffer)
{
	ASSERT(vkCommandPool);

	m_vkCommandPool = vkCommandPool;

	if (bMainCommandBuffer) {
		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.pNext = nullptr;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		CALL_VK_FUNCTION_RETURN_BOOL(vkCreateFence(m_pDevice->GetDevice(), &fenceCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkFence));
	}

	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.pNext = nullptr;
	commandBufferAllocateInfo.commandPool = vkCommandPool;
	commandBufferAllocateInfo.level = bMainCommandBuffer ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
	commandBufferAllocateInfo.commandBufferCount = 1;
	CALL_VK_FUNCTION_RETURN_BOOL(vkAllocateCommandBuffers(m_pDevice->GetDevice(), &commandBufferAllocateInfo, &m_vkCommandBuffer));

	return true;
}

void CVKCommandBuffer::Destroy(void)
{
	ASSERT(m_vkCommandBuffer);

	Clearup();

	if (m_vkFence) {
		vkDestroyFence(m_pDevice->GetDevice(), m_vkFence, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	vkFreeCommandBuffers(m_pDevice->GetDevice(), m_vkCommandPool, 1, &m_vkCommandBuffer);

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
	ASSERT(m_vkCommandPool);
	return m_vkCommandPool;
}

VkCommandBuffer CVKCommandBuffer::GetCommandBuffer(void) const
{
	ASSERT(m_vkCommandBuffer);
	return m_vkCommandBuffer;
}

const CGfxSemaphore* CVKCommandBuffer::GetSemaphore(void) const
{
	ASSERT(m_pSemaphore);
	return m_pSemaphore;
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
	ResetCommandBuffer();

	for (const auto& itCommand : m_pCommands) {
		delete itCommand;
	}

	m_pCommands.clear();

	m_indexSubpass = -1;
	m_ptrRenderPass.Release();
	m_ptrFrameBuffer.Release();
	m_pCurrentPipelineCompute = nullptr;
	m_pCurrentPipelineGraphics = nullptr;
}

void CVKCommandBuffer::Execute(void) const
{

}

void CVKCommandBuffer::WaitForFinish(void) const
{
	if (m_vkFence) {
		vkWaitForFences(m_pDevice->GetDevice(), 1, &m_vkFence, VK_TRUE, UINT64_MAX);
	}
}

void CVKCommandBuffer::ResetCommandBuffer(void) const
{
	ASSERT(m_vkCommandBuffer);
	vkResetCommandBuffer(m_vkCommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
}

bool CVKCommandBuffer::IsEmpty(void) const
{
	return m_pCommands.empty();
}

bool CVKCommandBuffer::BeginRecord(void)
{
	ASSERT(m_vkCommandBuffer);

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
	ASSERT(m_vkCommandBuffer);

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
	ASSERT(m_vkCommandBuffer);

	m_pCommands.emplace_back(new CVKCommandEndRecord(m_vkCommandBuffer));
	return true;
}

bool CVKCommandBuffer::CmdSetImageLayout(const CGfxTexture2DPtr ptrTexture, GfxImageLayout imageLayout)
{
	ASSERT(ptrTexture);
	ASSERT(m_vkCommandBuffer);

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
	ASSERT(m_vkCommandBuffer);

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
	ASSERT(m_vkCommandBuffer);

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
	ASSERT(m_vkCommandBuffer);

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
	ASSERT(m_vkCommandBuffer);

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
	ASSERT(m_vkCommandBuffer);

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
	ASSERT(m_vkCommandBuffer);

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
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCurrentPipelineCompute = (CVKPipelineCompute*)pPipelineCompute;
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
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCurrentPipelineGraphics = (CVKPipelineGraphics*)pPipelineGraphics;
		m_pCommands.emplace_back(new CVKCommandBindPipelineGraphics(m_vkCommandBuffer, pPipelineGraphics));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdBindDescriptorSet(const CGfxDescriptorSetPtr ptrDescriptorSet)
{
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandBindDescriptorSet(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, ptrDescriptorSet));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdBindMesh(const CGfxMeshPtr ptrMesh)
{
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandBindMesh(m_vkCommandBuffer, m_pCurrentPipelineGraphics, ptrMesh));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdBindMeshDraw(const CGfxMeshDrawPtr ptrMeshDraw)
{
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandBindMeshDraw(m_vkCommandBuffer, m_pCurrentPipelineGraphics, ptrMeshDraw));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform1i(uint32_t name, int v0)
{
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform1i(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform2i(uint32_t name, int v0, int v1)
{
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform2i(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0, v1));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform3i(uint32_t name, int v0, int v1, int v2)
{
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform3i(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0, v1, v2));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform4i(uint32_t name, int v0, int v1, int v2, int v3)
{
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform4i(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0, v1, v2, v3));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform1f(uint32_t name, float v0)
{
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform1f(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform2f(uint32_t name, float v0, float v1)
{
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform2f(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0, v1));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform3f(uint32_t name, float v0, float v1, float v2)
{
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform3f(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0, v1, v2));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform4f(uint32_t name, float v0, float v1, float v2, float v3)
{
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform4f(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, v0, v1, v2, v3));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform1iv(uint32_t name, int count, const int* value)
{
	ASSERT(value);
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform1iv(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform2iv(uint32_t name, int count, const int* value)
{
	ASSERT(value);
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform2iv(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform3iv(uint32_t name, int count, const int* value)
{
	ASSERT(value);
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform3iv(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform4iv(uint32_t name, int count, const int* value)
{
	ASSERT(value);
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform4iv(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform1fv(uint32_t name, int count, const float* value)
{
	ASSERT(value);
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform1fv(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform2fv(uint32_t name, int count, const float* value)
{
	ASSERT(value);
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform2fv(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform3fv(uint32_t name, int count, const float* value)
{
	ASSERT(value);
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform3fv(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniform4fv(uint32_t name, int count, const float* value)
{
	ASSERT(value);
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniform4fv(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniformMatrix2fv(uint32_t name, int count, const float* value)
{
	ASSERT(value);
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniformMatrix2fv(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniformMatrix3fv(uint32_t name, int count, const float* value)
{
	ASSERT(value);
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniformMatrix3fv(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUniformMatrix4fv(uint32_t name, int count, const float* value)
{
	ASSERT(value);
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUniformMatrix4fv(m_vkCommandBuffer, m_pCurrentPipelineCompute, m_pCurrentPipelineGraphics, name, count, value));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdSetScissor(int x, int y, int width, int height)
{
	ASSERT(m_vkCommandBuffer);

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
	ASSERT(m_vkCommandBuffer);

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
	ASSERT(m_vkCommandBuffer);

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
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandClearColor(m_vkCommandBuffer, red, green, blue, alpha));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdDrawInstance(const CGfxMeshDrawPtr ptrMeshDraw)
{
	ASSERT(ptrMeshDraw);
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandDrawInstance(m_vkCommandBuffer, m_pCurrentPipelineGraphics, ptrMeshDraw));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdDrawIndirect(const CGfxMeshDrawPtr ptrMeshDraw)
{
	ASSERT(ptrMeshDraw);
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandDrawIndirect(m_vkCommandBuffer, m_pCurrentPipelineGraphics, ptrMeshDraw));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdUpdateInstanceBuffer(const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceBuffer, uint32_t size)
{
	ASSERT(ptrMeshDraw);
	ASSERT(m_vkCommandBuffer);

	if ((IsMainCommandBuffer() == false) || (IsMainCommandBuffer() == true && IsInRenderPass() == true)) {
		m_pCommands.emplace_back(new CVKCommandUpdateInstanceBuffer(m_vkCommandBuffer, ptrMeshDraw, pInstanceBuffer, size));
		return true;
	}
	else {
		return false;
	}
}

bool CVKCommandBuffer::CmdExecute(const CGfxCommandBufferPtr ptrCommandBuffer)
{
	ASSERT(ptrCommandBuffer);
	ASSERT(m_vkCommandBuffer);

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
	ASSERT(szMessage);
	ASSERT(m_vkCommandBuffer);

	m_pCommands.emplace_back(new CVKCommandPushDebugGroup(m_vkCommandBuffer, szMessage));
	return true;
}

bool CVKCommandBuffer::CmdPopDebugGroup(void)
{
	ASSERT(m_vkCommandBuffer);

	m_pCommands.emplace_back(new CVKCommandPopDebugGroup(m_vkCommandBuffer));
	return true;
}
