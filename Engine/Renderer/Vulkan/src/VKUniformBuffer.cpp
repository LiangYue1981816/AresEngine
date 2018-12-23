#include "VKRenderer.h"


CVKUniformBuffer::CVKUniformBuffer(CVKDevice *pDevice, size_t size, bool bDynamic)
	: CGfxUniformBuffer(size, bDynamic)
{

}

CVKUniformBuffer::~CVKUniformBuffer(void)
{

}

bool CVKUniformBuffer::BufferData(size_t offset, size_t size, const void *pBuffer)
{
	return true;
}

void CVKUniformBuffer::Bind(void *pParam)
{

}
