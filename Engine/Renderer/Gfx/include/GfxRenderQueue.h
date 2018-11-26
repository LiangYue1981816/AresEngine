#pragma once
#include "GfxRenderer.h"


class CALL_API CTaskCommandBuffer : public CTask
{
public:
	CTaskCommandBuffer(int indexQueue, const CGfxUniformBufferPtr &ptrUniformBufferEngine, const CGfxUniformBufferPtr &ptrUniformBufferCamera, CGfxPipelineGraphics *pPipeline, uint32_t namePass);
	virtual ~CTaskCommandBuffer(void);


public:
	const CGfxCommandBufferPtr& GetCommandBuffer(void) const;
	void TaskFunc(void *pParams);


private:
	int m_indexQueue;
	CGfxCommandBufferPtr m_ptrCommandBuffer;
	CGfxUniformBufferPtr m_ptrUniformBufferEngine;
	CGfxUniformBufferPtr m_ptrUniformBufferCamera;
	CGfxPipelineGraphics*m_pPipeline;
	uint32_t m_namePass;
};

class CALL_API CGfxRenderQueue
{
public:
	CGfxRenderQueue(void);
	virtual ~CGfxRenderQueue(void);


public:
	void Clear(int indexQueue);
	void Begin(int indexQueue);
	void Add(int indexThread, int indexQueue, const CGfxMaterialPtr &ptrMaterial, const CGfxMeshPtr &ptrMesh, int indexDraw, const uint8_t *pInstanceData, uint32_t size);
	void End(int indexQueue);

public:
	virtual void CmdDraw(int indexQueue, CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxUniformBufferPtr &ptrUniformBufferEngine, const CGfxUniformBufferPtr &ptrUniformBufferCamera, uint32_t namePass);
	virtual void CmdDrawThread(int indexQueue, CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxUniformBufferPtr &ptrUniformBufferEngine, const CGfxUniformBufferPtr &ptrUniformBufferCamera, CGfxPipelineGraphics *pPipeline, uint32_t namePass);


private:
	eastl::vector<CTaskCommandBuffer> m_tasks[2];
	eastl::unordered_map<CGfxPipelineGraphics*, eastl::unordered_map<CGfxMaterialPtr, CGfxMaterialPtr>> m_pipelineMeshQueue[2];
	eastl::unordered_map<CGfxMaterialPtr, eastl::unordered_map<CGfxMeshPtr, eastl::unordered_map<int, eastl::vector<uint8_t>>>> m_materialMeshQueue[2];
	eastl::unordered_map<CGfxMaterialPtr, eastl::unordered_map<CGfxMeshPtr, eastl::unordered_map<int, eastl::vector<uint8_t>>>> m_materialMeshQueueThreads[THREAD_COUNT][2];

private:
	CTaskGraph m_taskGraph;
};
