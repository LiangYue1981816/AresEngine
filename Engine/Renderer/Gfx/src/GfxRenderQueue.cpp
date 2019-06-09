#include "GfxHeader.h"


class CALL_API CTaskCommandBuffer : public CTask
{
public:
	CTaskCommandBuffer(int indexQueue, const CGfxDescriptorSetPtr ptrDescriptorSetPass, const CGfxDescriptorSetPtr ptrDescriptorSetInputAttachment, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass, const CGfxPipelineGraphics* pPipeline, uint32_t matPassName, const glm::vec4& scissor, const glm::vec4& viewport)
		: m_indexQueue(indexQueue)

		, m_ptrDescriptorSetPass(ptrDescriptorSetPass)
		, m_ptrDescriptorSetInputAttachment(ptrDescriptorSetInputAttachment)

		, m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
		, m_indexSubpass(indexSubpass)

		, m_pPipeline(pPipeline)
		, m_matPassName(matPassName)

		, m_scissor(scissor)
		, m_viewport(viewport)
	{

	}
	virtual ~CTaskCommandBuffer(void)
	{

	}


public:
	CGfxCommandBufferPtr GetCommandBuffer(void) const
	{
		return m_ptrCommandBuffer;
	}

	void TaskFunc(int indexThread, void* pParams)
	{
		CGfxRenderQueue* pRenderQueue = (CGfxRenderQueue*)pParams;

		m_ptrCommandBuffer = GfxRenderer()->NewCommandBuffer(indexThread, false);
		pRenderQueue->CmdDrawThread(m_indexQueue, m_ptrCommandBuffer, m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass, m_ptrDescriptorSetPass, m_ptrDescriptorSetInputAttachment, m_pPipeline, m_matPassName, m_scissor, m_viewport);
	}


private:
	const int m_indexQueue;

	const CGfxDescriptorSetPtr m_ptrDescriptorSetPass;
	const CGfxDescriptorSetPtr m_ptrDescriptorSetInputAttachment;

	const CGfxFrameBufferPtr m_ptrFrameBuffer;
	const CGfxRenderPassPtr m_ptrRenderPass;
	const int m_indexSubpass;

	const CGfxPipelineGraphics* m_pPipeline;
	const uint32_t m_matPassName;

	const glm::vec4 m_scissor;
	const glm::vec4 m_viewport;

private:
	CGfxCommandBufferPtr m_ptrCommandBuffer;
};


CGfxRenderQueue::CGfxRenderQueue(void)
	: m_taskGraph("TashGraph_RenderQueue")
{

}

CGfxRenderQueue::~CGfxRenderQueue(void)
{

}

void CGfxRenderQueue::Clear(int indexQueue)
{
	m_pipelineMaterialQueue[indexQueue].clear();
	m_materialMeshDrawQueue[indexQueue].clear();

	for (int indexThread = 0; indexThread < MAX_THREAD_COUNT; indexThread++) {
		m_materialMeshDrawQueueThreads[indexThread][indexQueue].clear();
	}
}

void CGfxRenderQueue::Begin(int indexQueue)
{
	Clear(indexQueue);
}

void CGfxRenderQueue::Add(int indexQueue, int indexThread, const CGfxMaterialPtr ptrMaterial, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceData, uint32_t size)
{
	if (indexThread >= 0 && indexThread < MAX_THREAD_COUNT) {
		eastl::vector<uint8_t>& meshDrawInstanceBuffer = m_materialMeshDrawQueueThreads[indexThread][indexQueue][ptrMaterial][ptrMeshDraw];
		meshDrawInstanceBuffer.insert(meshDrawInstanceBuffer.end(), pInstanceData, pInstanceData + size);
	}
	else {
		ASSERT(false);
	}
}

void CGfxRenderQueue::End(int indexQueue)
{
	m_materialMeshDrawQueue[indexQueue].clear();

	for (int indexThread = 0; indexThread < MAX_THREAD_COUNT; indexThread++) {
		for (const auto& itMaterialMeshDrawQueueThread : m_materialMeshDrawQueueThreads[indexThread][indexQueue]) {
			eastl::unordered_map<CGfxMeshDrawPtr, eastl::vector<uint8_t>>& meshDrawQueue = m_materialMeshDrawQueue[indexQueue][itMaterialMeshDrawQueueThread.first];

			for (const auto& itMeshDrawQueueThread : itMaterialMeshDrawQueueThread.second) {
				eastl::vector<uint8_t>& meshDrawInstanceBuffer = meshDrawQueue[itMeshDrawQueueThread.first];
				meshDrawInstanceBuffer.insert(meshDrawInstanceBuffer.end(), itMeshDrawQueueThread.second.begin(), itMeshDrawQueueThread.second.end());
			}
		}

		m_materialMeshDrawQueueThreads[indexThread][indexQueue].clear();
	}
}

