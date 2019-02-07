#pragma once
#include "VKRenderer.h"


class CVKDescriptorSetManager
{
private:
	CVKDescriptorSetManager(CVKDevice *pDevice);
	virtual ~CVKDescriptorSetManager(void);


public:


private:
	CVKDevice *m_pDevice;
};
