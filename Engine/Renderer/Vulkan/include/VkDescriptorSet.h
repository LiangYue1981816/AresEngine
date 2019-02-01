#pragma once
#include "VKRenderer.h"


class CVKDescriptorSet
{
private:
	CVKDescriptorSet(CVKDevice *pDevice, VkDescriptorSet vkDescriptorSet);
	virtual ~CVKDescriptorSet(void);


private:
	CVKDevice *m_pDevice;
};
