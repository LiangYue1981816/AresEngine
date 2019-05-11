#pragma once
#include "GLES3Renderer.h"


class CGLES3Buffer : public CGfxResource
{
	friend class CGLES3IndexBuffer;
	friend class CGLES3VertexBuffer;
	friend class CGLES3InstanceBuffer;
	friend class CGLES3IndirectBuffer;
	friend class CGLES3UniformBuffer;


private:
	CGLES3Buffer(uint32_t target, size_t size, bool bDynamic);
	virtual ~CGLES3Buffer(void);
	virtual void Release(void);


private:
	bool Create(uint32_t target, size_t size, bool bDynamic);
	void Destroy(void);

private:
	uint32_t GetTarget(void) const;
	uint32_t GetBuffer(void) const;
	uint32_t GetSize(void) const;

private:
	bool BufferSize(size_t size, bool bDynamic);
	bool BufferData(size_t offset, size_t size, const void* data, bool bSync = true);

private:
	void Bind(void) const;
	void Bind(int binding, int offset, int size) const;


private:
	uint32_t m_target;
	uint32_t m_buffer;
	uint32_t m_size;
};
