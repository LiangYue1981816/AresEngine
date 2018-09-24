#pragma once
#include "GfxRenderer.h"


class CGfxUniformBuffer
{
public:
	CGfxUniformBuffer(size_t size, bool bDynamic);
	virtual ~CGfxUniformBuffer(void);


public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);

public:
	GLuint GetBuffer(void) const;
	GLsizeiptr GetSize(void) const;


private:
	GLuint m_buffer;
	GLsizeiptr m_size;
};
