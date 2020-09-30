#pragma once
#include "VKRenderer.h"


class CVKSemaphore : public CGfxSemaphore
{
public:
	CVKSemaphore(CVKDevice* pDevice);
	virtual ~CVKSemaphore(void);


public:
	VkSemaphore GetSemaphore(void) const;


private:
	VkSemaphore m_vkSemaphore;

private:
	CVKDevice* m_pDevice;
};
