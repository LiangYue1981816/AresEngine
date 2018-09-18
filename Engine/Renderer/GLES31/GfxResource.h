#pragma once
#include "GfxRenderer.h"


class CGfxResource
{
protected:
	CGfxResource(void);
	virtual ~CGfxResource(void);
	virtual void Release(void) = 0;
};

template<class T> class CGfxResourcePtr : public CSharedPtr<T>
{
public:
	CGfxResourcePtr(void)
	{

	}
	CGfxResourcePtr(const T *pPointer)
	{
		Set((const T *)pPointer, NULL);
	}
	CGfxResourcePtr(const CGfxResourcePtr<CGfxResource> &ptr)
	{
		Set((const T *)ptr.m_pPointer, ptr.m_pRefCount);
	}
	virtual ~CGfxResourcePtr(void)
	{
		Release();
	}

protected:
	virtual void FreePointer(void)
	{
		if (m_pPointer) {
			((CGfxResource *)m_pPointer)->Release();
		}
	}
};
