#include "EngineHeader.h"
#include "RenderHeader.h"


typedef enum SortOrder {
	FrontToBack,
	BackToFront
} SortOrder;

class CTaskSort : public CTask
{
public:
	CTaskSort(eastl::unordered_map<CGfxMeshPtr, eastl::unordered_map<CGfxMeshDrawPtr, eastl::vector<int>>>& meshQueue)
	{
		m_meshQueue = meshQueue;
	}
	virtual ~CTaskSort(void)
	{

	}

public:
	void TaskFunc(int indexThread, void* pParams)
	{
		switch (order) {
		case SortOrder::FrontToBack:
			for (auto& itMeshQueue : m_meshQueue) {
				for (auto& itDrawQueue : itMeshQueue.second) {
					eastl::sort(itDrawQueue.second.begin(), itDrawQueue.second.end(), CompareForFrontToBack);
				}
			}
			break;

		case SortOrder::BackToFront:
			for (auto& itMeshQueue : m_meshQueue) {
				for (auto& itDrawQueue : itMeshQueue.second) {
					eastl::sort(itDrawQueue.second.begin(), itDrawQueue.second.end(), CompareForBackToFront);
				}
			}
			break;
		}
	}

private:
	static bool CompareForFrontToBack(const int& index0, const int& index1)
	{
		const glm::vec4& center0 = RenderSystem()->GetInstanceData(index0).center;
		const glm::vec4& center1 = RenderSystem()->GetInstanceData(index1).center;
		float distance0 = glm::distance2(glm::vec3(center0.x, center0.y, center0.z), cameraPosition);
		float distance1 = glm::distance2(glm::vec3(center1.x, center1.y, center1.z), cameraPosition);
		return distance0 < distance1;
	}

	static bool CompareForBackToFront(const int& index0, const int& index1)
	{
		const glm::vec4& center0 = RenderSystem()->GetInstanceData(index0).center;
		const glm::vec4& center1 = RenderSystem()->GetInstanceData(index1).center;
		float distance0 = glm::distance2(glm::vec3(center0.x, center0.y, center0.z), cameraPosition);
		float distance1 = glm::distance2(glm::vec3(center1.x, center1.y, center1.z), cameraPosition);
		return distance0 > distance1;
	}


public:
	static SortOrder order;
	static glm::vec3 cameraPosition;

private:
	eastl::unordered_map<CGfxMeshPtr, eastl::unordered_map<CGfxMeshDrawPtr, eastl::vector<int>>> m_meshQueue;
};

SortOrder CTaskSort::order;
glm::vec3 CTaskSort::cameraPosition;


class CTaskCommandBuffer : public CTask
{
public:
	CTaskCommandBuffer(const CGfxDescriptorSetPtr ptrDescriptorSetPass, const CGfxDescriptorSetPtr ptrDescriptorSetInputAttachment, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, int indexSubpass, const CGfxPipelineGraphics* pPipeline, uint32_t matPassName, const glm::vec4& scissor, const glm::vec4& viewport, uint32_t mask)
		: m_ptrDescriptorSetPass(ptrDescriptorSetPass)
		, m_ptrDescriptorSetInputAttachment(ptrDescriptorSetInputAttachment)

		, m_ptrFrameBuffer(ptrFrameBuffer)
		, m_ptrRenderPass(ptrRenderPass)
		, m_indexSubpass(indexSubpass)

		, m_pPipeline(pPipeline)
		, m_matPassName(matPassName)

		, m_scissor(scissor)
		, m_viewport(viewport)

		, m_mask(mask)
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
		CRenderQueue* pRenderQueue = (CRenderQueue*)pParams;

		m_ptrCommandBuffer = GfxRenderer()->NewCommandBuffer(indexThread, false);
		pRenderQueue->CmdDrawThread(m_ptrCommandBuffer, m_ptrFrameBuffer, m_ptrRenderPass, m_indexSubpass, m_ptrDescriptorSetPass, m_ptrDescriptorSetInputAttachment, m_pPipeline, m_matPassName, m_scissor, m_viewport, m_mask);
	}


private:
	const CGfxDescriptorSetPtr m_ptrDescriptorSetPass;
	const CGfxDescriptorSetPtr m_ptrDescriptorSetInputAttachment;

	const CGfxFrameBufferPtr m_ptrFrameBuffer;
	const CGfxRenderPassPtr m_ptrRenderPass;
	const int m_indexSubpass;

	const CGfxPipelineGraphics* m_pPipeline;
	const uint32_t m_matPassName;

	const glm::vec4 m_scissor;
	const glm::vec4 m_viewport;

	const uint32_t m_mask;

