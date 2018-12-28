#pragma once
#include "VKRenderer.h"


class CVKImage : public CGfxResource
{
	friend class CVKDevice;


private:
	CVKImage(CVKDevice *pDevice);
	virtual ~CVKImage(void);
	virtual void Release(void);


private:
	CVKDevice *m_pDevice;
};

typedef CGfxResourcePtr<CVKImage> CVKImagePtr;
