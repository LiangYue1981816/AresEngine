#pragma once
#include "GLES3Renderer.h"


class CGLES3StorageBufferManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3StorageBuffer;


private:
	CGLES3StorageBufferManager(void);
	virtual ~CGLES3StorageBufferManager(void);


private:
	CGLES3StorageBuffer* Create(size_t size);
	void Destroy(CGLES3StorageBuffer* pStorageBuffer);


private:
	eastl::unordered_map<CGLES3StorageBuffer*, CGLES3StorageBuffer*> m_pStorageBuffers;
};
