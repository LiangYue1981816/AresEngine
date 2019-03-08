#pragma once
#include "GLES3Renderer.h"


class CGLES3UniformBuffer : public CGLES3Buffer, public CGfxUniformBuffer
{
	friend class CGLES3Pipeline;
	friend class CGLES3UniformBufferManager;


private:
	CGLES3UniformBuffer(CGLES3UniformBufferManager *pManager, size_t size);
	virtual ~CGLES3UniformBuffer(void);
	virtual void Release(void);


public:
	HANDLE GetBuffer(void) const;

public:
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);

private:
	void Bind(int index, int offset, int size) const;


private:
	CGLES3UniformBufferManager *m_pManager;
};
