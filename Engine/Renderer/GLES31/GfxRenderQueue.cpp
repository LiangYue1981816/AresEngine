#include "Engine.h"
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

void CGfxRenderQueue::AddMeshIndirect(int indexThread, int indexQueue, const CGfxMaterialPtr &ptrMaterial, const CGfxMeshPtr &ptrMesh, const CGfxDrawIndirectBufferPtr &ptrDrawIndirectBuffer, const uint8_t *pInstanceData, uint32_t size)
{
	if (Renderer()->IsSupportGLES31()) {
		if (indexThread >= 0 && indexThread < THREAD_COUNT) {
			m_materialMeshIndirectQueue[indexThread][indexQueue][ptrMaterial][ptrMesh][ptrDrawIndirectBuffer].insert(m_materialMeshIndirectQueue[indexThread][indexQueue][ptrMaterial][ptrMesh][ptrDrawIndirectBuffer].end(), pInstanceData, pInstanceData + size);
		}
	}
	else {
		if (indexThread >= 0 && indexThread < THREAD_COUNT) {
			m_materialMeshQueue[indexThread][indexQueue][ptrMaterial][ptrMesh].insert(m_materialMeshQueue[indexThread][indexQueue][ptrMaterial][ptrMesh].end(), pInstanceData, pInstanceData + size);
		}
	}
}

void CGfxRenderQueue::Clear(int indexQueue)
{
	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		m_materialMeshQueue[indexThread][indexQueue].clear();
		m_materialMeshIndirectQueue[indexThread][indexQueue].clear();
	}
}

void CGfxRenderQueue::CmdDraw(CGfxCamera *pCamera, CGfxCommandBuffer *pCommandBuffer, int indexThread, int indexQueue, uint32_t namePass)
{
	eastl::unordered_map<const CGfxPipelineBase*, eastl::unordered_map<CGfxMaterialPtr, CGfxMaterialPtr>> pipelineQueue;
	{
		for (const auto &itMaterialQueue : m_materialMeshQueue[indexThread][indexQueue]) {
			if (const CGfxMaterialPass *pPass = itMaterialQueue.first->GetPass(namePass)) {
				if (const CGfxPipelineBase *pPipeline = pPass->GetPipeline()) {
					pipelineQueue[pPipeline][itMaterialQueue.first] = itMaterialQueue.first;
				}
			}
		}

		for (const auto &itMaterialQueue : m_materialMeshIndirectQueue[indexThread][indexQueue]) {
			if (const CGfxMaterialPass *pPass = itMaterialQueue.first->GetPass(namePass)) {
				if (const CGfxPipelineBase *pPipeline = pPass->GetPipeline()) {
					pipelineQueue[pPipeline][itMaterialQueue.first] = itMaterialQueue.first;
				}
			}
		}

		if (pipelineQueue.empty()) {
			return;
		}
	}

	for (const auto &itPipelineQueue : pipelineQueue) {
		Renderer()->CmdBindPipeline(pCommandBuffer, (CGfxPipelineBase *)itPipelineQueue.first);
		Renderer()->CmdBindCamera(pCommandBuffer, pCamera);

		for (const auto &itMaterial : itPipelineQueue.second) {
			Renderer()->CmdBindMaterialPass(pCommandBuffer, itMaterial.first, namePass);

			for (const auto &itMeshQueue : m_materialMeshQueue[indexThread][indexQueue][itMaterial.first]) {
				Renderer()->CmdDrawInstance(pCommandBuffer, itMeshQueue.first, 0, itMeshQueue.first->GetIndexCount(), itMeshQueue.second.data(), itMeshQueue.second.size());
			}

			for (const auto &itMeshIndirectQueue : m_materialMeshIndirectQueue[indexThread][indexQueue][itMaterial.first]) {
				for (const auto &itIndirectQueue : itMeshIndirectQueue.second) {
					Renderer()->CmdDrawIndirect(pCommandBuffer, itMeshIndirectQueue.first, itIndirectQueue.first, 0, itIndirectQueue.second.data(), itIndirectQueue.second.size());
//					Renderer()->CmdDrawIndirect(pCommandBuffer, itMeshIndirectQueue.first, itIndirectQueue.first->GetBaseVertex(), itIndirectQueue.first->GetFirstIndex(), itIndirectQueue.first->GetIndexCount(), itIndirectQueue.second.data(), itIndirectQueue.second.size());
				}
			}
		}
	}
}
