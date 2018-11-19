#include "GfxHeader.h"


CTaskCommandBuffer::CTaskCommandBuffer(int indexQueue, const CGfxUniformEnginePtr &ptrUniformEngine, const CGfxUniformCameraPtr &ptrUniformCamera, CGfxPipelineGraphics *pPipeline, uint32_t namePass)
	: m_indexQueue(indexQueue)
	, m_ptrUniformEngine(ptrUniformEngine)
	, m_ptrUniformCamera(ptrUniformCamera)
	, m_pPipeline(pPipeline)
	, m_namePass(namePass)
{
	m_ptrCommandBuffer = Renderer()->NewCommandBuffer(false);
}

CTaskCommandBuffer::~CTaskCommandBuffer(void)
{

}

const CGfxCommandBufferPtr& CTaskCommandBuffer::GetCommandBuffer(void) const
{
	return m_ptrCommandBuffer;
}

void CTaskCommandBuffer::TaskFunc(void *pParams)
{
	if (CGfxRenderQueue *pRenderQueue = (CGfxRenderQueue *)pParams) {
		pRenderQueue->CmdDrawThread(m_indexQueue, m_ptrCommandBuffer, m_ptrUniformEngine, m_ptrUniformCamera, m_pPipeline, m_namePass);
	}
}


CGfxRenderQueue::CGfxRenderQueue(void)
	: m_taskGraph("TashGraph_RenderQueue", 75)
{

}

CGfxRenderQueue::~CGfxRenderQueue(void)
{

}

void CGfxRenderQueue::Clear(int indexQueue)
{
	m_tasks[indexQueue].clear();
	m_pipelineMeshQueue[indexQueue].clear();
	m_materialMeshQueue[indexQueue].clear();

	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		m_materialMeshQueueThreads[indexThread][indexQueue].clear();
	}
}

void CGfxRenderQueue::Begin(int indexQueue)
{
	Clear(indexQueue);
}

void CGfxRenderQueue::End(int indexQueue)
{
	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		for (const auto &itMaterialMeshQueue : m_materialMeshQueueThreads[indexThread][indexQueue]) {
			eastl::unordered_map<CGfxMeshPtr, eastl::unordered_map<int, eastl::vector<uint8_t>>> &meshQueue = m_materialMeshQueue[indexQueue][itMaterialMeshQueue.first];

			for (const auto &itMeshQueue : itMaterialMeshQueue.second) {
				eastl::unordered_map<int, eastl::vector<uint8_t>> &drawQueue = meshQueue[itMeshQueue.first];

				for (const auto &itDrawQueue : itMeshQueue.second) {
					eastl::vector<uint8_t> &instanceQueue = drawQueue[itDrawQueue.first];
					instanceQueue.insert(instanceQueue.end(), itDrawQueue.second.begin(), itDrawQueue.second.end());
				}
			}
		}
	}
}

void CGfxRenderQueue::Add(int indexThread, int indexQueue, const CGfxMaterialPtr &ptrMaterial, const CGfxMeshPtr &ptrMesh, int indexDraw, const uint8_t *pInstanceData, uint32_t size)
{
	if (indexThread >= 0 && indexThread < THREAD_COUNT) {
		eastl::vector<uint8_t> &instanceQueue = m_materialMeshQueueThreads[indexThread][indexQueue][ptrMaterial][ptrMesh][indexDraw];
		instanceQueue.insert(instanceQueue.end(), pInstanceData, pInstanceData + size);
	}
}

void CGfxRenderQueue::CmdDraw(int indexQueue, CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxUniformEnginePtr &ptrUniformEngine, const CGfxUniformCameraPtr &ptrUniformCamera, uint32_t namePass)
{
	m_tasks[indexQueue].clear();
	m_pipelineMeshQueue[indexQueue].clear();

	for (const auto &itMaterialQueue : m_materialMeshQueue[indexQueue]) {
		if (CGfxMaterialPass *pPass = itMaterialQueue.first->GetPass(namePass)) {
			if (CGfxPipelineGraphics *pPipeline = pPass->GetPipeline()) {
				m_pipelineMeshQueue[indexQueue][pPipeline][itMaterialQueue.first] = itMaterialQueue.first;
			}
		}
	}

	if (m_pipelineMeshQueue[indexQueue].empty()) {
		return;
	}

	for (const auto &itPipelineQueue : m_pipelineMeshQueue[indexQueue]) {
		m_tasks[indexQueue].emplace_back(indexQueue, ptrUniformEngine, ptrUniformCamera, itPipelineQueue.first, namePass);
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

void CGfxRenderQueue::CmdDrawThread(int indexQueue, CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxUniformEnginePtr &ptrUniformEngine, const CGfxUniformCameraPtr &ptrUniformCamera, CGfxPipelineGraphics *pPipeline, uint32_t namePass)
{
	Renderer()->CmdBindPipelineGraphics(ptrCommandBuffer, (CGfxPipelineGraphics *)pPipeline);
	Renderer()->CmdBindUniformEngine(ptrCommandBuffer, ptrUniformEngine);
	Renderer()->CmdBindUniformCamera(ptrCommandBuffer, ptrUniformCamera);

	for (const auto &itMaterial : m_pipelineMeshQueue[indexQueue][pPipeline]) {
		Renderer()->CmdBindMaterialPass(ptrCommandBuffer, itMaterial.first, namePass);

		for (const auto &itMeshQueue : m_materialMeshQueue[indexQueue][itMaterial.first]) {
			for (const auto itDrawQueue : itMeshQueue.second) {
				if (itDrawQueue.first == -1) {
					Renderer()->CmdDrawInstance(ptrCommandBuffer, itMeshQueue.first, itDrawQueue.second.data(), itDrawQueue.second.size());
				}
				else {
					Renderer()->CmdDrawIndirect(ptrCommandBuffer, itMeshQueue.first, itDrawQueue.second.data(), itDrawQueue.second.size());
				}
			}
		}
	}
}
