#include "VKRenderer.h"


CVKCommandBufferManager::CVKCommandBufferManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
{

}

CVKCommandBufferManager::~CVKCommandBufferManager(void)
{

}

CVKCommandBuffer* CVKCommandBufferManager::Create(bool bMainCommandBuffer)
{
	mutex_autolock autolock(&lock);
	{
		return nullptr;
	}
}

void CVKCommandBufferManager::Destroy(CVKCommandBuffer *pCommandBuffer)
{
	mutex_autolock autolock(&lock);
	{

	}
}
