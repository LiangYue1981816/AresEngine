#pragma once
#include "GLES3Renderer.h"


class CGLES3InstanceBuffer : public CGfxInstanceBuffer
{
public:
	CGLES3InstanceBuffer(uint32_t instanceFormat, int instanceBinding);
	virtual ~CGLES3InstanceBuffer(void);
	virtual void Release(void);


public:
	uint32_t GetInstanceBinding(void) const;
	uint32_t GetInstanceFormat(void) const;
	uint32_t GetInstanceCount(void) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t size, const void* data);

public:
	void Bind(void) const;


private:
	uint32_t m_binding;
	uint32_t m_format;
	uint32_t m_count;

private:
	CGLES3Buffer* m_pBuffer;
};


class CGLES3MultiInstanceBuffer : public CGfxMultiInstanceBuffer
{
public:
	CGLES3MultiInstanceBuffer(uint32_t instanceFormat, int instanceBinding, int count);
	virtual ~CGLES3MultiInstanceBuffer(void);
	virtual void Release(void);


public:
	void SetBufferIndex(int index);

public:
	CGLES3InstanceBuffer* GetBuffer(void) const;

public:
	uint32_t GetInstanceBinding(void) const;
	uint32_t GetInstanceFormat(void) const;
	uint32_t GetInstanceCount(void) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t size, const void* data);

public:
	void Bind(void) const;


private:
	int m_index;
	eastl::vector<CGLES3InstanceBuffer*> m_pBuffers;
};
