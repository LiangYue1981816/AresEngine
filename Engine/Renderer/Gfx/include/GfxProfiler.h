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
	friend class CGfxProfilerSample;


public:
	typedef struct Sample {
		Sample(void)
		{
			name = nullptr;
			timeBegin = 0;
			timeEnd = 0;
			timeTotal = 0;
			count = 0;
		}

		void Begin(void)
		{
			timeBegin = Tick();
		}

		void End(void)
		{
			timeEnd = Tick();
			timeTotal += timeEnd - timeBegin;
			count += 1;
		}

		const char* name;
		uint32_t timeBegin;
		uint32_t timeEnd;
		uint32_t timeTotal;
		uint32_t count;
	} Sample;


public:
	static void SetEnable(bool bEnableProfiler);

	static void IncBufferSize(GfxBufferType type, size_t size);
	static void DecBufferSize(GfxBufferType type, size_t size);

	static void IncTextureSize(GfxTextureType type, size_t size);
	static void DecTextureSzie(GfxTextureType type, size_t size);

	static void LogGfxMemory(void);
	static void LogProfiler(uint32_t frames);


private:
	static bool bEnable;
	static size_t bufferSizes[GFX_BUFFER_TYPE_COUNT];
	static size_t bufferCounts[GFX_BUFFER_TYPE_COUNT];
	static size_t textureSizes[GFX_TEXTURE_COUNT];
	static size_t textureCounts[GFX_TEXTURE_COUNT];
	static eastl::unordered_map<uint32_t, Sample> samples;
};
