#pragma once
#include "VKRenderer.h"


class CVKInstanceBufferManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKMultiInstanceBuffer;


private:
	CVKInstanceBufferManager(CVKDevice* pDevice);
	virtual ~CVKInstanceBufferManager(void);


private:
	CVKMultiInstanceBuffer* Create(uint32_t instanceFormat, int instanceBinding = 1, int count = 3);
	void Destroy(CVKMultiInstanceBuffer* pInstanceBuffer);


private:
	eastl::unordered_map<CVKMultiInstanceBuffer*, CVKMultiInstanceBuffer*> m_pInstanceBuffers;

private:
	CVKDevice* m_pDevice;
};
