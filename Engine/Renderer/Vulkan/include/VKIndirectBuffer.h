#pragma once
#include "VKRenderer.h"


class CVKIndirectBuffer : public CGfxIndirectBuffer
{
	friend class CVKDevice;


private:
	CVKIndirectBuffer(CVKDevice *pDevice, uint32_t count);
	virtual ~CVKIndirectBuffer(void);


public:
	bool BufferData(int indexDraw, int instanceCount);
	bool BufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount);
	void Bind(void *pParam);

private:
	CVKBufferPtr GetBuffer(void) const;


private:
	eastl::vector<DrawCommand> m_draws;

private:
	CVKBufferPtr m_ptrBuffer;

private:
	CVKDevice *m_pDevice;
};
