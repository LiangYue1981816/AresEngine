#pragma once
#include "GLES3Renderer.h"


class CGLES3VertexBuffer : public CGfxVertexBuffer
{
public:
	CGLES3VertexBuffer(uint32_t vertexFormat, int vertexBinding, size_t size, bool bDynamic);
	virtual ~CGLES3VertexBuffer(void);
	virtual void Release(void);


public:
	uint32_t GetVertexBinding(void) const;
	uint32_t GetVertexFormat(void) const;
	uint32_t GetVertexCount(void) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void* data);

public:
	void Bind(void) const;


private:
	uint32_t m_binding;
	uint32_t m_format;
	uint32_t m_count;

private:
	CGLES3Buffer* m_pBuffer;
};


class CGLES3MultiVertexBuffer : public CGfxMultiVertexBuffer
{
public:
	CGLES3MultiVertexBuffer(uint32_t vertexFormat, int vertexBinding, size_t size, bool bDynamic, int count);
	virtual ~CGLES3MultiVertexBuffer(void);
	virtual void Release(void);


public:
	void SetBufferIndex(int index);

public:
	CGLES3VertexBuffer* GetBuffer(void) const;

public:
	uint32_t GetVertexBinding(void) const;
	uint32_t GetVertexFormat(void) const;
	uint32_t GetVertexCount(void) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void* data);

public:
	void Bind(void) const;


private:
	int m_index;
	eastl::vector<CGLES3VertexBuffer*> m_pBuffers;
};
