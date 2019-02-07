#include "VKRenderer.h"


CVKDescriptorSetManager::CVKDescriptorSetManager(CVKDevice *pDevice)
	: m_pDevice(pDevice)
{
	pthread_mutex_init(&m_lock, nullptr);
}

CVKDescriptorSetManager::~CVKDescriptorSetManager(void)
{
	pthread_mutex_destroy(&m_lock);
}
