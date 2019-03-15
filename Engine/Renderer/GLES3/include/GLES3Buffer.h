#pragma once
#include "GLES3Renderer.h"


class CGLES3Buffer
{
private:
	CGLES3Buffer(uint32_t target, size_t size, bool bDynamic);
	virtual ~CGLES3Buffer(void);


private:
	uint32_t GetSize(void) const;

private:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);

private:
	void Bind(void) const;
	void Bind(int index, int offset, int size) const;


private:
	uint32_t m_size;
	uint32_t m_target;
	uint32_t m_buffer;
};

typedef CGfxResourcePtr<CGLES3Buffer> CGLES3BufferPtr;
