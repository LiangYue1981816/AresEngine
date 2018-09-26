#pragma once
#include <eastl/vector.h>
#include <eastl/unordered_map.h>


template<class T> class CComponentManager
{
public:
	CComponentManager(void)
	{

	}
	~CComponentManager(void)
	{

	}


public:
	bool NewComponent(uint32_t key, T component)
	{
		if (m_keyIndex.find(key) == m_keyIndex.end()) {
			size_t index = m_components.size();

			m_keyIndex[key] = index;
			m_indexKey[index] = key;
			m_components.push_back(component);

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
		return index < m_components.size() ? &m_components[index] : NULL;
	}

	T* GetComponentByKey(uint32_t key)
	{
		const auto &itKeyValue = m_keyIndex.find(key);
		return itKeyValue != m_keyIndex.end() ? &m_components[itKeyValue->second] : NULL;
	}


private:
	eastl::vector<T> m_components;
	eastl::unordered_map<uint32_t, size_t> m_keyIndex;
	eastl::unordered_map<size_t, uint32_t> m_indexKey;
};

template<class T> class CComponentPtr
{
public:
	CComponentPtr(void)
		: m_key(0)
		, m_pManager(NULL)
		, m_pRefCount(NULL)
	{

	}
	CComponentPtr(uint32_t key, CComponentManager<T> *pManager)
		: m_key(0)
		, m_pManager(NULL)
		, m_pRefCount(NULL)
	{
		Set(key, pManager, NULL);
	}
	CComponentPtr(const CComponentPtr<T> &ptr)
		: m_key(0)
		, m_pManager(NULL)
		, m_pRefCount(NULL)
	{
		Set(ptr.m_key, ptr.m_pManager, ptr.m_pRefCount);
	}
	virtual ~CComponentPtr(void)
	{
		Release();
	}


private:
	inline void Set(uint32_t key, CComponentManager<T> *pManager, uint32_t *pRefCount)
	{
		if (m_key == key && m_pManager == pManager) {
			return;
		}

		Release();

		if (key && pManager) {
			m_key = key;
			m_pManager = pManager;
			m_pRefCount = pRefCount;

			if (m_pRefCount == NULL) {
				m_pRefCount = (uint32_t *)AllocMemory(sizeof(*m_pRefCount)); (*m_pRefCount) = 0;
			}

			++(*m_pRefCount);
		}
	}

public:
	inline void Release(void)
	{
		if (m_pRefCount) {
			if (-- (*m_pRefCount) == 0) {
				FreeMemory(m_pRefCount);
				m_pManager->DeleteComponent(m_key);
			}
		}

		m_key = 0;
		m_pManager = NULL;
		m_pRefCount = NULL;
	}

	inline uint32_t GetKey(void) const
	{
		return m_key;
	}

	inline CComponentManager<T>* GetManager(void) const
	{
		return m_pManager;
	}

	inline T* GetPointer(void) const
	{
		return m_key && m_pManager ? m_pManager->GetComponentByKey(m_key) : NULL;
	}

	inline bool IsNull(void) const
	{
		return m_key && m_pManager ? false : true;
	}

	inline CComponentPtr<T>& operator = (const CComponentPtr<T> &ptr)
	{
		Set(ptr.m_key, ptr.m_pManager, ptr.m_pRefCount);
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


private:
	uint32_t m_key;
	CComponentManager<T> *m_pManager;

private:
	uint32_t *m_pRefCount;
};

template<class T> inline bool operator == (const CComponentPtr<T> &ptrLeft, const CComponentPtr<T> &ptrRight)
{
	return ptrLeft.GetKey() == ptrRight.GetKey() && ptrLeft.GetManager() == ptrRight.GetManager();
}

template<class T> inline bool operator != (const CComponentPtr<T> &ptrLeft, const CComponentPtr<T> &ptrRight)
{
	return ptrLeft.GetKey() != ptrRight.GetKey() || ptrLeft.GetManager() != ptrRight.GetManager();
}

template<class T> inline bool operator < (const CComponentPtr<T> &ptrLeft, const CComponentPtr<T> &ptrRight)
{
	return ptrLeft.GetKey() < ptrRight.GetKey();
}

template<class T> inline bool operator <= (const CComponentPtr<T> &ptrLeft, const CComponentPtr<T> &ptrRight)
{
	return ptrLeft.GetKey() <= ptrRight.GetKey();
}

template<class T> inline bool operator > (const CComponentPtr<T> &ptrLeft, const CComponentPtr<T> &ptrRight)
{
	return ptrLeft.GetKey() > ptrRight.GetKey();
}

template<class T> inline bool operator >= (const CComponentPtr<T> &ptrLeft, const CComponentPtr<T> &ptrRight)
{
	return ptrLeft.GetKey() >= ptrRight.GetKey();
}

template<class T> struct eastl::hash<CComponentPtr<T>>
{
	inline size_t operator()(const CComponentPtr<T> &key) const
	{
		size_t key = (size_t)key.GetKey();
		size_t manager = (size_t)key.GetManager();
		key ^= manager + 0x9e3779b9 + (key << 6) + (key >> 2);
		return key;
	}
};

template<class T> struct eastl::equal_to<CComponentPtr<T>>
{
	inline bool operator()(const CComponentPtr<T> &ptrLeft, const CComponentPtr<T> &ptrRight) const
	{
		return ptrLeft.GetKey() == ptrRight.GetKey() && ptrLeft.GetManager() == ptrRight.GetManager();
	}
};
