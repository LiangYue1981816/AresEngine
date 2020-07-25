#pragma once
#include "PreHeader.h"


class CStream
{
public:
	CStream(void);
	virtual ~CStream(void);


public:
	virtual bool IsValid(void) const = 0;

public:
	virtual bool LoadFromFile(const char* szFileName) = 0;
	virtual bool LoadFromPack(ZZIP_DIR* pPack, const char* szFileName) = 0;
	virtual void Close(void) = 0;

public:
	virtual size_t GetFullSize(void) const = 0;
	virtual size_t GetFreeSize(void) const = 0;
	virtual size_t GetCurrentPosition(void) const = 0;

public:
	virtual size_t Read(void* pBuffer, size_t size, size_t count) = 0;
	virtual bool Seek(int offset, int origin) = 0;
	virtual bool IsEOF(void) const = 0;
};


template<typename T>
inline CStream& operator << (CStream& stream, T& value)
{
	stream.Read((void*)& value, sizeof(value), 1);
	return stream;
}

inline CStream& operator << (CStream& stream, char*& value)
{
	size_t count;
	stream << count;
	stream.Read((void*)value, sizeof(char), count);
	return stream;
}

inline CStream& operator << (CStream& stream, eastl::string& value)
{
	size_t count;
	stream << count; value.resize(count);
	stream.Read((void*)value.data(), sizeof(char), count);
	return stream;
}

template<typename T>
inline CStream& operator << (CStream& stream, eastl::vector<T>& values)
{
	values.clear();

	size_t count;
	stream << count;

	for (size_t index = 0; index < count; index++) {
		T value;
		stream << value;
		values.emplace_back(value);
	}

	return stream;
}

template<typename K, typename T>
inline CStream& operator << (CStream& stream, eastl::map<K, T>& values)
{
	values.clear();

	size_t count;
	stream << count;

	for (size_t index = 0; index < count; index++) {
		K key;
		T value;
		stream << key;
		stream << value;
		values.insert(eastl::map<K, T>::value_type(key, value));
	}

	return stream;
}

template<typename K, typename T>
inline CStream& operator << (CStream& stream, eastl::unordered_map<K, T>& values)
{
	values.clear();

	size_t count;
	stream << count;

	for (size_t index = 0; index < count; index++) {
		K key;
		T value;
		stream << key;
		stream << value;
		values.insert(eastl::unordered_map<K, T>::value_type(key, value));
	}

	return stream;
}
