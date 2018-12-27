#pragma once
#include "GLES3Renderer.h"


class CGLES3IndirectBuffer : public CGfxIndirectBuffer
{
	friend class CGLES3Mesh;
	friend class CGLES3MeshDraw;


private:
	CGLES3IndirectBuffer(uint32_t count);
	virtual ~CGLES3IndirectBuffer(void);


public:
	bool BufferData(int indexDraw, int instanceCount);
	bool BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount);
	void Bind(void *pParam);


private:
	uint32_t m_buffer;
	eastl::vector<DrawCommand> m_draws;
};
