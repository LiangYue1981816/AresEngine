#include "GfxHeader.h"


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


static bool bEnable = false;
static size_t bufferSizes[GFX_BUFFER_TYPE_COUNT] = { 0 };
static size_t bufferCounts[GFX_BUFFER_TYPE_COUNT] = { 0 };
static size_t textureSizes[GFX_TEXTURE_COUNT] = { 0 };
static size_t textureCounts[GFX_TEXTURE_COUNT] = { 0 };
static eastl::unordered_map<uint32_t, Sample> samples;


CGfxProfilerSample::CGfxProfilerSample(const char* name)
	: m_name(HashValue(name))
{
	if (bEnable) {
		samples[m_name].name = name;
		samples[m_name].Begin();
	}
}

CGfxProfilerSample::~CGfxProfilerSample(void)
{
	if (bEnable) {
		samples[m_name].End();
	}
}


void CGfxProfiler::SetEnable(bool bEnableProfiler)
{
	bEnable = bEnableProfiler;
}

void CGfxProfiler::IncBufferSize(GfxBufferType type, size_t size)
{
	bufferSizes[type] += size;
	bufferCounts[type] += 1;
}

void CGfxProfiler::DecBufferSize(GfxBufferType type, size_t size)
{
	bufferSizes[type] -= size;
	bufferCounts[type] -= 1;
}

void CGfxProfiler::IncTextureSize(GfxTextureType type, size_t size)
{
	textureSizes[type] += size;
	textureCounts[type] += 1;
}

void CGfxProfiler::DecTextureSzie(GfxTextureType type, size_t size)
{
	textureSizes[type] -= size;
	textureCounts[type] -= 1;
}

void CGfxProfiler::LogGfxMemory(void)
{

}

void CGfxProfiler::LogProfiler(uint32_t frames)
{

}


/*
void CGfxProfiler::LogGfxMemory(void)
{
	LogOutput(LOG_INFO, LOG_TAG_RENDERER, "GfxMemory");
	LogOutput(LOG_INFO, LOG_TAG_RENDERER, "\tTextureData = %dKB", textureDataSize / 1024);
	LogOutput(LOG_INFO, LOG_TAG_RENDERER, "\tUniformBuffer = %dKB", uniformBufferSize / 1024);
	LogOutput(LOG_INFO, LOG_TAG_RENDERER, "\tStorageBuffer = %dKB", storageBufferSize / 1024);
	LogOutput(LOG_INFO, LOG_TAG_RENDERER, "\tVertexBuffer = %dKB", vertexBufferSize / 1024);
	LogOutput(LOG_INFO, LOG_TAG_RENDERER, "\tIndexBuffer = %dKB", indexBufferSize / 1024);
	LogOutput(LOG_INFO, LOG_TAG_RENDERER, "\tInstanceBuffer = %dKB", instanceBufferSize / 1024);
	LogOutput(LOG_INFO, LOG_TAG_RENDERER, "\tIndirectBuffer = %dKB", indirectBufferSize / 1024);
	LogOutput(LOG_INFO, LOG_TAG_RENDERER, "\tTransferBuffer = %dKB", transferBufferSize / 1024);
	LogOutput(LOG_INFO, LOG_TAG_RENDERER, "\tTotal memory = %dKB", (textureDataSize + uniformBufferSize + storageBufferSize + vertexBufferSize + indexBufferSize + instanceBufferSize + indirectBufferSize + transferBufferSize) / 1024);
}

void CGfxProfiler::LogProfiler(int frameCount)
{
	if (bEnableProfiler) {
		LogOutput(LOG_INFO, LOG_TAG_RENDERER, "GfxProfiler");

		float totalTime = 0.0f;
		for (int indexSample = 0; indexSample < SampleType::SAMPLE_TYPE_COUNT; indexSample++) {
			if (samples[indexSample].count) {
				LogOutput(LOG_INFO, LOG_TAG_RENDERER, "\t%s time = %3.3fms count = %d", samples[indexSample].name, samples[indexSample].timeTotal / frameCount / 1000.0f, samples[indexSample].count / frameCount);
				totalTime += samples[indexSample].timeTotal / frameCount / 1000.0f;
			}
		}

		LogOutput(LOG_INFO, LOG_TAG_RENDERER, "\tTotal time=%3.3fms", totalTime);
	}
}

*/