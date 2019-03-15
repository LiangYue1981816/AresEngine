#pragma once
#include "GLES3Renderer.h"


class CGLES3IndirectBuffer : public CGLES3Buffer, public CGfxIndirectBuffer
{
	friend class CGLES3MeshDraw;
	friend class CGLES3VertexArrayObject;


private:
	CGLES3IndirectBuffer(uint32_t numDrawCommands);
	virtual ~CGLES3IndirectBuffer(void);


public:
	uint32_t GetDrawCommandCount(void) const;
	uint32_t GetDrawCommandOffset(int indexDraw) const;
	uint32_t GetSize(void) const;

public:
	bool BufferData(int indexDraw, int instanceCount);
	bool BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount);

private:
	void Bind(void) const;


private:
	eastl::vector<DrawCommand> m_draws;
};
