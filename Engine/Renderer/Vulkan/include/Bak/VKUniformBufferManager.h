#pragma once
#include "VKRenderer.h"


class CVKUniformBufferManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKUniformBuffer;


private:
	CVKUniformBufferManager(CVKDevice *pDevice);
	virtual ~CVKUniformBufferManager(void);


private:
	CVKUniformBuffer* Create(size_t size);
	void Destroy(CVKUniformBuffer *pUniformBuffer);


private:
	eastl::unordered_map<CVKUniformBuffer*, CVKUniformBuffer*> m_pUniformBuffers;

private:
	CVKDevice *m_pDevice;
};
