#pragma once
#include "GLES3Renderer.h"


class CGLES3InstanceBuffer : public CGfxInstanceBuffer
{
	friend class CGLES3MultiInstanceBuffer;
	friend class CGLES3InstanceBufferManager;


private:
	CGLES3InstanceBuffer(CGLES3InstanceBufferManager* pManager, uint32_t instanceFormat, int instanceBinding);
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

private:
	CGLES3InstanceBufferManager* m_pManager;
};


class CGLES3MultiInstanceBuffer : public CGfxMultiInstanceBuffer
{
	friend class CGLES3InstanceBufferManager;


private:
	CGLES3MultiInstanceBuffer(CGLES3InstanceBufferManager* pManager, uint32_t instanceFormat, int instanceBinding, int count);
	virtual ~CGLES3MultiInstanceBuffer(void);
	virtual void Release(void);


public:
	CGfxInstanceBuffer* GetBuffer(int index) const;


private:
	eastl::vector<CGLES3InstanceBuffer*> m_pBuffers;

private:
	CGLES3InstanceBufferManager* m_pManager;
};
