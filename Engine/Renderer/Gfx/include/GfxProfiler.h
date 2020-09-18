#pragma once
#include "PreHeader.h"


class CGfxProfilerSample
{
public:
	CGfxProfilerSample(const char* name);
	~CGfxProfilerSample(void);


private:
	uint32_t m_name;
};

class CGfxProfiler
{
public:
	static void SetEnable(bool bEnableProfiler);

	static void IncBufferSize(GfxBufferType type, size_t size);
	static void DecBufferSize(GfxBufferType type, size_t size);

	static void IncTextureSize(GfxTextureType type, size_t size);
	static void DecTextureSzie(GfxTextureType type, size_t size);

	static void LogGfxMemory(void);
	static void LogProfiler(uint32_t frames);
};
