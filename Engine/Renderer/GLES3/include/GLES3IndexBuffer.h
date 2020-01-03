#pragma once
#include "GLES3Renderer.h"


class CGLES3IndexBuffer : public CGfxIndexBuffer
{
	friend class CGLES3Mesh;
	friend class CGLES3MultiIndexBuffer;


private:
	CGLES3IndexBuffer(GfxIndexType type, size_t size, bool bDynamic);
	virtual ~CGLES3IndexBuffer(void);
	virtual void Release(void);


public:
	GfxIndexType GetIndexType(void) const;
	uint32_t GetIndexCount(void) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void* data);

public:
	void Bind(void) const;


private:
	GfxIndexType m_type;

private:
	CGLES3Buffer* m_pBuffer;
};

class CGLES3MultiIndexBuffer : public CGfxMultiIndexBuffer
{
	friend class CGLES3Mesh;


private:
	CGLES3MultiIndexBuffer(GfxIndexType type, size_t size, bool bDynamic, int count);
	virtual ~CGLES3MultiIndexBuffer(void);
	virtual void Release(void);


public:
	void SetBufferIndex(int index);

public:
	GfxIndexType GetIndexType(void) const;
	uint32_t GetIndexCount(void) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t offset, size_t size, const void* data);

public:
	void Bind(void) const;


private:
	int m_index;
	eastl::vector<CGLES3IndexBuffer*> m_pBuffers;
};