void CGfxRenderQueue::CmdDraw(int indexQueue, CGfxCommandBufferPtr ptrCommandBuffer, const CGfxDescriptorSetPtr ptrDescriptorSetPass, const uint32_t matPassName, const glm::vec4& scissor, const glm::vec4& viewport)
{
	m_pipelineMaterialQueue[indexQueue].clear();
	{
		for (const auto& itMaterialQueue : m_materialMeshDrawQueue[indexQueue]) {
			if (CGfxMaterialPass* pPass = (CGfxMaterialPass*)itMaterialQueue.first->GetPass(matPassName)) {
				if (CGfxPipelineGraphics* pPipeline = (CGfxPipelineGraphics*)pPass->GetPipeline()) {
					m_pipelineMaterialQueue[indexQueue][pPipeline][itMaterialQueue.first] = itMaterialQueue.first;
				}
			}
		}

		if (m_pipelineMaterialQueue[indexQueue].empty()) {
			return;
		}
	}

	eastl::vector<CTaskCommandBuffer> tasks;
	{
		for (const auto& itPipelineQueue : m_pipelineMaterialQueue[indexQueue]) {
			CGfxDescriptorSetPtr ptrDescriptorSetInputAttachment = GfxRenderer()->NewDescriptorSet(itPipelineQueue.first, ptrCommandBuffer->GetFrameBuffer(), ptrCommandBuffer->GetRenderPass(), ptrCommandBuffer->GetSubpassIndex());
			tasks.emplace_back(indexQueue, ptrDescriptorSetPass, ptrDescriptorSetInputAttachment, ptrCommandBuffer->GetFrameBuffer(), ptrCommandBuffer->GetRenderPass(), ptrCommandBuffer->GetSubpassIndex(), itPipelineQueue.first, matPassName, scissor, viewport);
		}

		for (int indexTask = 0; indexTask < tasks.size(); indexTask++) {
			m_taskGraph.Task(&tasks[indexTask], this, nullptr);
		}

		m_taskGraph.Dispatch();
		m_taskGraph.Wait();

		for (int indexTask = 0; indexTask < tasks.size(); indexTask++) {
			ptrCommandBuffer->CmdExecute(tasks[indexTask].GetCommandBuffer());
		}
	}
}

void CGfxRenderQueue::CmdDrawThread(int indexQueue, CGfxCommandBufferPtr ptrCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, const int indexSubpass, const CGfxDescriptorSetPtr ptrDescriptorSetPass, const CGfxDescriptorSetPtr ptrDescriptorSetInputAttachment, const CGfxPipelineGraphics* pPipeline, const uint32_t matPassName, const glm::vec4& scissor, const glm::vec4& viewport)
{
	GfxRenderer()->BeginRecord(ptrCommandBuffer, ptrFrameBuffer, ptrRenderPass, indexSubpass);
	{
		GfxRenderer()->CmdSetScissor(ptrCommandBuffer, (int)scissor.x, (int)scissor.y, (int)scissor.z, (int)scissor.w);
		GfxRenderer()->CmdSetViewport(ptrCommandBuffer, (int)viewport.x, (int)viewport.y, (int)viewport.z, (int)viewport.w);

		GfxRenderer()->CmdBindPipelineGraphics(ptrCommandBuffer, pPipeline);
		GfxRenderer()->CmdBindDescriptorSet(ptrCommandBuffer, ptrDescriptorSetPass);
		GfxRenderer()->CmdBindDescriptorSet(ptrCommandBuffer, ptrDescriptorSetInputAttachment);

		for (const auto& itMaterial : m_pipelineMaterialQueue[indexQueue][pPipeline]) {
			GfxRenderer()->CmdBindDescriptorSet(ptrCommandBuffer, itMaterial.first->GetPass(matPassName)->GetDescriptorSet());

			for (const auto& itMeshDrawQueue : m_materialMeshDrawQueue[indexQueue][itMaterial.first]) {
				GfxRenderer()->CmdUpdateInstanceBuffer(ptrCommandBuffer, itMeshDrawQueue.first, itMeshDrawQueue.second.data(), itMeshDrawQueue.second.size());
				GfxRenderer()->CmdDrawInstance(ptrCommandBuffer, itMeshDrawQueue.first);
//				GfxRenderer()->CmdDrawIndirect(ptrCommandBuffer, itMeshDrawQueue.first);
			}
		}
	}
	GfxRenderer()->EndRecord(ptrCommandBuffer);
}