private:
	CGfxCommandBufferPtr m_ptrCommandBuffer;
};


CRenderQueue::CRenderQueue(void)
	: m_pCamera(nullptr)
{

}

CRenderQueue::~CRenderQueue(void)
{

}

void CRenderQueue::Begin(CGfxCamera* pCamera)
{
	m_pCamera = pCamera;
	m_pipelineMaterialQueue.clear();
	m_materialMeshDrawQueue.clear();
	m_materialMeshDrawInstanceRangeQueue.clear();
}

void CRenderQueue::Add(const CGfxMaterialPtr ptrMaterial, const CGfxMeshDrawPtr ptrMeshDraw, int indexInstance, int indexThread)
{
	if (indexThread >= 0 && indexThread < MAX_THREAD_COUNT) {
		m_materialMeshDrawQueueThreads[indexThread][ptrMaterial][ptrMeshDraw->GetMesh()][ptrMeshDraw].emplace_back(indexInstance);
	}
}

void CRenderQueue::End(void)
{
	m_materialMeshDrawQueue.clear();
	{
		for (int indexThread = 0; indexThread < MAX_THREAD_COUNT; indexThread++) {
			auto& materialQueue = m_materialMeshDrawQueue;
			{
				for (const auto& itMaterialQueueThread : m_materialMeshDrawQueueThreads[indexThread]) {
					auto& meshQueue = materialQueue[itMaterialQueueThread.first];
					{
						for (const auto& itMeshQueueThread : itMaterialQueueThread.second) {
							auto& drawQueue = meshQueue[itMeshQueueThread.first];
							{
								for (const auto& itDrawQueueThread : itMeshQueueThread.second) {
									auto& instanceBuffer = drawQueue[itDrawQueueThread.first];
									{
										instanceBuffer.insert(instanceBuffer.end(), itDrawQueueThread.second.begin(), itDrawQueueThread.second.end());
									}
								}
							}
						}
					}
				}
			}

			m_materialMeshDrawQueueThreads[indexThread].clear();
		}
	}
}

void CRenderQueue::CmdDraw(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, const CGfxDescriptorSetPtr ptrDescriptorSetPass, const uint32_t matPassName, const glm::vec4& scissor, const glm::vec4& viewport, uint32_t mask, bool bIsTransparency)
{
	if (m_materialMeshDrawQueue.empty()) {
		return;
	}

	if (m_pCamera) {
		eastl::vector<CTaskSort> sortTasks;
		{
			CTaskSort::order = bIsTransparency ? SortOrder::BackToFront : SortOrder::FrontToBack;
			CTaskSort::cameraPosition = m_pCamera->GetPosition();

			for (auto& itMaterialQueue : m_materialMeshDrawQueue) {
				sortTasks.emplace_back(itMaterialQueue.second);
			}

			for (int indexTask = 0; indexTask < sortTasks.size(); indexTask++) {
				taskGraph.Task(&sortTasks[indexTask], this, nullptr);
			}

			taskGraph.Dispatch();
			taskGraph.Wait();
		}
	}

	m_materialMeshDrawInstanceRangeQueue.clear();
	{
		int offset = 0;
		eastl::vector<int> instances;

		auto& materialQueue = m_materialMeshDrawInstanceRangeQueue;
		{
			for (const auto& itMaterialQueue : m_materialMeshDrawQueue) {
				auto& meshQueue = materialQueue[itMaterialQueue.first];
				{
					for (const auto& itMeshQueue : itMaterialQueue.second) {
						auto& drawQueue = meshQueue[itMeshQueue.first];
						{
							for (const auto& itDrawQueue : itMeshQueue.second) {
								auto& instanceRange = drawQueue[itDrawQueue.first];
								{
									instanceRange.Set(offset, itDrawQueue.second.size());
								}
								offset += itDrawQueue.second.size();
								instances.insert(instances.end(), itDrawQueue.second.begin(), itDrawQueue.second.end());
							}
						}
					}
				}
			}
		}

		if (m_materialMeshDrawInstanceRangeQueue.empty()) {
			return;
		}

		if (instances.empty()) {
			return;
		}

		m_ptrInstanceBuffer = RenderSystem()->GetInstanceBuffer(INSTANCE_FORMAT, INSTANCE_BINDING);
		m_ptrInstanceBuffer->GetBuffer(GfxRenderer()->GetSwapChain()->GetFrameIndex())->BufferData(instances.size() * sizeof(int), (const uint8_t*)instances.data());
	}

	m_pipelineMaterialQueue.clear();
	{
		for (const auto& itMaterialQueue : m_materialMeshDrawQueue) {
			if (CGfxMaterialPass* pPass = (CGfxMaterialPass*)itMaterialQueue.first->GetPass(matPassName)) {
				if (CGfxPipelineGraphics* pPipeline = (CGfxPipelineGraphics*)pPass->GetPipeline()) {
					if (pPipeline->IsTransparency() == bIsTransparency) {
						m_pipelineMaterialQueue[pPipeline].emplace(itMaterialQueue.first);
					}
				}
			}
		}

		if (m_pipelineMaterialQueue.empty()) {
			return;
		}
	}

	eastl::vector<CTaskCommandBuffer> cmdTasks;
	{
		for (auto& itPipelineQueue : m_pipelineMaterialQueue) {
			CGfxDescriptorSetPtr ptrDescriptorSetInputAttachment = GfxRenderer()->NewDescriptorSet(itPipelineQueue.first, ptrCommandBuffer->GetFrameBuffer(), ptrCommandBuffer->GetRenderPass(), ptrCommandBuffer->GetSubpassIndex());
			cmdTasks.emplace_back(ptrDescriptorSetPass, ptrDescriptorSetInputAttachment, ptrCommandBuffer->GetFrameBuffer(), ptrCommandBuffer->GetRenderPass(), ptrCommandBuffer->GetSubpassIndex(), itPipelineQueue.first, matPassName, scissor, viewport, mask);
		}

		for (int indexTask = 0; indexTask < cmdTasks.size(); indexTask++) {
			taskGraph.Task(&cmdTasks[indexTask], this, nullptr);
		}

		taskGraph.Dispatch();
		taskGraph.Wait();

		for (int indexTask = 0; indexTask < cmdTasks.size(); indexTask++) {
			ptrCommandBuffer->CmdExecute(cmdTasks[indexTask].GetCommandBuffer());
		}
	}
}

