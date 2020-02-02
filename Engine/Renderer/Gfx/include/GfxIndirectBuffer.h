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

class CALL_API CGfxIndirectBuffer : public CGfxResource
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
	virtual bool BufferData(int indexDraw, int firstIndex, int baseVertex, int baseInstance, int indexCount, int instanceCount) = 0;
};

class CALL_API CGfxMultiIndirectBuffer : public CGfxResource
{
public:
	CGfxMultiIndirectBuffer(int numDrawCommands, int count)
	{

	}
	virtual ~CGfxMultiIndirectBuffer(void)
	{

	}


public:
	virtual CGfxIndirectBuffer* GetBuffer(int index) const = 0;
};
