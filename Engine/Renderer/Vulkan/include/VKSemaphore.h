#pragma once
#include "GfxRenderer.h"


class CVKSemaphore : public CGfxSemaphore
{
public:
	CVKSemaphore(CVKDevice* pDevice);
	virtual ~CVKSemaphore(void);


private:
	bool Create(void);
	void Destroy(void);

public:
	VkSemaphore GetSemaphore(void) const;


private:
	VkSemaphore m_vkSemaphore;

private:
	CVKDevice* m_pDevice;
};
