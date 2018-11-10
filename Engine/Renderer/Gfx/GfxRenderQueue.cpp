#include "GfxRenderer.h"
#include "GfxRenderQueue.h"


CGfxRenderQueue::CGfxRenderQueue(void)
{

}

CGfxRenderQueue::~CGfxRenderQueue(void)
{

}

void CGfxRenderQueue::AddMesh(int indexThread, int indexQueue, const CGfxMaterialPtr &ptrMaterial, const CGfxMeshPtr &ptrMesh, const uint8_t *pInstanceData, uint32_t size)
{
	if (indexThread >= 0 && indexThread < THREAD_COUNT) {
		m_materialMeshQueue[indexThread][indexQueue][ptrMaterial][ptrMesh].insert(m_materialMeshQueue[indexThread][indexQueue][ptrMaterial][ptrMesh].end(), pInstanceData, pInstanceData + size);
	}
}

void CGfxRenderQueue::Clear(int indexQueue)
{
	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		m_materialMeshQueue[indexThread][indexQueue].clear();
	}
}

void CGfxRenderQueue::CmdDraw(CGfxCamera *pCamera, CGfxCommandBufferPtr &ptrCommandBuffer, int indexThread, int indexQueue, uint32_t namePass)
{
	eastl::unordered_map<const CGfxPipelineGraphics*, eastl::unordered_map<CGfxMaterialPtr, CGfxMaterialPtr>> pipelineQueue;
	{
		for (const auto &itMaterialQueue : m_materialMeshQueue[indexThread][indexQueue]) {
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
		Renderer()->CmdBindUniformCamera(ptrCommandBuffer, pCamera->m_ptrUniformCamera);

		for (const auto &itMaterial : itPipelineQueue.second) {
			Renderer()->CmdBindMaterialPass(ptrCommandBuffer, itMaterial.first, namePass);

			for (const auto &itMeshQueue : m_materialMeshQueue[indexThread][indexQueue][itMaterial.first]) {
				Renderer()->CmdDrawInstance(ptrCommandBuffer, itMeshQueue.first, itMeshQueue.second.data(), itMeshQueue.second.size());
//				Renderer()->CmdDrawIndirect(ptrCommandBuffer, itMeshQueue.first, itMeshQueue.second.data(), itMeshQueue.second.size());
			}
		}
	}
}
