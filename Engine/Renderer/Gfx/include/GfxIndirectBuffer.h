#pragma once
#include "PreHeader.h"


typedef struct DrawCommand {
	int indexCount = 0;
	int instanceCount = 0;
	int firstIndex = 0;
	int baseVertex = 0;
	int baseInstance = 0;
} DrawCommand;

class CALL_API CGfxIndirectBuffer
{
public:
	CGfxIndirectBuffer(int numDrawCommands)
	{

	}
	virtual ~CGfxIndirectBuffer(void)
	{

	}


public:
	virtual uint32_t GetDrawCommandCount(void) const = 0;
	virtual uint32_t GetDrawCommandOffset(int indexDraw) const = 0;
	virtual uint32_t GetSize(void) const = 0;
	virtual uint32_t GetStride(void) const = 0;

public:
	virtual bool BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount) = 0;
};
