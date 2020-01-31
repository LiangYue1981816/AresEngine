#pragma once
#include "VKRenderer.h"


class CVKInstanceBufferManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKInstanceBuffer;
	friend class CVKMultiInstanceBuffer;


private:
	CVKInstanceBufferManager(CVKDevice* pDevice);
	virtual ~CVKInstanceBufferManager(void);


private:
	CVKInstanceBuffer* Create(uint32_t instanceFormat, int instanceBinding);
	CVKMultiInstanceBuffer* Create(uint32_t instanceFormat, int instanceBinding, int count);
	void Destroy(CVKInstanceBuffer* pInstanceBuffer);
	void Destroy(CVKMultiInstanceBuffer* pMultiInstanceBuffer);


private:
	eastl::unordered_map<CVKInstanceBuffer*, CVKInstanceBuffer*> m_pInstanceBuffers;
	eastl::unordered_map<CVKMultiInstanceBuffer*, CVKMultiInstanceBuffer*> m_pMultiInstanceBuffers;

private:
	CVKDevice* m_pDevice;
};
