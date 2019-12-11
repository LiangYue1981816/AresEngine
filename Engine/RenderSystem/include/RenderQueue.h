#pragma once
#include "PreHeader.h"


class CALL_API CTaskGraph;
class CALL_API CRenderQueue
{
public:
	CRenderQueue(void);
	virtual ~CRenderQueue(void);


public:
	void Clear(void);
	void Begin(void);
	void Add(const CGfxMaterialPtr ptrMaterial, const CGfxMeshDrawPtr ptrMeshDraw, int indexInstance, int indexThread = 0);
	void End(void);

public:
	virtual void CmdDraw(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer, const CGfxDescriptorSetPtr ptrDescriptorSetPass, const uint32_t matPassName, const glm::vec4& scissor, const glm::vec4& viewport, uint32_t mask, bool bIsTransparency);
	virtual void CmdDrawThread(CGfxCommandBufferPtr ptrCommandBuffer, const CGfxFrameBufferPtr ptrFrameBuffer, const CGfxRenderPassPtr ptrRenderPass, const int indexSubpass, const CGfxDescriptorSetPtr ptrDescriptorSetPass, const CGfxDescriptorSetPtr ptrDescriptorSetInputAttachment, const CGfxPipelineGraphics* pPipeline, const uint32_t matPassName, const glm::vec4& scissor, const glm::vec4& viewport, uint32_t mask);


private:
	eastl::unordered_map<CGfxMaterialPtr, eastl::unordered_map<CGfxMeshPtr, eastl::unordered_map<CGfxMeshDrawPtr, eastl::vector<int>>>> m_materialMeshDrawQueue;
	eastl::unordered_map<CGfxMaterialPtr, eastl::unordered_map<CGfxMeshPtr, eastl::unordered_map<CGfxMeshDrawPtr, eastl::vector<int>>>> m_materialMeshDrawQueueThreads[MAX_THREAD_COUNT];
	eastl::unordered_map<const CGfxPipelineGraphics*, eastl::unordered_set<CGfxMaterialPtr>> m_pipelineMaterialQueue;
};
