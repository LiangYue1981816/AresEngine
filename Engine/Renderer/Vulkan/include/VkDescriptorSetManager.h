#pragma once
#include "VKRenderer.h"


class CVKDescriptorSetManager
{
private:
	CVKDescriptorSetManager(CVKDevice *pDevice);
	virtual ~CVKDescriptorSetManager(void);


public:


private:
	pthread_mutex_t m_lock;

private:
	CVKDevice *m_pDevice;
};
