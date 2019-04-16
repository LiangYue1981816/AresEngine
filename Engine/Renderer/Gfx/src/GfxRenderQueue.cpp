#include "GfxHeader.h"


CTaskCommandBuffer::CTaskCommandBuffer(int indexQueue, const CGfxDescriptorSetPtr ptrDescriptorSetEngine, const CGfxDescriptorSetPtr ptrDescriptorSetCamera, CGfxPipelineGraphics* pPipeline, uint32_t namePass)
	: m_indexQueue(indexQueue)
	, m_ptrDescriptorSetEngine(ptrDescriptorSetEngine)
	, m_ptrDescriptorSetCamera(ptrDescriptorSetCamera)
	, m_pPipeline(pPipeline)
	, m_namePass(namePass)
{

}

CTaskCommandBuffer::~CTaskCommandBuffer(void)
{

}

CGfxCommandBufferPtr CTaskCommandBuffer::GetCommandBuffer(void) const
{
	return m_ptrCommandBuffer;
}

void CTaskCommandBuffer::TaskFunc(int indexThread, void *pParams)
{
	m_ptrCommandBuffer = GfxRenderer()->NewCommandBuffer(indexThread, false);

	if (CGfxRenderQueue *pRenderQueue = (CGfxRenderQueue *)pParams) {
		pRenderQueue->CmdDrawThread(m_indexQueue, m_ptrCommandBuffer, m_ptrDescriptorSetEngine, m_ptrDescriptorSetCamera, m_pPipeline, m_namePass);
	}
}


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

void CGfxRenderQueue::Add(int indexQueue, int indexThread, const CGfxMaterialPtr ptrMaterial, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t *pInstanceData, uint32_t size)
{
	if (indexThread >= 0 && indexThread < MAX_THREAD_COUNT) {
		eastl::vector<uint8_t> &meshDrawInstanceBuffer = m_materialMeshDrawQueueThreads[indexThread][indexQueue][ptrMaterial][ptrMeshDraw];
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
		for (const auto &itMaterialMeshDrawQueueThread : m_materialMeshDrawQueueThreads[indexThread][indexQueue]) {
			eastl::unordered_map<CGfxMeshDrawPtr, eastl::vector<uint8_t>> &meshDrawQueue = m_materialMeshDrawQueue[indexQueue][itMaterialMeshDrawQueueThread.first];

			for (const auto &itMeshDrawQueueThread : itMaterialMeshDrawQueueThread.second) {
				eastl::vector<uint8_t> &meshDrawInstanceBuffer = meshDrawQueue[itMeshDrawQueueThread.first];
				meshDrawInstanceBuffer.insert(meshDrawInstanceBuffer.end(), itMeshDrawQueueThread.second.begin(), itMeshDrawQueueThread.second.end());
			}
		}

		m_materialMeshDrawQueueThreads[indexThread][indexQueue].clear();
	}
}

void CGfxRenderQueue::CmdDraw(int indexQueue, CGfxCommandBufferPtr ptrCommandBuffer, const CGfxDescriptorSetPtr ptrDescriptorSetEngine, const CGfxDescriptorSetPtr ptrDescriptorSetCamera, uint32_t namePass)
{
	m_pipelineMaterialQueue[indexQueue].clear();
	{
		for (const auto &itMaterialQueue : m_materialMeshDrawQueue[indexQueue]) {
			if (CGfxMaterialPass *pPass = (CGfxMaterialPass *)itMaterialQueue.first->GetPass(namePass)) {
				if (CGfxPipelineGraphics *pPipeline = (CGfxPipelineGraphics *)pPass->GetPipeline()) {
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
		for (const auto &itPipelineQueue : m_pipelineMaterialQueue[indexQueue]) {
			tasks.emplace_back(indexQueue, ptrDescriptorSetEngine, ptrDescriptorSetCamera, itPipelineQueue.first, namePass);
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

void CGfxRenderQueue::CmdDrawThread(int indexQueue, CGfxCommandBufferPtr ptrCommandBuffer, const CGfxDescriptorSetPtr ptrDescriptorSetEngine, const CGfxDescriptorSetPtr ptrDescriptorSetCamera, CGfxPipelineGraphics *pPipeline, uint32_t namePass)
{
//	CGfxDescriptorSetPtr ptrDescriptorSetInputAttachment = GfxRenderer()->NewDescriptorSet(pPipeline, ptrCommandBuffer->GetFrameBuffer(), ptrCommandBuffer->GetRenderPass(), ptrCommandBuffer->GetSubpassIndex());

	GfxRenderer()->CmdBindPipelineGraphics(ptrCommandBuffer, pPipeline);
	GfxRenderer()->CmdBindDescriptorSet(ptrCommandBuffer, ptrDescriptorSetEngine);
	GfxRenderer()->CmdBindDescriptorSet(ptrCommandBuffer, ptrDescriptorSetCamera);
//	GfxRenderer()->CmdBindDescriptorSet(ptrCommandBuffer, ptrDescriptorSetInputAttachment);

	for (const auto &itMaterial : m_pipelineMaterialQueue[indexQueue][pPipeline]) {
		GfxRenderer()->CmdBindDescriptorSet(ptrCommandBuffer, itMaterial.first->GetPass(namePass)->GetDescriptorSet());

		for (const auto &itMeshDrawQueue : m_materialMeshDrawQueue[indexQueue][itMaterial.first]) {
			GfxRenderer()->CmdDrawInstance(ptrCommandBuffer, itMeshDrawQueue.first, itMeshDrawQueue.second.data(), itMeshDrawQueue.second.size());
//			GfxRenderer()->CmdDrawIndirect(ptrCommandBuffer, itMeshDrawQueue.first, itMeshDrawQueue.second.data(), itMeshDrawQueue.second.size());
		}
	}
}
