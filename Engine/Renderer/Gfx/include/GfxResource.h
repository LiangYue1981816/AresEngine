#pragma once
#include "PreHeader.h"


class CGfxResource
{
	template<class T>
	friend class CGfxResourcePtr;


public:
	CGfxResource(void)
		: refCount(0)
	{

	}
	CGfxResource(const CGfxResource& resource)
		: refCount(0)
	{
		refCount.store(resource.refCount);
	}
	virtual ~CGfxResource(void)
	{
		ASSERT(refCount == 0);
	}

	virtual void Release(void)
	{

	}

	CGfxResource& operator = (const CGfxResource& resource)
	{
		refCount.store(resource.refCount);
		return *this;
	}


public:
	uint32_t GetRefCount(void)
	{
		return refCount;
	}

private:
	uint32_t IncRefCount(void)
	{
		return ++refCount;
	}

	uint32_t DecRefCount(void)
	{
		return --refCount;
	}


private:
	std::atomic_uint refCount;
};

template<class T>
class CGfxResourcePtr
{
public:
	CGfxResourcePtr(void)
		: m_pPointer(nullptr)
	{

	}
	CGfxResourcePtr(const T* pPointer)
		: m_pPointer(nullptr)
	{
		Set(pPointer);
	}
	CGfxResourcePtr(const CGfxResourcePtr<T>& ptr)
		: m_pPointer(nullptr)
	{
		Set(ptr.m_pPointer);
	}
	virtual ~CGfxResourcePtr(void)
	{
		Release();
	}


private:
	inline void Set(const T* pPointer)
	{
		if (m_pPointer == pPointer) {
			return;
		}

		Release();

		if (pPointer) {
			m_pPointer = (T*)pPointer;
		}

		if (CGfxResource* pResource = (CGfxResource*)m_pPointer) {
			pResource->IncRefCount();
		}
	}

public:
	inline void Release(void)
	{
		if (CGfxResource* pResource = (CGfxResource*)m_pPointer) {
			if (pResource->DecRefCount() == 0) {
				pResource->Release();
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
		if (CGfxResource* pResource = (CGfxResource*)m_pPointer) {
			return pResource->GetRefCount();
		}
		else {
			return 0;
		}
	}

	inline CGfxResourcePtr<T>& operator = (const CGfxResourcePtr<T>& ptr)
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
	T* m_pPointer;
};


template<class T>
inline bool operator == (const CGfxResourcePtr<T>& ptrLeft, const void* pPointer)
{
	return ptrLeft.GetPointer() == pPointer;
}

template<class T>
inline bool operator == (const void* pPointer, const CGfxResourcePtr<T>& ptrRight)
{
	return pPointer == ptrRight.GetPointer();
}

template<class T>
inline bool operator == (const CGfxResourcePtr<T>& ptrLeft, const CGfxResourcePtr<T>& ptrRight)
{
	return ptrLeft.GetPointer() == ptrRight.GetPointer();
}


template<class T>
inline bool operator != (const CGfxResourcePtr<T>& ptrLeft, const void* pPointer)
{
	return ptrLeft.GetPointer() != pPointer;
}

template<class T>
inline bool operator != (const void* pPointer, const CGfxResourcePtr<T>& ptrRight)
{
	return pPointer != ptrRight.GetPointer();
}

template<class T>
inline bool operator != (const CGfxResourcePtr<T>& ptrLeft, const CGfxResourcePtr<T>& ptrRight)
{
	return ptrLeft.GetPointer() != ptrRight.GetPointer();
}


template<class T>
inline bool operator < (const CGfxResourcePtr<T>& ptrLeft, const void* pPointer)
{
	return ptrLeft.GetPointer() < pPointer;
}

template<class T>
inline bool operator < (const void* pPointer, const CGfxResourcePtr<T>& ptrRight)
{
	return pPointer < ptrRight.GetPointer();
}

template<class T>
inline bool operator < (const CGfxResourcePtr<T>& ptrLeft, const CGfxResourcePtr<T>& ptrRight)
{
	return ptrLeft.GetPointer() < ptrRight.GetPointer();
}


template<class T>
inline bool operator <= (const CGfxResourcePtr<T>& ptrLeft, const void* pPointer)
{
	return ptrLeft.GetPointer() <= pPointer;
}

template<class T>
inline bool operator <= (const void* pPointer, const CGfxResourcePtr<T>& ptrRight)
{
	return pPointer <= ptrRight.GetPointer();
}

template<class T>
inline bool operator <= (const CGfxResourcePtr<T>& ptrLeft, const CGfxResourcePtr<T>& ptrRight)
{
	return ptrLeft.GetPointer() <= ptrRight.GetPointer();
}


template<class T>
inline bool operator > (const CGfxResourcePtr<T>& ptrLeft, const void* pPointer)
{
	return ptrLeft.GetPointer() > pPointer;
}

template<class T>
inline bool operator > (const void* pPointer, const CGfxResourcePtr<T>& ptrRight)
{
	return pPointer > ptrRight.GetPointer();
}

template<class T>
inline bool operator > (const CGfxResourcePtr<T>& ptrLeft, const CGfxResourcePtr<T>& ptrRight)
{
	return ptrLeft.GetPointer() > ptrRight.GetPointer();
}


template<class T>
inline bool operator >= (const CGfxResourcePtr<T>& ptrLeft, const void* pPointer)
{
	return ptrLeft.GetPointer() >= pPointer;
}

template<class T>
inline bool operator >= (const void* pPointer, const CGfxResourcePtr<T>& ptrRight)
{
	return pPointer >= ptrRight.GetPointer();
}

template<class T>
inline bool operator >= (const CGfxResourcePtr<T>& ptrLeft, const CGfxResourcePtr<T>& ptrRight)
{
	return ptrLeft.GetPointer() >= ptrRight.GetPointer();
}


template<class T>
struct eastl::hash<CGfxResourcePtr<T>>
{
	inline size_t operator()(const CGfxResourcePtr<T>& key) const
	{
		return (size_t)key.GetPointer();
	}
};

template<class T>
struct eastl::equal_to<CGfxResourcePtr<T>>
{
	inline bool operator()(const CGfxResourcePtr<T>& ptrLeft, const CGfxResourcePtr<T>& ptrRight) const
	{
		return ptrLeft.GetPointer() == ptrRight.GetPointer();
	}
};
