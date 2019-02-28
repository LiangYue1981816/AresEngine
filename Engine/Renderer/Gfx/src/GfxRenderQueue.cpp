#include "GfxHeader.h"


CTaskCommandBuffer::CTaskCommandBuffer(int indexQueue, const CGfxUniformBufferPtr ptrUniformBufferEngine, const CGfxUniformBufferPtr ptrUniformBufferCamera, CGfxPipelineGraphics *pPipeline, uint32_t namePass)
	: m_indexQueue(indexQueue)
	, m_ptrUniformBufferEngine(ptrUniformBufferEngine)
	, m_ptrUniformBufferCamera(ptrUniformBufferCamera)
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

void CTaskCommandBuffer::TaskFunc(void *pParams)
{
	m_ptrCommandBuffer = GfxRenderer()->NewCommandBuffer(0, false);

	if (CGfxRenderQueue *pRenderQueue = (CGfxRenderQueue *)pParams) {
		pRenderQueue->CmdDrawThread(m_indexQueue, m_ptrCommandBuffer, m_ptrUniformBufferEngine, m_ptrUniformBufferCamera, m_pPipeline, m_namePass);
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
	m_tasks[indexQueue].clear();
	m_pipelineMaterialQueue[indexQueue].clear();
	m_materialMeshDrawQueue[indexQueue].clear();

	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		m_materialMeshDrawQueueThreads[indexThread][indexQueue].clear();
	}
}

void CGfxRenderQueue::Begin(int indexQueue)
{
	Clear(indexQueue);
}

void CGfxRenderQueue::Add(int indexThread, int indexQueue, const CGfxMaterialPtr ptrMaterial, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t *pInstanceData, uint32_t size)
{
	if (indexThread >= 0 && indexThread < THREAD_COUNT) {
		eastl::vector<uint8_t> &meshDrawInstanceBuffer = m_materialMeshDrawQueueThreads[indexThread][indexQueue][ptrMaterial][ptrMeshDraw];
		meshDrawInstanceBuffer.insert(meshDrawInstanceBuffer.end(), pInstanceData, pInstanceData + size);
	}
}

void CGfxRenderQueue::End(int indexQueue)
{
	m_materialMeshDrawQueue[indexQueue].clear();

	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
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

void CGfxRenderQueue::CmdDraw(int indexQueue, CGfxCommandBufferPtr ptrCommandBuffer, const CGfxUniformBufferPtr ptrUniformBufferEngine, const CGfxUniformBufferPtr ptrUniformBufferCamera, uint32_t namePass)
{
	m_tasks[indexQueue].clear();
	m_pipelineMaterialQueue[indexQueue].clear();

	for (const auto &itMaterialQueue : m_materialMeshDrawQueue[indexQueue]) {
		if (CGfxMaterialPass *pPass = itMaterialQueue.first->GetPass(namePass)) {
			if (CGfxPipelineGraphics *pPipeline = pPass->GetPipeline()) {
				m_pipelineMaterialQueue[indexQueue][pPipeline][itMaterialQueue.first] = itMaterialQueue.first;
			}
		}
	}

	if (m_pipelineMaterialQueue[indexQueue].empty()) {
		return;
	}

	for (const auto &itPipelineQueue : m_pipelineMaterialQueue[indexQueue]) {
		m_tasks[indexQueue].emplace_back(indexQueue, ptrUniformBufferEngine, ptrUniformBufferCamera, itPipelineQueue.first, namePass);
	}

	for (int indexTask = 0; indexTask < (int)m_tasks[indexQueue].size(); indexTask++) {
		m_taskGraph.Task(&m_tasks[indexQueue][indexTask], this, nullptr);
	}

	m_taskGraph.Dispatch();
	m_taskGraph.Wait();

	for (int indexTask = 0; indexTask < (int)m_tasks[indexQueue].size(); indexTask++) {
		ptrCommandBuffer->CmdExecute(m_tasks[indexQueue][indexTask].GetCommandBuffer());
	}
}

void CGfxRenderQueue::CmdDrawThread(int indexQueue, CGfxCommandBufferPtr ptrCommandBuffer, const CGfxUniformBufferPtr ptrUniformBufferEngine, const CGfxUniformBufferPtr ptrUniformBufferCamera, CGfxPipelineGraphics *pPipeline, uint32_t namePass)
{
	static const uint32_t nameUniformEngine = HashValue(UNIFORM_ENGINE_NAME);
	static const uint32_t nameUniformCamera = HashValue(UNIFORM_CAMERA_NAME);

	GfxRenderer()->CmdBindPipelineGraphics(ptrCommandBuffer, (CGfxPipelineGraphics *)pPipeline);
	GfxRenderer()->CmdBindUniformBuffer(ptrCommandBuffer, ptrUniformBufferEngine, nameUniformEngine);
	GfxRenderer()->CmdBindUniformBuffer(ptrCommandBuffer, ptrUniformBufferCamera, nameUniformCamera);

	for (const auto &itMaterial : m_pipelineMaterialQueue[indexQueue][pPipeline]) {
		GfxRenderer()->CmdBindMaterialPass(ptrCommandBuffer, itMaterial.first, namePass);

		for (const auto &itMeshDrawQueue : m_materialMeshDrawQueue[indexQueue][itMaterial.first]) {
			GfxRenderer()->CmdSetInstanceBufferData(ptrCommandBuffer, itMeshDrawQueue.first, itMeshDrawQueue.second.data(), itMeshDrawQueue.second.size());
			GfxRenderer()->CmdDrawInstance(ptrCommandBuffer, itMeshDrawQueue.first, itMeshDrawQueue.second.size() / GetInstanceStride(itMeshDrawQueue.first->GetInstanceFormat()));
//			GfxRenderer()->CmdDrawIndirect(ptrCommandBuffer, itMeshDrawQueue.first);
		}
	}
}
