#pragma once
#include "GLES3Renderer.h"


class CGLES3DrawIndirectBuffer : public CGfxDrawIndirectBuffer
{
	friend class CGLES3Mesh;
	friend class CGLES3MeshDraw;


private:
	CGLES3DrawIndirectBuffer(uint32_t count);
	virtual ~CGLES3DrawIndirectBuffer(void);


public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);
	bool BufferData(int indexDraw, int instanceCount);
	bool BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount);
	void Bind(void *pParam);


private:
	uint32_t m_buffer;
	eastl::vector<DrawCommand> m_draws;
};
