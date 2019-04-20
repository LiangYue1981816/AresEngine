#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxRenderQueue
{
public:
	CGfxRenderQueue(void);
	virtual ~CGfxRenderQueue(void);


public:
	void Clear(int indexQueue);
	void Begin(int indexQueue);
	void Add(int indexQueue, int indexThread, const CGfxMaterialPtr ptrMaterial, const CGfxMeshDrawPtr ptrMeshDraw, const uint8_t* pInstanceData, uint32_t size);
	void End(int indexQueue);

public:
	virtual void CmdDraw(int indexQueue, CGfxCommandBufferPtr ptrCommandBuffer, const CGfxDescriptorSetPtr ptrDescriptorSetEngine, const CGfxDescriptorSetPtr ptrDescriptorSetCamera, uint32_t namePass);
	virtual void CmdDrawThread(int indexQueue, CGfxCommandBufferPtr ptrCommandBuffer, const CGfxDescriptorSetPtr ptrDescriptorSetEngine, const CGfxDescriptorSetPtr ptrDescriptorSetCamera, const CGfxDescriptorSetPtr ptrDescriptorSetInputAttachment, CGfxPipelineGraphics* pPipeline, uint32_t namePass);


private:
	eastl::unordered_map<CGfxPipelineGraphics*, eastl::unordered_map<CGfxMaterialPtr, CGfxMaterialPtr>> m_pipelineMaterialQueue[2];
	eastl::unordered_map<CGfxMaterialPtr, eastl::unordered_map<CGfxMeshDrawPtr, eastl::vector<uint8_t>>> m_materialMeshDrawQueue[2];
	eastl::unordered_map<CGfxMaterialPtr, eastl::unordered_map<CGfxMeshDrawPtr, eastl::vector<uint8_t>>> m_materialMeshDrawQueueThreads[MAX_THREAD_COUNT][2];

private:
	CTaskGraph m_taskGraph;
};
