#pragma once
#include "GfxRenderer.h"


class CGfxRenderQueue
{
	friend class CGfxRenderer;


public:
	CGfxRenderQueue(void);
	virtual ~CGfxRenderQueue(void);


public:
	void AddMesh(int indexThread, int indexQueue, const CGfxMaterialPtr &ptrMaterial, const CGfxMeshPtr &ptrMesh, const glm::mat4 &mtxTransform);
	void Clear(int indexQueue);

public:
	void CmdDraw(CGfxCamera *pCamera, CGfxCommandBuffer *pCommandBuffer, int indexThread, int indexQueue, uint32_t namePass);


private:
	eastl::unordered_map<CGfxMaterialPtr, eastl::unordered_map<CGfxMeshPtr, eastl::vector<glm::mat4>>> m_materialMeshQueue[THREAD_COUNT][2];
};
