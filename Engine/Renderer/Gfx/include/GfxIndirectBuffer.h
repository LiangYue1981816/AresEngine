#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxIndirectBuffer
{
public:
	typedef struct DrawCommand {
		int indexCount = 0;
		int instanceCount = 0;
		int firstIndex = 0;
		int baseVertex = 0;
		int reservedMustBeZero = 0;
	} DrawCommand;


public:
	CGfxIndirectBuffer(uint32_t numDrawCommands)
	{

	}
	virtual ~CGfxIndirectBuffer(void)
	{

	}


public:
	virtual uint32_t GetDrawCommandCount(void) const = 0;
	virtual uint32_t GetDrawCommandOffset(int indexDraw) const = 0;
	virtual uint32_t GetSize(void) const = 0;

public:
	virtual bool BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount) = 0;
};