void CRenderQueue::CmdDrawThread(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, const int indexSubpass, const CGfxDescriptorSetPtr ptrDescriptorSetPass, const CGfxDescriptorSetPtr ptrDescriptorSetInputAttachment, const CGfxPipelineGraphics* pPipeline, const uint32_t matPassName, const glm::vec4& scissor, const glm::vec4& viewport, uint32_t mask)
{
	GfxRenderer()->BeginRecord(ptrCommandBuffer, ptrFrameBuffer, ptrRenderPass, indexSubpass);
	{
		GfxRenderer()->CmdSetScissor(ptrCommandBuffer, (int)scissor.x, (int)scissor.y, (int)scissor.z, (int)scissor.w);
		GfxRenderer()->CmdSetViewport(ptrCommandBuffer, (int)viewport.x, (int)viewport.y, (int)viewport.z, (int)viewport.w);

		GfxRenderer()->CmdBindPipelineGraphics(ptrCommandBuffer, pPipeline);
		{
			GfxRenderer()->CmdBindDescriptorSet(ptrCommandBuffer, ptrDescriptorSetPass);
			GfxRenderer()->CmdBindDescriptorSet(ptrCommandBuffer, ptrDescriptorSetInputAttachment);
			{
				for (const auto& itMaterialQueue : m_pipelineMaterialQueue[pPipeline]) {
					GfxRenderer()->CmdBindDescriptorSet(ptrCommandBuffer, itMaterialQueue->GetPass(matPassName)->GetDescriptorSet());
					{
						for (const auto& itMeshQueue : m_materialMeshDrawInstanceRangeQueue[itMaterialQueue]) {
							GfxRenderer()->CmdBindIndexBuffer(ptrCommandBuffer, itMeshQueue.first->GetIndexBufferPtr());
							GfxRenderer()->CmdBindVertexBuffer(ptrCommandBuffer, itMeshQueue.first->GetVertexBufferPtr());
							{
								for (const auto& itDrawQueue : itMeshQueue.second) {
									if (itDrawQueue.first->GetMask() & mask) {
										itDrawQueue.first->OnRenderCallback(ptrCommandBuffer);

										GfxRenderer()->CmdBindDescriptorSet(ptrCommandBuffer, GfxRenderer()->GetDescriptorSet(itDrawQueue.first->GetName()));
										{
											GfxRenderer()->CmdBindInstanceBuffer(ptrCommandBuffer, m_ptrInstanceBuffer->GetBuffer(GfxRenderer()->GetSwapChain()->GetFrameIndex()), itDrawQueue.second.offset * sizeof(int));
											GfxRenderer()->CmdDrawInstance(ptrCommandBuffer, itDrawQueue.first->GetIndexType(), itDrawQueue.first->GetIndexOffset(), itDrawQueue.first->GetIndexCount(), itDrawQueue.second.count);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	GfxRenderer()->EndRecord(ptrCommandBuffer);
}
