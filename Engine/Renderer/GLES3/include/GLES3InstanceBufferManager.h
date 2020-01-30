#pragma once
#include "GLES3Renderer.h"


class CGLES3InstanceBufferManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3MultiInstanceBuffer;


private:
	CGLES3InstanceBufferManager(void);
	virtual ~CGLES3InstanceBufferManager(void);


private:
	CGLES3MultiInstanceBuffer* Create(uint32_t instanceFormat, int instanceBinding = 1, int count = 3);
	void Destroy(CGLES3MultiInstanceBuffer* pInstanceBuffer);


private:
	eastl::unordered_map<CGLES3MultiInstanceBuffer*, CGLES3MultiInstanceBuffer*> m_pInstanceBuffers;
};
