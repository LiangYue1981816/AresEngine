#include "GfxRenderer.h"
#include "GfxRenderQueue.h"


CGfxRenderQueue::CGfxRenderQueue(void)
{

}

CGfxRenderQueue::~CGfxRenderQueue(void)
{

}

void CGfxRenderQueue::Clear(int indexQueue)
{
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
	eastl::unordered_map<const CGfxPipelineGraphics*, eastl::unordered_map<CGfxMaterialPtr, CGfxMaterialPtr>> pipelineQueue;
	{
		for (const auto &itMaterialQueue : m_materialMeshQueue[indexQueue]) {
			if (const CGfxMaterialPass *pPass = itMaterialQueue.first->GetPass(namePass)) {
				if (const CGfxPipelineGraphics *pPipeline = pPass->GetPipeline()) {
					pipelineQueue[pPipeline][itMaterialQueue.first] = itMaterialQueue.first;
				}
			}
		}

		if (pipelineQueue.empty()) {
			return;
		}
	}

	for (const auto &itPipelineQueue : pipelineQueue) {
		Renderer()->CmdBindPipelineGraphics(ptrCommandBuffer, (CGfxPipelineGraphics *)itPipelineQueue.first);
		Renderer()->CmdBindUniformEngine(ptrCommandBuffer, ptrUniformEngine);
		Renderer()->CmdBindUniformCamera(ptrCommandBuffer, ptrUniformCamera);

		for (const auto &itMaterial : itPipelineQueue.second) {
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
}
