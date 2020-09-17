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

	typedef enum BufferType {
		BUFFER_TYPE_INDEX_BUFFER = 0,
		BUFFER_TYPE_VERTEX_BUFFER,
		BUFFER_TYPE_UNIFORM_BUFFER,
		BUFFER_TYPE_STORAGE_BUFFER,
		BUFFER_TYPE_INSTANCE_BUFFER,
		BUFFER_TYPE_INDIRECT_BUFFER,
		BUFFER_TYPE_TRANSFER_BNUFER,
		BUFFER_TYPE_COUNT
	} BufferType;

	typedef enum TextureType {
		TEXTURE_TYPE_TEXTURE2D = 0,
		TEXTURE_TYPE_TEXTURE2D_ARRAY,
		TEXTURE_TYPE_TEXTURE_CUBEMAP,
		TEXTURE_TYPE_RENDER_TEXTURE,
		TEXTURE_TYPE_COUNT
	} TextureType;


public:
	static void SetEnable(bool bEnableProfiler);

	static void IncBufferSize(BufferType type, size_t size);
	static void DecBufferSize(BufferType type, size_t size);

	static void IncTextureSize(TextureType type, size_t size);
	static void DecTextureSzie(TextureType type, size_t size);

	static void LogGfxMemory(void);
	static void LogProfiler(uint32_t frames);


private:
	static bool bEnable;
	static size_t bufferSizes[BUFFER_TYPE_COUNT];
	static size_t bufferCounts[BUFFER_TYPE_COUNT];
	static size_t textureSizes[TEXTURE_TYPE_COUNT];
	static size_t textureCounts[TEXTURE_TYPE_COUNT];
	static eastl::unordered_map<uint32_t, Sample> samples;
};
