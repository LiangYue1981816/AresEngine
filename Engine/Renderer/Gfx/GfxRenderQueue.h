#pragma once
#include "GfxRenderer.h"


class CGfxRenderQueue
{
public:
	CGfxRenderQueue(void);
	virtual ~CGfxRenderQueue(void);


public:
	void AddMesh(int indexThread, int indexQueue, const CGfxMaterialPtr &ptrMaterial, const CGfxMeshPtr &ptrMesh, const uint8_t *pInstanceData, uint32_t size);
	void Clear(int indexQueue);

public:
	void CmdDraw(CGfxCamera *pCamera, CGfxCommandBufferPtr &ptrCommandBuffer, int indexThread, int indexQueue, uint32_t namePass);


private:
	eastl::unordered_map<CGfxMaterialPtr, eastl::unordered_map<CGfxMeshPtr, eastl::vector<uint8_t>>> m_materialMeshQueue[THREAD_COUNT][2];
};
