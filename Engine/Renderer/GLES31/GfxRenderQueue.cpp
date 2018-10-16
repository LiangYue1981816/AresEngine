#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxRenderQueue.h"


CGfxRenderQueue::CGfxRenderQueue(void)
{

}

CGfxRenderQueue::~CGfxRenderQueue(void)
{

}

void CGfxRenderQueue::AddQueue(int indexThread, int indexQueue, const CGfxMaterialPtr &ptrMaterial, const CGfxMeshPtr &ptrMesh, const glm::mat4 &mtxTransform)
{
	if (indexThread >= 0 && indexThread < THREAD_COUNT) {
		m_materialQueue[indexThread][indexQueue][ptrMaterial][ptrMesh].emplace_back(mtxTransform);
	}
}

void CGfxRenderQueue::ClearQueueAll(void)
{
	ClearQueue(0);
	ClearQueue(1);
}

void CGfxRenderQueue::ClearQueue(int indexQueue)
{
	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		m_materialQueue[indexThread][indexQueue].clear();
		m_secondaryCommandBuffer[indexThread][indexQueue].clear();
	}
}

void CGfxRenderQueue::CmdDraw(CGfxCamera *pCamera, int indexThread, int indexQueue, uint32_t namePass)
{
	eastl::unordered_map<const CGfxPipelineBase*, eastl::vector<CGfxMaterialPtr>> pipelineQueue;
	{
		for (const auto &itMaterialQueue : m_materialQueue[indexThread][indexQueue]) {
			if (const CGfxMaterialPass *pPass = itMaterialQueue.first->GetPass(namePass)) {
				if (const CGfxPipelineBase *pPipeline = pPass->GetPipeline()) {
					pipelineQueue[pPipeline].emplace_back(itMaterialQueue.first);
				}
			}
		}

		if (pipelineQueue.empty()) {
			return;
		}
	}

	m_secondaryCommandBuffer[indexThread][indexQueue].emplace_back(false);
	CGfxCommandBuffer *pCommandBuffer = &m_secondaryCommandBuffer[indexThread][indexQueue][m_secondaryCommandBuffer[indexThread][indexQueue].size() - 1];
	{
		for (const auto &itPipelineQueue : pipelineQueue) {
			Renderer()->CmdBindPipeline(pCommandBuffer, (CGfxPipelineBase *)itPipelineQueue.first);
			Renderer()->CmdBindCamera(pCommandBuffer, pCamera);

			for (const auto &itMaterial : itPipelineQueue.second) {
				Renderer()->CmdBindMaterialPass(pCommandBuffer, itMaterial, namePass);

				for (const auto &itMeshQueue : m_materialQueue[indexThread][indexQueue][itMaterial]) {
					Renderer()->CmdDrawInstance(pCommandBuffer, itMeshQueue.first, itMeshQueue.first->GetIndexCount(), 0, itMeshQueue.second);
				}
			}
		}
	}
}

void CGfxRenderQueue::CmdExecute(CGfxCommandBuffer *pMainCommandBuffer, int indexQueue)
{
	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		for (const auto &itCommandBuffer : m_secondaryCommandBuffer[indexThread][indexQueue]) {
			Renderer()->CmdExecute(pMainCommandBuffer, (CGfxCommandBuffer *)&itCommandBuffer);
		}
	}
}
