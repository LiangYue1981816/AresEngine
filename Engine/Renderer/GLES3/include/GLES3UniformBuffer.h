#pragma once
#include "GLES3Renderer.h"


class CGLES3UniformBuffer : public CGfxUniformBuffer
{
	friend class CGLES3UniformBufferManager;


private:
	CGLES3UniformBuffer(CGLES3UniformBufferManager* pManager, size_t size);
	virtual ~CGLES3UniformBuffer(void);
	virtual void Release(void);


public:
	uint32_t GetSize(void) const;
	uint32_t GetRange(void) const;
	uint32_t GetOffset(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void* data);
	bool BufferRange(size_t offset, size_t range);

public:
	void Bind(int binding) const;


private:
	CGLES3Buffer* m_pBuffer;

private:
	CGLES3UniformBufferManager* m_pManager;
};
