#pragma once
#include "GLES3Renderer.h"


class CGLES3UniformBase : public CGfxUniformBase
{
	friend class CGLES3UniformManager;


protected:
	CGLES3UniformBase(CGLES3UniformManager *pManager, size_t size, bool bDynamic);
	virtual ~CGLES3UniformBase(void);


public:
	CGLES3UniformBuffer* GetUniformBuffer(void) const;


protected:
	CGLES3UniformBuffer *m_pUniformBuffer;

protected:
	CGLES3UniformManager *m_pManager;
};
