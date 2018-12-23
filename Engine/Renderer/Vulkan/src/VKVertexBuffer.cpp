#include "VKRenderer.h"


CVKVertexBuffer::CVKVertexBuffer(CVKDevice *pDevice, uint32_t vertexFormat, uint32_t vertexBinding, size_t size, bool bDynamic)
	: CGfxVertexBuffer(vertexFormat, vertexBinding, size, bDynamic)
{

}

CVKVertexBuffer::~CVKVertexBuffer(void)
{

}

bool CVKVertexBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	return true;
}

void CVKVertexBuffer::Bind(void *pParam)
{

}
