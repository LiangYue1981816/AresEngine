#pragma once
#include "PreHeader.h"


class CALL_API CComponentBase
{
	template<class T>
	friend class CComponentPtr;


public:
	CComponentBase(void)
		: refCount(0)
	{

	}
	CComponentBase(const CComponentBase& component)
		: refCount(0)
	{
		refCount.store(component.refCount);
	}
	virtual ~CComponentBase(void)
	{
		ASSERT(refCount == 0);
	}

	virtual void Release(void)
	{

	}

	CComponentBase& operator = (const CComponentBase& component)
	{
		refCount.store(component.refCount);
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
class CALL_API CComponentManager
{
public:
	CComponentManager(void)
	{

	}
	~CComponentManager(void)
	{

	}


public:
	void Reserve(size_t size)
	{
		m_components.reserve(size);
	}

	bool NewComponent(uint32_t key, T component)
	{
		if (m_keyIndex.find(key) == m_keyIndex.end()) {
			size_t index = m_components.size();

			m_keyIndex[key] = index;
			m_indexKey[index] = key;
			m_components.emplace_back(component);

			return true;
		}
		else {
			return false;
		}
	}

	bool DeleteComponent(uint32_t key)
	{
		if (m_keyIndex.find(key) != m_keyIndex.end()) {
			size_t indexCurrent = m_keyIndex[key];
			size_t indexEnd = m_components.size() - 1;
			uint32_t keyEnd = m_indexKey[indexEnd];

			m_keyIndex[keyEnd] = indexCurrent;
			m_keyIndex.erase(key);

			m_indexKey[indexCurrent] = keyEnd;
			m_indexKey.erase(indexEnd);

			eastl::swap(m_components[indexCurrent], m_components[indexEnd]);

			auto itComponent = m_components.end();
			m_components.erase(--itComponent);

			return true;
		}
		else {
			return false;
		}
	}

	void DeleteComponentAll(void)
	{
		m_keyIndex.clear();
		m_indexKey.clear();
		m_components.clear();
	}

	size_t GetComponentCount(void) const
	{
		return m_components.size();
	}

	T* GetComponentByIndex(size_t index)
	{
		if (index < m_components.size()) {
			return &m_components[index];
		}
		else {
			return nullptr;
		}
	}

	T* GetComponentByKey(uint32_t key)
	{
		const auto& itKeyValue = m_keyIndex.find(key);

		if (itKeyValue != m_keyIndex.end()) {
			return &m_components[itKeyValue->second];
		}
		else {
			return nullptr;
		}
	}


private:
	eastl::vector<T> m_components;
	eastl::unordered_map<uint32_t, size_t> m_keyIndex;
	eastl::unordered_map<size_t, uint32_t> m_indexKey;
};

template<class T>
class CALL_API CComponentPtr
{
public:
	CComponentPtr(void)
		: m_key(0)
		, m_pManager(nullptr)
	{

	}
	CComponentPtr(uint32_t key, CComponentManager<T>* pManager)
		: m_key(0)
		, m_pManager(nullptr)
	{
		Set(key, pManager);
	}
	CComponentPtr(const CComponentPtr<T>& ptr)
		: m_key(0)
		, m_pManager(nullptr)
	{
		Set(ptr.m_key, ptr.m_pManager);
	}
	virtual ~CComponentPtr(void)
	{
		Release();
	}


private:
	inline void Set(uint32_t key, CComponentManager<T>* pManager)
	{
		if (m_key == key && m_pManager == pManager) {
			return;
		}

		Release();

		if (key && pManager) {
			m_key = key;
			m_pManager = pManager;
		}

		if (CComponentBase* pComponent = (CComponentBase*)GetPointer()) {
			pComponent->IncRefCount();
		}
	}

public:
	inline void Release(void)
	{
		if (CComponentBase* pComponent = (CComponentBase*)GetPointer()) {
			if (pComponent->DecRefCount() == 0) {
				pComponent->Release();
				m_pManager->DeleteComponent(m_key);
			}
		}

		m_key = 0;
		m_pManager = nullptr;
	}

	inline bool IsValid(void) const
	{
		return GetPointer() != nullptr;
	}

	inline bool IsNull(void) const
	{
		return GetPointer() == nullptr;
	}

	inline T* GetPointer(void) const
	{
		if (m_key && m_pManager) {
			return m_pManager->GetComponentByKey(m_key);
		}
		else {
			return nullptr;
		}
	}

	inline uint32_t GetRefCount(void) const
	{
		if (CComponentBase* pComponent = (CComponentBase*)GetPointer()) {
			return pComponent->GetRefCount();
		}
		else {
			return 0;
		}
	}

	inline CComponentPtr<T>& operator = (const CComponentPtr<T>& ptr)
	{
		Set(ptr.m_key, ptr.m_pManager);
		return *this;
	}

	inline T* operator -> (void) const
	{
		return GetPointer();
	}

	inline operator T* (void) const
	{
		return GetPointer();
	}

	inline operator const T* (void) const
	{
		return GetPointer();
	}

	inline operator bool(void) const
	{
		return GetPointer() != nullptr;
	}


private:
	uint32_t m_key;
	CComponentManager<T>* m_pManager;
};


template<class T>
inline bool operator == (const CComponentPtr<T>& ptrLeft, const void* pPointer)
{
	return ptrLeft.GetPointer() == pPointer;
}

template<class T>
inline bool operator == (const void* pPointer, const CComponentPtr<T>& ptrRight)
{
	return pPointer == ptrRight.GetPointer();
}

template<class T>
inline bool operator == (const CComponentPtr<T>& ptrLeft, const CComponentPtr<T>& ptrRight)
{
	return ptrLeft.GetKey() == ptrRight.GetKey() && ptrLeft.GetManager() == ptrRight.GetManager();
}


template<class T>
inline bool operator != (const CComponentPtr<T>& ptrLeft, const void* pPointer)
{
	return ptrLeft.GetPointer() != pPointer;
}

template<class T>
inline bool operator != (const void* pPointer, const CComponentPtr<T>& ptrRight)
{
	return pPointer != ptrRight.GetPointer();
}

template<class T>
inline bool operator != (const CComponentPtr<T>& ptrLeft, const CComponentPtr<T>& ptrRight)
{
	return ptrLeft.GetKey() != ptrRight.GetKey() || ptrLeft.GetManager() != ptrRight.GetManager();
}


template<class T>
inline bool operator < (const CComponentPtr<T>& ptrLeft, const void* pPointer)
{
	return ptrLeft.GetPointer() < pPointer;
}

template<class T>
inline bool operator < (const void* pPointer, const CComponentPtr<T>& ptrRight)
{
	return pPointer < ptrRight.GetPointer();
}

template<class T>
inline bool operator < (const CComponentPtr<T>& ptrLeft, const CComponentPtr<T>& ptrRight)
{
	return ptrLeft.GetKey() < ptrRight.GetKey();
}


template<class T>
inline bool operator <= (const CComponentPtr<T>& ptrLeft, const void* pPointer)
{
	return ptrLeft.GetPointer() <= pPointer;
}

template<class T>
inline bool operator <= (const void* pPointer, const CComponentPtr<T>& ptrRight)
{
	return pPointer <= ptrRight.GetPointer();
}

template<class T>
inline bool operator <= (const CComponentPtr<T>& ptrLeft, const CComponentPtr<T>& ptrRight)
{
	return ptrLeft.GetKey() <= ptrRight.GetKey();
}


template<class T>
inline bool operator > (const CComponentPtr<T>& ptrLeft, const void* pPointer)
{
	return ptrLeft.GetPointer() > pPointer;
}

template<class T>
inline bool operator > (const void* pPointer, const CComponentPtr<T>& ptrRight)
{
	return pPointer > ptrRight.GetPointer();
}

template<class T>
inline bool operator > (const CComponentPtr<T>& ptrLeft, const CComponentPtr<T>& ptrRight)
{
	return ptrLeft.GetKey() > ptrRight.GetKey();
}


template<class T>
inline bool operator >= (const CComponentPtr<T>& ptrLeft, const void* pPointer)
{
	return ptrLeft.GetPointer() >= pPointer;
}

template<class T>
inline bool operator >= (const void* pPointer, const CComponentPtr<T>& ptrRight)
{
	return pPointer >= ptrRight.GetPointer();
}

template<class T>
inline bool operator >= (const CComponentPtr<T>& ptrLeft, const CComponentPtr<T>& ptrRight)
{
	return ptrLeft.GetKey() >= ptrRight.GetKey();
}


template<class T>
struct eastl::hash<CComponentPtr<T>>
{
	inline size_t operator()(const CComponentPtr<T>& ptr) const
	{
		size_t key = (size_t)ptr.GetKey();
		size_t manager = (size_t)ptr.GetManager();
		key ^= manager + 0x9e3779b9 + (key << 6) + (key >> 2);
		return key;
	}
};

template<class T>
struct eastl::equal_to<CComponentPtr<T>>
{
	inline bool operator()(const CComponentPtr<T>& ptrLeft, const CComponentPtr<T>& ptrRight) const
	{
		return ptrLeft.GetKey() == ptrRight.GetKey() && ptrLeft.GetManager() == ptrRight.GetManager();
	}
};
