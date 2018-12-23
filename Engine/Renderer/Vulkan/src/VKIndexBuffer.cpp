#include "VKRenderer.h"


CVKIndexBuffer::CVKIndexBuffer(CVKDevice *pDevice, GfxIndexType type, size_t size, bool bDynamic)
	: CGfxIndexBuffer(type, size, bDynamic)
{

}

CVKIndexBuffer::~CVKIndexBuffer(void)
{

}

bool CVKIndexBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	return true;
}

void CVKIndexBuffer::Bind(void *pParam)
{

}
