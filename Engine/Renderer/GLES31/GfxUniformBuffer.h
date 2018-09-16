#pragma once
#include "GfxRenderer.h"


class CGfxUniformBuffer
{
public:
	CGfxUniformBuffer(void);
	virtual ~CGfxUniformBuffer(void);


public:
	bool Create(const void *pBuffer, size_t size, bool bDynamic);
	void Destroy(void);
	bool SetData(const void *pBuffer, size_t size, size_t offset = 0);

public:
	GLuint GetBuffer(void) const;
	GLsizeiptr GetSize(void) const;


private:
	GLuint m_buffer;
	GLsizeiptr m_size;
};
