#pragma once
#include "GLES3Renderer.h"


class CGLES3DrawIndirectBuffer : public CGfxDrawIndirectBuffer
{
	friend class CGLES3Mesh;


private:
	CGLES3DrawIndirectBuffer(size_t size);
	virtual ~CGLES3DrawIndirectBuffer(void);


public:
	bool BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount);
	void Bind(void *pParam);


private:
	uint32_t m_buffer;
};
