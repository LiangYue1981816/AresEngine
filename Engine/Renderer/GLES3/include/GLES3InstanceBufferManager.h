#pragma once
#include "GLES3Renderer.h"


class CGLES3InstanceBufferManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3InstanceBuffer;
	friend class CGLES3MultiInstanceBuffer;


private:
	CGLES3InstanceBufferManager(void);
	virtual ~CGLES3InstanceBufferManager(void);


private:
	CGLES3InstanceBuffer* Create(uint32_t instanceFormat, int instanceBinding);
	CGLES3MultiInstanceBuffer* Create(uint32_t instanceFormat, int instanceBinding, int count);
	void Destroy(CGLES3InstanceBuffer* pInstanceBuffer);
	void Destroy(CGLES3MultiInstanceBuffer* pMultiInstanceBuffer);


private:
	eastl::unordered_map<CGLES3InstanceBuffer*, CGLES3InstanceBuffer*> m_pInstanceBuffers;
	eastl::unordered_map<CGLES3MultiInstanceBuffer*, CGLES3MultiInstanceBuffer*> m_pMultiInstanceBuffers;
};
