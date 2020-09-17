#pragma once
#include "PreHeader.h"


typedef struct DrawCommand {
	DrawCommand(void)
	{
		indexCount = 0;
		instanceCount = 0;
		firstIndex = 0;
		baseVertex = 0;
		baseInstance = 0;
	}

	void Set(int _indexCount, int _instanceCount, int _firstIndex, int _baseVertex, int _baseInstance)
	{
		indexCount = _indexCount;
		instanceCount = _instanceCount;
		firstIndex = _firstIndex;
		baseVertex = _baseVertex;
		baseInstance = _baseInstance;
	}

	int indexCount;
	int instanceCount;
	int firstIndex;
	int baseVertex;
	int baseInstance;
} DrawCommand;

class CGfxIndirectBuffer : public CGfxResource
{
public:
	CGfxIndirectBuffer(void)
	{

	}
	virtual ~CGfxIndirectBuffer(void)
	{

	}


public:
	virtual uint32_t GetOffset(int indexDraw) const = 0;
	virtual uint32_t GetCount(void) const = 0;

public:
	virtual bool BufferData(uint32_t count, const DrawCommand* data) = 0;
};
