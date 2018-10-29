#pragma once
#include "GfxRenderer.h"


class CGfxRenderQueue
{
	friend class CGfxRenderer;


public:
	CGfxRenderQueue(void);
	virtual ~CGfxRenderQueue(void);


public:
	void AddMesh(int indexThread, int indexQueue, const CGfxMaterialPtr &ptrMaterial, const CGfxMeshPtr &ptrMesh, const uint8_t *pInstanceData, uint32_t size);
	void AddMeshIndirect(int indexThread, int indexQueue, const CGfxMaterialPtr &ptrMaterial, const CGfxMeshPtr &ptrMesh, const CGfxDrawIndirectBufferPtr &ptrDrawIndirectBuffer, const uint8_t *pInstanceData, uint32_t size);
	void Clear(int indexQueue);

public:
	void CmdDraw(CGfxCamera *pCamera, CGfxCommandBuffer *pCommandBuffer, int indexThread, int indexQueue, uint32_t namePass);


private:
	eastl::unordered_map<CGfxMaterialPtr, eastl::unordered_map<CGfxMeshPtr, eastl::vector<uint8_t>>> m_materialMeshQueue[THREAD_COUNT][2];
	eastl::unordered_map<CGfxMaterialPtr, eastl::unordered_map<CGfxMeshPtr, eastl::unordered_map<CGfxDrawIndirectBufferPtr, eastl::vector<uint8_t>>>> m_materialMeshIndirectQueue[THREAD_COUNT][2];
};
