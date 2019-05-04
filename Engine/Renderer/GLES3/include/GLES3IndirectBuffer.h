#pragma once
#include "GLES3Renderer.h"


class CGLES3IndirectBuffer : public CGfxIndirectBuffer
{
	friend class CGLES3MeshDraw;


private:
	CGLES3IndirectBuffer(uint32_t numDrawCommands);
	virtual ~CGLES3IndirectBuffer(void);


public:
	uint32_t GetSize(void) const;
	uint32_t GetDrawCommandCount(void) const;
	uint32_t GetDrawCommandOffset(int indexDraw) const;

public:
	bool BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount);

public:
	void Bind(void) const;


private:
	eastl::vector<DrawCommand> m_draws;

private:
	CGLES3BufferPtr m_ptrBuffer;
};
