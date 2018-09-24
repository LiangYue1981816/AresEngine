#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxProfiler.h"


bool CGfxProfiler::bEnableProfiler = false;

size_t CGfxProfiler::textureDataSize = 0;
size_t CGfxProfiler::uniformBufferSize = 0;
size_t CGfxProfiler::vertexBufferSize = 0;
size_t CGfxProfiler::indexBufferSize = 0;
size_t CGfxProfiler::instanceBufferSize = 0;

CGfxProfiler::Sample CGfxProfiler::samples[SampleType::SAMPLE_TYPE_COUNT];

void CGfxProfiler::SetEnableProfiler(bool bEnable)
{
	bEnableProfiler = bEnable;
}

void CGfxProfiler::IncTextureDataSize(size_t size)
{
	textureDataSize += size;
}

void CGfxProfiler::DecTextureDataSize(size_t size)
{
	textureDataSize -= size;
}

void CGfxProfiler::IncUniformBufferSize(size_t size)
{
	uniformBufferSize += size;
}

void CGfxProfiler::DecUniformBufferSize(size_t size)
{
	uniformBufferSize -= size;
}

void CGfxProfiler::IncVertexBufferSize(size_t size)
{
	vertexBufferSize += size;
}

void CGfxProfiler::DecVertexBufferSize(size_t size)
{
	vertexBufferSize -= size;
}

void CGfxProfiler::IncIndexBufferSize(size_t size)
{
	indexBufferSize += size;
}

void CGfxProfiler::DecIndexBufferSize(size_t size)
{
	indexBufferSize -= size;
}

void CGfxProfiler::IncInstanceBufferSize(size_t size)
{
	instanceBufferSize += size;
}

void CGfxProfiler::DecInstanceBufferSize(size_t size)
{
	instanceBufferSize -= size;
}

void CGfxProfiler::ResetSamples(void)
{
	for (int index = 0; index < SampleType::SAMPLE_TYPE_COUNT; index++) {
		samples[index].Reset();
	}
}

void CGfxProfiler::BeginSample(SampleType type, const char *name)
{
	if (bEnableProfiler) {
		samples[type].name = name;
		samples[type].timeBegin = tick();
	}
}

void CGfxProfiler::EndSample(SampleType type)
{
	if (bEnableProfiler) {
		samples[type].timeEnd = tick();
		samples[type].timeTotal += samples[type].timeEnd - samples[type].timeBegin;
		samples[type].timeCount += 1;
	}
}

void CGfxProfiler::LogGfxMemory(void)
{
	LogOutput("GfxRenderer", "GfxMemory\n");
	LogOutput("GfxRenderer", "\tTextureData = %dKB\n", textureDataSize / 1024);
	LogOutput("GfxRenderer", "\tUniformBuffer = %dKB\n", uniformBufferSize / 1024);
	LogOutput("GfxRenderer", "\tVertexBuffer = %dKB\n", vertexBufferSize / 1024);
	LogOutput("GfxRenderer", "\tIndexBuffer = %dKB\n", indexBufferSize / 1024);
	LogOutput("GfxRenderer", "\tInstanceBuffer = %dKB\n", instanceBufferSize / 1024);
	LogOutput("GfxRenderer", "Total memory = %dKB\n", (textureDataSize + uniformBufferSize + vertexBufferSize + indexBufferSize + instanceBufferSize) / 1024);
}

void CGfxProfiler::LogProfiler(void)
{
	if (bEnableProfiler) {
		LogOutput("GfxRenderer", "Profiler\n");

		float totalTime = 0.0f;
		for (int index = 0; index < SampleType::SAMPLE_TYPE_COUNT; index++) {
			if (samples[index].name) {
				LogOutput("GfxRenderer", "\t%s time = %fms count = %d\n", samples[index].name, samples[index].timeTotal / 1000.0f, samples[index].timeCount);
				totalTime += samples[index].timeTotal / 1000.0f;
			}
		}

		LogOutput("GfxRenderer", "Total time=%fms\n", totalTime);
	}
}


CGfxProfilerSample::CGfxProfilerSample(CGfxProfiler::SampleType type, const char *name)
	: m_type(type)
	, m_name(name)
{
	CGfxProfiler::BeginSample(m_type, m_name);
}

CGfxProfilerSample::~CGfxProfilerSample(void)
{
	CGfxProfiler::EndSample(m_type);
}
