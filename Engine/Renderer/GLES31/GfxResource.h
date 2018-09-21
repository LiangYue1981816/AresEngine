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


private:
	uint32_t refCount;
};

template<class T> class CGfxResourcePtr
{
public:
	CGfxResourcePtr(void)
		: m_pPointer(NULL)
	{

	}
	CGfxResourcePtr(const T *pPointer)
		: m_pPointer(NULL)
	{
		Set((const T *)pPointer);
	}
	CGfxResourcePtr(const CGfxResourcePtr<T> &ptr)
		: m_pPointer(NULL)
	{
		Set((const T *)ptr.m_pPointer);
	}
	virtual ~CGfxResourcePtr(void)
	{
		Release();
	}


private:
	inline void Set(const T *pPointer)
	{
		if (m_pPointer == pPointer) {
			return;
		}

		Release();

		if (pPointer) {
			m_pPointer = (T *)pPointer;
			((CGfxResource *)m_pPointer)->refCount++;
		}
	}

	virtual void FreePointer(void)
	{
		if (m_pPointer) {
			((CGfxResource *)m_pPointer)->Release();
		}
	}

public:
	inline void Release(void)
	{
		if (m_pPointer) {
			if (--(((CGfxResource *)m_pPointer)->refCount) == 0) {
				FreePointer();
			}
		}

		m_pPointer = NULL;
	}

	inline bool IsValid(void) const
	{
		return m_pPointer ? true : false;
	}

	inline bool IsNull(void) const
	{
		return m_pPointer ? false : true;
	}

	inline T* GetPointer(void) const
	{
		return m_pPointer;
	}

	inline uint32_t GetRefCount(void) const
	{
		return m_pPointer ? ((CGfxResource *)m_pPointer)->refCount : 0;
	}

	inline CGfxResourcePtr<T>& operator = (const CGfxResourcePtr<T> &ptr)
	{
		Set(ptr.m_pPointer);
		return *this;
	}

	inline T* operator -> (void) const
	{
		return m_pPointer;
	}

	inline operator T* (void) const
	{
		return m_pPointer;
	}

	inline operator const T* (void) const
	{
		return m_pPointer;
	}


private:
	T *m_pPointer;
};

template<class T> inline bool operator == (const CGfxResourcePtr<T> &ptrLeft, const void *pPointer)
{
	return ptrLeft.GetPointer() == pPointer;
}

template<class T> inline bool operator == (const void *pPointer, const CGfxResourcePtr<T> &ptrRight)
{
	return pPointer == ptrRight.GetPointer();
}

template<class T> inline bool operator == (const CGfxResourcePtr<T> &ptrLeft, const CGfxResourcePtr<T> &ptrRight)
{
	return ptrLeft.GetPointer() == ptrRight.GetPointer();
}


template<class T> inline bool operator != (const CGfxResourcePtr<T> &ptrLeft, const void *pPointer)
{
	return ptrLeft.GetPointer() != pPointer;
}

template<class T> inline bool operator != (const void *pPointer, const CGfxResourcePtr<T> &ptrRight)
{
	return pPointer != ptrRight.GetPointer();
}

template<class T> inline bool operator != (const CGfxResourcePtr<T> &ptrLeft, const CGfxResourcePtr<T> &ptrRight)
{
	return ptrLeft.GetPointer() != ptrRight.GetPointer();
}


template<class T> inline bool operator < (const CGfxResourcePtr<T> &ptrLeft, const void *pPointer)
{
	return ptrLeft.GetPointer() < pPointer;
}

template<class T> inline bool operator < (const void *pPointer, const CGfxResourcePtr<T> &ptrRight)
{
	return pPointer < ptrRight.GetPointer();
}

template<class T> inline bool operator < (const CGfxResourcePtr<T> &ptrLeft, const CGfxResourcePtr<T> &ptrRight)
{
	return ptrLeft.GetPointer() < ptrRight.GetPointer();
}


template<class T> inline bool operator <= (const CGfxResourcePtr<T> &ptrLeft, const void *pPointer)
{
	return ptrLeft.GetPointer() <= pPointer;
}

template<class T> inline bool operator <= (const void *pPointer, const CGfxResourcePtr<T> &ptrRight)
{
	return pPointer <= ptrRight.GetPointer();
}

template<class T> inline bool operator <= (const CGfxResourcePtr<T> &ptrLeft, const CGfxResourcePtr<T> &ptrRight)
{
	return ptrLeft.GetPointer() <= ptrRight.GetPointer();
}


template<class T> inline bool operator > (const CGfxResourcePtr<T> &ptrLeft, const void *pPointer)
{
	return ptrLeft.GetPointer() > pPointer;
}

template<class T> inline bool operator > (const void *pPointer, const CGfxResourcePtr<T> &ptrRight)
{
	return pPointer > ptrRight.GetPointer();
}

template<class T> inline bool operator > (const CGfxResourcePtr<T> &ptrLeft, const CGfxResourcePtr<T> &ptrRight)
{
	return ptrLeft.GetPointer() > ptrRight.GetPointer();
}


template<class T> inline bool operator >= (const CGfxResourcePtr<T> &ptrLeft, const void *pPointer)
{
	return ptrLeft.GetPointer() >= pPointer;
}

template<class T> inline bool operator >= (const void *pPointer, const CGfxResourcePtr<T> &ptrRight)
{
	return pPointer >= ptrRight.GetPointer();
}

template<class T> inline bool operator >= (const CGfxResourcePtr<T> &ptrLeft, const CGfxResourcePtr<T> &ptrRight)
{
	return ptrLeft.GetPointer() >= ptrRight.GetPointer();
}

template<class T> struct eastl::hash<CGfxResourcePtr<T>>
{
	inline size_t operator()(const CGfxResourcePtr<T> &key) const
	{
		return (size_t)key.GetPointer();
	}
};

template<class T> struct eastl::equal_to<CGfxResourcePtr<T>>
{
	inline bool operator()(const CGfxResourcePtr<T> &ptrLeft, const CGfxResourcePtr<T> &ptrRight) const
	{
		return ptrLeft.GetPointer() == ptrRight.GetPointer();
	}
};
