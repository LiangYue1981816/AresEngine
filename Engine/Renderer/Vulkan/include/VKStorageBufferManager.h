#pragma once
#include "VKRenderer.h"


class CVKStorageBufferManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKStorageBuffer;


private:
	CVKStorageBufferManager(CVKDevice* pDevice);
	virtual ~CVKStorageBufferManager(void);


private:
	CVKStorageBuffer* Create(size_t size);
	void Destroy(CVKStorageBuffer* pStorageBuffer);


private:
	eastl::unordered_map<CVKStorageBuffer*, CVKStorageBuffer*> m_pStorageBuffers;

private:
	CVKDevice* m_pDevice;
};
