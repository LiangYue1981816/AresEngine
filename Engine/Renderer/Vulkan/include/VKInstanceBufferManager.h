#pragma once
#include "VKRenderer.h"


class CVKInstanceBufferManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKInstanceBuffer;


private:
	CVKInstanceBufferManager(CVKDevice* pDevice);
	virtual ~CVKInstanceBufferManager(void);


private:
	CVKInstanceBuffer* Create(uint32_t instanceFormat, int instanceBinding);
	void Destroy(CVKInstanceBuffer* pInstanceBuffer);


private:
	eastl::unordered_map<CVKInstanceBuffer*, CVKInstanceBuffer*> m_pInstanceBuffers;

private:
	CVKDevice* m_pDevice;
};
