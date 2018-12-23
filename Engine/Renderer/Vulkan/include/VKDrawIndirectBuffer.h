#pragma once
#include "VKRenderer.h"


class CVKDrawIndirectBuffer : public CVKBufferBase, public CGfxDrawIndirectBuffer
{
	friend class CVKDevice;


private:
	CVKDrawIndirectBuffer(CVKDevice *pDevice, uint32_t count);
	virtual ~CVKDrawIndirectBuffer(void);


public:
	bool BufferData(size_t offset, size_t size, const void *pBuffer);
	bool BufferData(int indexDraw, int instanceCount);
	bool BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount);
	void Bind(void *pParam);


private:
	eastl::vector<DrawCommand> m_draws;
};
