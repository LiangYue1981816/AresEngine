#pragma once
#include "VKRenderer.h"


class CVKMemoryAllocator
{
	friend class CVKMemoryManager;


private:
	CVKMemoryAllocator(void);
	virtual ~CVKMemoryAllocator(void);
};
