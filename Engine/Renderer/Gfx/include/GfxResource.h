#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxResource
{
	template<class T>
	friend class CGfxResourcePtr;


public:
	CGfxResource(void);
	virtual ~CGfxResource(void);
	virtual void Release(void) = 0;


private:
	uint32_t IncRefCount(void);
	uint32_t DecRefCount(void);


private:
	uint32_t refCount;
	std::atomic_flag lock;
};

template<class T>
class CALL_API CGfxResourcePtr
{
public:
	CGfxResourcePtr(void)
		: m_pPointer(nullptr)
	{

	}
	CGfxResourcePtr(const T *pPointer)
		: m_pPointer(nullptr)
	{
		Set(pPointer);
	}
	CGfxResourcePtr(const CGfxResourcePtr<T> &ptr)
		: m_pPointer(nullptr)
	{
		Set(ptr.m_pPointer);
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

		if ((m_pPointer = (T *)pPointer) != nullptr) {
			((CGfxResource *)m_pPointer)->IncRefCount();
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
			if (((CGfxResource *)m_pPointer)->DecRefCount() == 0) {
				FreePointer();
			}
		}

		m_pPointer = nullptr;
	}

	inline bool IsValid(void) const
	{
		return m_pPointer != nullptr;
	}

	inline bool IsNull(void) const
	{
		return m_pPointer == nullptr;
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

	inline operator bool (void) const
	{
		return m_pPointer != nullptr;
	}


private:
	T *m_pPointer;
};

template<class T>
inline bool operator == (const CGfxResourcePtr<T> &ptrLeft, const void *pPointer)
{
	return ptrLeft.GetPointer() == pPointer;
}

template<class T>
inline bool operator == (const void *pPointer, const CGfxResourcePtr<T> &ptrRight)
{
	return pPointer == ptrRight.GetPointer();
}

template<class T>
inline bool operator == (const CGfxResourcePtr<T> &ptrLeft, const CGfxResourcePtr<T> &ptrRight)
{
	return ptrLeft.GetPointer() == ptrRight.GetPointer();
}


template<class T>
inline bool operator != (const CGfxResourcePtr<T> &ptrLeft, const void *pPointer)
{
	return ptrLeft.GetPointer() != pPointer;
}

template<class T>
inline bool operator != (const void *pPointer, const CGfxResourcePtr<T> &ptrRight)
{
	return pPointer != ptrRight.GetPointer();
}

template<class T>
inline bool operator != (const CGfxResourcePtr<T> &ptrLeft, const CGfxResourcePtr<T> &ptrRight)
{
	return ptrLeft.GetPointer() != ptrRight.GetPointer();
}


template<class T>
inline bool operator < (const CGfxResourcePtr<T> &ptrLeft, const void *pPointer)
{
	return ptrLeft.GetPointer() < pPointer;
}

template<class T>
inline bool operator < (const void *pPointer, const CGfxResourcePtr<T> &ptrRight)
{
	return pPointer < ptrRight.GetPointer();
}

template<class T>
inline bool operator < (const CGfxResourcePtr<T> &ptrLeft, const CGfxResourcePtr<T> &ptrRight)
{
	return ptrLeft.GetPointer() < ptrRight.GetPointer();
}


template<class T>
inline bool operator <= (const CGfxResourcePtr<T> &ptrLeft, const void *pPointer)
{
	return ptrLeft.GetPointer() <= pPointer;
}

template<class T>
inline bool operator <= (const void *pPointer, const CGfxResourcePtr<T> &ptrRight)
{
	return pPointer <= ptrRight.GetPointer();
}

template<class T>
inline bool operator <= (const CGfxResourcePtr<T> &ptrLeft, const CGfxResourcePtr<T> &ptrRight)
{
	return ptrLeft.GetPointer() <= ptrRight.GetPointer();
}


template<class T>
inline bool operator > (const CGfxResourcePtr<T> &ptrLeft, const void *pPointer)
{
	return ptrLeft.GetPointer() > pPointer;
}

template<class T>
inline bool operator > (const void *pPointer, const CGfxResourcePtr<T> &ptrRight)
{
	return pPointer > ptrRight.GetPointer();
}

template<class T>
inline bool operator > (const CGfxResourcePtr<T> &ptrLeft, const CGfxResourcePtr<T> &ptrRight)
{
	return ptrLeft.GetPointer() > ptrRight.GetPointer();
}


template<class T>
inline bool operator >= (const CGfxResourcePtr<T> &ptrLeft, const void *pPointer)
{
	return ptrLeft.GetPointer() >= pPointer;
}

template<class T>
inline bool operator >= (const void *pPointer, const CGfxResourcePtr<T> &ptrRight)
{
	return pPointer >= ptrRight.GetPointer();
}

template<class T>
inline bool operator >= (const CGfxResourcePtr<T> &ptrLeft, const CGfxResourcePtr<T> &ptrRight)
{
	return ptrLeft.GetPointer() >= ptrRight.GetPointer();
}

template<class T>
struct eastl::hash<CGfxResourcePtr<T>>
{
	inline size_t operator()(const CGfxResourcePtr<T> &key) const
	{
		return (size_t)key.GetPointer();
	}
};

template<class T>
struct eastl::equal_to<CGfxResourcePtr<T>>
{
	inline bool operator()(const CGfxResourcePtr<T> &ptrLeft, const CGfxResourcePtr<T> &ptrRight) const
	{
		return ptrLeft.GetPointer() == ptrRight.GetPointer();
	}
};
