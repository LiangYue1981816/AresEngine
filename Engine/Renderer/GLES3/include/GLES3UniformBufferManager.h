#pragma once
#include "GLES3Renderer.h"


class CGLES3UniformBufferManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3UniformBuffer;


private:
	CGLES3UniformBufferManager(void);
	virtual ~CGLES3UniformBufferManager(void);


private:
	CGLES3UniformBuffer* Create(size_t size);
	void Destroy(CGLES3UniformBuffer *pUniformBuffer);


private:
	eastl::unordered_map<CGLES3UniformBuffer*, CGLES3UniformBuffer*> m_pUniformBuffers;
};
