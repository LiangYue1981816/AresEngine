#pragma once
#include <stdint.h>


template<class T> class CSharedPtr
{
public:
	CSharedPtr(void)
		: m_pPointer(NULL)
		, m_pRefCount(NULL)
	{

	}
	CSharedPtr(const T *pPointer)
		: m_pPointer(NULL)
		, m_pRefCount(NULL)
	{
		Set(pPointer, NULL);
	}
	CSharedPtr(const CSharedPtr<T> &ptr)
		: m_pPointer(NULL)
		, m_pRefCount(NULL)
	{
		Set(ptr.m_pPointer, ptr.m_pRefCount);
	}
	virtual ~CSharedPtr(void)
	{

	}


protected:
	inline void Set(const T *pPointer, const uint32_t *pRefCount)
	{
		if (m_pPointer == pPointer) {
			return;
		}

		Release();

		if (pPointer) {
			m_pPointer = (T *)pPointer;
			m_pRefCount = (uint32_t *)pRefCount;

			if (m_pRefCount == NULL) {
				m_pRefCount = (uint32_t *)malloc(sizeof(*m_pRefCount)); (*m_pRefCount) = 0;
			}

			++(*m_pRefCount);
		}
	}

	virtual void FreePointer(void) = 0;


public:
	inline void Release(void)
	{
		if (m_pRefCount) {
			if (-- (*m_pRefCount) == 0) {
				free(m_pRefCount);
				FreePointer();
			}
		}

		m_pPointer = NULL;
		m_pRefCount = NULL;
	}

	inline bool IsValid(void) const
	{
		return m_pPointer && m_pRefCount ? true : false;
	}

	inline bool IsNull(void) const
	{
		return m_pPointer && m_pRefCount ? false : true;
	}

	inline T* GetPointer(void) const
	{
		return m_pPointer;
	}

	inline uint32_t GetRefCount(void) const
	{
		return *m_pRefCount;
	}

	inline CSharedPtr<T>& operator = (const CSharedPtr<T> &ptr)
	{
		Set(ptr.m_pPointer, ptr.m_pRefCount);
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


protected:
	T *m_pPointer;
	uint32_t *m_pRefCount;
};


template<class T> inline bool operator == (const CSharedPtr<T> &ptrLeft, const void *pPointer)
{
	return ptrLeft.GetPointer() == pPointer;
}

template<class T> inline bool operator == (const void *pPointer, const CSharedPtr<T> &ptrRight)
{
	return pPointer == ptrRight.GetPointer();
}

template<class T> inline bool operator == (const CSharedPtr<T> &ptrLeft, const CSharedPtr<T> &ptrRight)
{
	return ptrLeft.GetPointer() == ptrRight.GetPointer();
}


template<class T> inline bool operator != (const CSharedPtr<T> &ptrLeft, const void *pPointer)
{
	return ptrLeft.GetPointer() != pPointer;
}

template<class T> inline bool operator != (const void *pPointer, const CSharedPtr<T> &ptrRight)
{
	return pPointer != ptrRight.GetPointer();
}

template<class T> inline bool operator != (const CSharedPtr<T> &ptrLeft, const CSharedPtr<T> &ptrRight)
{
	return ptrLeft.GetPointer() != ptrRight.GetPointer();
}


template<class T> inline bool operator < (const CSharedPtr<T> &ptrLeft, const void *pPointer)
{
	return ptrLeft.GetPointer() < pPointer;
}

template<class T> inline bool operator < (const void *pPointer, const CSharedPtr<T> &ptrRight)
{
	return pPointer < ptrRight.GetPointer();
}

template<class T> inline bool operator < (const CSharedPtr<T> &ptrLeft, const CSharedPtr<T> &ptrRight)
{
	return ptrLeft.GetPointer() < ptrRight.GetPointer();
}


template<class T> inline bool operator <= (const CSharedPtr<T> &ptrLeft, const void *pPointer)
{
	return ptrLeft.GetPointer() <= pPointer;
}

template<class T> inline bool operator <= (const void *pPointer, const CSharedPtr<T> &ptrRight)
{
	return pPointer <= ptrRight.GetPointer();
}

template<class T> inline bool operator <= (const CSharedPtr<T> &ptrLeft, const CSharedPtr<T> &ptrRight)
{
	return ptrLeft.GetPointer() <= ptrRight.GetPointer();
}


template<class T> inline bool operator > (const CSharedPtr<T> &ptrLeft, const void *pPointer)
{
	return ptrLeft.GetPointer() > pPointer;
}

template<class T> inline bool operator > (const void *pPointer, const CSharedPtr<T> &ptrRight)
{
	return pPointer > ptrRight.GetPointer();
}

template<class T> inline bool operator > (const CSharedPtr<T> &ptrLeft, const CSharedPtr<T> &ptrRight)
{
	return ptrLeft.GetPointer() > ptrRight.GetPointer();
}


template<class T> inline bool operator >= (const CSharedPtr<T> &ptrLeft, const void *pPointer)
{
	return ptrLeft.GetPointer() >= pPointer;
}

template<class T> inline bool operator >= (const void *pPointer, const CSharedPtr<T> &ptrRight)
{
	return pPointer >= ptrRight.GetPointer();
}

template<class T> inline bool operator >= (const CSharedPtr<T> &ptrLeft, const CSharedPtr<T> &ptrRight)
{
	return ptrLeft.GetPointer() >= ptrRight.GetPointer();
}
