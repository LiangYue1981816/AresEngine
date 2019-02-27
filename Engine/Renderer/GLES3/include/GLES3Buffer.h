#pragma once
#include "GLES3Renderer.h"


class CGLES3Buffer
{
protected:
	CGLES3Buffer(uint32_t target, size_t size, bool bDynamic);
	virtual ~CGLES3Buffer(void);


protected:
	uint32_t m_size;
	uint32_t m_target;
	uint32_t m_buffer;
};
