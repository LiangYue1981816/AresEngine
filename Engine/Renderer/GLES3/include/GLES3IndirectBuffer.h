#pragma once
#include "GLES3Renderer.h"


class CGLES3IndirectBuffer : public CGfxIndirectBuffer
{
	friend class CGLES3MeshDraw;


private:
	CGLES3IndirectBuffer(int numDrawCommands);
	virtual ~CGLES3IndirectBuffer(void);
	virtual void Release(void);


public:
	uint32_t GetDrawCommandCount(void) const;
	uint32_t GetDrawCommandOffset(int indexDraw) const;
	uint32_t GetSize(void) const;
	uint32_t GetStride(void) const;

public:
	bool BufferData(int indexDraw, int firstIndex, int baseVertex, int baseInstance, int indexCount, int instanceCount);

public:
	void Bind(void) const;


private:
	eastl::vector<DrawCommand> m_draws;

private:
	CGLES3Buffer* m_pBuffer;
};

class CGLES3MultiIndirectBuffer : public CGfxMultiIndirectBuffer
{
	friend class CGLES3MeshDraw;


private:
	CGLES3MultiIndirectBuffer(int numDrawCommands, int count);
	virtual ~CGLES3MultiIndirectBuffer(void);
	virtual void Release(void);


public:
	void SetBufferIndex(int index);

public:
	uint32_t GetDrawCommandCount(void) const;
	uint32_t GetDrawCommandOffset(int indexDraw) const;
	uint32_t GetSize(void) const;
	uint32_t GetStride(void) const;

public:
	bool BufferData(int indexDraw, int firstIndex, int baseVertex, int baseInstance, int indexCount, int instanceCount);

public:
	void Bind(void) const;


private:
	int m_index;
	eastl::vector<CGLES3IndirectBuffer*> m_pBuffers;
};
