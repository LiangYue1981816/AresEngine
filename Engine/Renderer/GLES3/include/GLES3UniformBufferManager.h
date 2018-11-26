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
	CGLES3UniformBuffer* CreateUniformBuffer(size_t size, bool bDynamic);
	void DestroyUniformBuffer(CGLES3UniformBuffer *pUniformBuffer);


private:
	eastl::unordered_map<CGLES3UniformBuffer*, CGLES3UniformBuffer*> m_pUniformBuffers;
};
