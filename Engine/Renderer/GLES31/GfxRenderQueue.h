#pragma once
#include "GfxRenderer.h"


class CGfxRenderQueue
{
	friend class CGfxRenderer;


public:
	CGfxRenderQueue(void);
	virtual ~CGfxRenderQueue(void);


public:
	void AddQueue(int indexThread, int indexQueue, const CGfxMaterialPtr &ptrMaterial, const CGfxMeshPtr &ptrMesh, const glm::mat4 &mtxTransform);
	void ClearQueue(int indexQueue);
	void ClearQueueAll(void);

public:
	void CmdDraw(CGfxCamera *pCamera, int indexThread, int indexQueue, uint32_t namePass);
	void CmdExecute(CGfxCommandBuffer *pMainCommandBuffer, int indexQueue);


private:
	eastl::vector<CGfxCommandBuffer> m_secondaryCommandBuffer[THREAD_COUNT][2];
	eastl::unordered_map<CGfxMaterialPtr, eastl::unordered_map<CGfxMeshPtr, eastl::vector<glm::mat4>>> m_materialQueue[THREAD_COUNT][2];
};
