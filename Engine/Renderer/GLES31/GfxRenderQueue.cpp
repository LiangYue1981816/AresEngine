#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxRenderQueue.h"


CGfxRenderQueue::CGfxRenderQueue(void)
{

}

CGfxRenderQueue::~CGfxRenderQueue(void)
{

}

void CGfxRenderQueue::AddMesh(int indexThread, int indexQueue, const CGfxMaterialPtr &ptrMaterial, const CGfxMeshPtr &ptrMesh, const glm::mat4 &mtxTransform)
{
	if (indexThread >= 0 && indexThread < THREAD_COUNT) {
		m_materialMeshQueue[indexThread][indexQueue][ptrMaterial][ptrMesh].emplace_back(mtxTransform);
	}
}

void CGfxRenderQueue::AddMeshIndirect(int indexThread, int indexQueue, const CGfxMaterialPtr &ptrMaterial, const CGfxMeshPtr &ptrMesh, const CGfxDrawIndirectBufferPtr &ptrDrawIndirectBuffer, const glm::mat4 &mtxTransform)
{
	if (indexThread >= 0 && indexThread < THREAD_COUNT) {
		m_materialMeshIndirectQueue[indexThread][indexQueue][ptrMaterial][ptrMesh][ptrDrawIndirectBuffer].emplace_back(mtxTransform);
	}
}

void CGfxRenderQueue::AddPointLight(int indexThread, int indexQueue, const CGfxMaterialPtr &ptrMaterial, const CGfxMeshPtr &ptrMesh, const glm::mat4 &mtxTransform)
{
	if (indexThread >= 0 && indexThread < THREAD_COUNT) {
		m_materialPointLightQueue[indexThread][indexQueue][ptrMaterial][ptrMesh].emplace_back(mtxTransform);
	}
}

void CGfxRenderQueue::AddPointLightIndirect(int indexThread, int indexQueue, const CGfxMaterialPtr &ptrMaterial, const CGfxMeshPtr &ptrMesh, const CGfxDrawIndirectBufferPtr &ptrDrawIndirectBuffer, const glm::mat4 &mtxTransform)
{
	if (indexThread >= 0 && indexThread < THREAD_COUNT) {
		m_materialPointLightIndirectQueue[indexThread][indexQueue][ptrMaterial][ptrMesh][ptrDrawIndirectBuffer].emplace_back(mtxTransform);
	}
}

void CGfxRenderQueue::Clear(int indexQueue)
{
	for (int indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		m_materialMeshQueue[indexThread][indexQueue].clear();
		m_materialMeshIndirectQueue[indexThread][indexQueue].clear();

		m_materialPointLightQueue[indexThread][indexQueue].clear();
		m_materialPointLightIndirectQueue[indexThread][indexQueue].clear();
	}
}

void CGfxRenderQueue::CmdDraw(CGfxCamera *pCamera, CGfxCommandBuffer *pCommandBuffer, int indexThread, int indexQueue, uint32_t namePass)
{
	CmdDrawMesh(pCamera, pCommandBuffer, indexThread, indexQueue, namePass);
	CmdDrawPointLight(pCamera, pCommandBuffer, indexThread, indexQueue, namePass);
}

void CGfxRenderQueue::CmdDrawMesh(CGfxCamera *pCamera, CGfxCommandBuffer *pCommandBuffer, int indexThread, int indexQueue, uint32_t namePass)
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
				Renderer()->CmdDrawInstance(pCommandBuffer, itMeshQueue.first, 0, itMeshQueue.first->GetIndexCount(), itMeshQueue.second);
			}

			for (const auto &itMeshIndirectQueue : m_materialMeshIndirectQueue[indexThread][indexQueue][itMaterial.first]) {
				for (const auto &itIndirectQueue : itMeshIndirectQueue.second) {
					Renderer()->CmdDrawIndirect(pCommandBuffer, itMeshIndirectQueue.first, itIndirectQueue.first, 0, itIndirectQueue.second);
//					Renderer()->CmdDrawIndirect(pCommandBuffer, itMeshIndirectQueue.first, itIndirectQueue.first->GetBaseVertex(), itIndirectQueue.first->GetFirstIndex(), itIndirectQueue.first->GetIndexCount(), itIndirectQueue.second);
				}
			}
		}
	}
}

void CGfxRenderQueue::CmdDrawPointLight(CGfxCamera *pCamera, CGfxCommandBuffer *pCommandBuffer, int indexThread, int indexQueue, uint32_t namePass)
{
	eastl::unordered_map<const CGfxPipelineBase*, eastl::unordered_map<CGfxMaterialPtr, CGfxMaterialPtr>> pipelineQueue;
	{
		for (const auto &itMaterialQueue : m_materialPointLightQueue[indexThread][indexQueue]) {
			if (const CGfxMaterialPass *pPass = itMaterialQueue.first->GetPass(namePass)) {
				if (const CGfxPipelineBase *pPipeline = pPass->GetPipeline()) {
					pipelineQueue[pPipeline][itMaterialQueue.first] = itMaterialQueue.first;
				}
			}
		}

		for (const auto &itMaterialQueue : m_materialPointLightIndirectQueue[indexThread][indexQueue]) {
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

			for (const auto &itMeshQueue : m_materialPointLightQueue[indexThread][indexQueue][itMaterial.first]) {
				Renderer()->CmdDrawInstance(pCommandBuffer, itMeshQueue.first, 0, itMeshQueue.first->GetIndexCount(), itMeshQueue.second);
			}

			for (const auto &itMeshIndirectQueue : m_materialPointLightIndirectQueue[indexThread][indexQueue][itMaterial.first]) {
				for (const auto &itIndirectQueue : itMeshIndirectQueue.second) {
					Renderer()->CmdDrawIndirect(pCommandBuffer, itMeshIndirectQueue.first, itIndirectQueue.first, 0, itIndirectQueue.second);
//					Renderer()->CmdDrawIndirect(pCommandBuffer, itMeshIndirectQueue.first, itIndirectQueue.first->GetBaseVertex(), itIndirectQueue.first->GetFirstIndex(), itIndirectQueue.first->GetIndexCount(), itIndirectQueue.second);
				}
			}
		}
	}
}
