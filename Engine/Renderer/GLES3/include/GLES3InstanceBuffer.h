#pragma once
#include "GLES3Renderer.h"


class CGLES3InstanceBuffer : public CGfxInstanceBuffer
{
	friend class CGLES3MeshDraw;


private:
	CGLES3InstanceBuffer(uint32_t instanceFormat, uint32_t instanceBinding);
	virtual ~CGLES3InstanceBuffer(void);


public:
	uint32_t GetInstanceFormat(void) const;
	uint32_t GetInstanceCount(void) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(size_t size, const void *pBuffer);

public:
	void Bind(void) const;


private:
	uint32_t m_format;
	uint32_t m_count;

private:
	CGLES3BufferPtr m_ptrBuffer;
};
