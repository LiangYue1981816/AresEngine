#pragma once
#include "GLES3Renderer.h"


class CGLES3InstanceBufferManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3InstanceBuffer;


private:
	CGLES3InstanceBufferManager(void);
	virtual ~CGLES3InstanceBufferManager(void);


private:
	CGLES3InstanceBuffer* Create(uint32_t instanceFormat, int instanceBinding);
	void Destroy(CGLES3InstanceBuffer* pInstanceBuffer);


private:
	eastl::unordered_map<CGLES3InstanceBuffer*, CGLES3InstanceBuffer*> m_pInstanceBuffers;
};
