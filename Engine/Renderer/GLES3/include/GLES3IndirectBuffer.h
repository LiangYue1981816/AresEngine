#pragma once
#include "GLES3Renderer.h"


class CGLES3IndirectBuffer : public CGfxIndirectBuffer
{
	friend class CGLES3MeshDraw;


private:
	CGLES3IndirectBuffer(uint32_t drawCommandCount);
	virtual ~CGLES3IndirectBuffer(void);


public:
	uint32_t GetDrawCommandCount(void) const;
	uint32_t GetDrawCommandOffset(int indexDraw) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(int indexDraw, int instanceCount);
	bool BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount);
	void Bind(void);


private:
	uint32_t m_count;
	uint32_t m_size;

private:
	uint32_t m_buffer;
	eastl::vector<DrawCommand> m_draws;
};
