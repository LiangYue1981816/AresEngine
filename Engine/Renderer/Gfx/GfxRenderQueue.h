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
	void End(int indexQueue);
	void Add(int indexThread, int indexQueue, const CGfxMaterialPtr &ptrMaterial, const CGfxMeshPtr &ptrMesh, int indexDraw, const uint8_t *pInstanceData, uint32_t size);

public:
	void CmdDraw(int indexQueue, CGfxCommandBufferPtr &ptrCommandBuffer, const CGfxUniformEnginePtr &ptrUniformEngine, const CGfxUniformCameraPtr &ptrUniformCamera, uint32_t namePass);


private:
	eastl::unordered_map<CGfxMaterialPtr, eastl::unordered_map<CGfxMeshPtr, eastl::unordered_map<int, eastl::vector<uint8_t>>>> m_materialMeshQueue[2];
	eastl::unordered_map<CGfxMaterialPtr, eastl::unordered_map<CGfxMeshPtr, eastl::unordered_map<int, eastl::vector<uint8_t>>>> m_materialMeshQueueThreads[THREAD_COUNT][2];
};
