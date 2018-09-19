#pragma once
#include "GfxRenderer.h"


class CGfxResource
{
	template<class T>
	friend class CGfxResourcePtr;


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
		CSharedPtr<T>::Set((const T *)pPointer, NULL);
	}
	CGfxResourcePtr(const CGfxResourcePtr<CGfxResource> &ptr)
	{
		CSharedPtr<T>::Set((const T *)ptr.m_pPointer, ptr.m_pRefCount);
	}
	virtual ~CGfxResourcePtr(void)
	{
		CSharedPtr<T>::Release();
	}

protected:
	virtual void FreePointer(void)
	{
		if (CSharedPtr<T>::m_pPointer) {
			((CGfxResource *)CSharedPtr<T>::m_pPointer)->Release();
		}
	}
};
