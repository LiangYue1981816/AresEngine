#include "GfxHeader.h"


bool CGfxProfiler::bEnableProfiler = false;

size_t CGfxProfiler::textureDataSize = 0;
size_t CGfxProfiler::uniformBufferSize = 0;
size_t CGfxProfiler::vertexBufferSize = 0;
size_t CGfxProfiler::indexBufferSize = 0;
size_t CGfxProfiler::instanceBufferSize = 0;
size_t CGfxProfiler::drawIndirectBufferSize = 0;

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

void CGfxProfiler::IncDrawIndirectBufferSize(size_t size)
{
	drawIndirectBufferSize += size;
}

void CGfxProfiler::DecDrawIndirectBufferSize(size_t size)
{
	drawIndirectBufferSize -= size;
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
		samples[type].timeBegin = Tick();
	}
}

void CGfxProfiler::EndSample(SampleType type)
{
	if (bEnableProfiler) {
		samples[type].timeEnd = Tick();
		samples[type].timeTotal += samples[type].timeEnd - samples[type].timeBegin;
		samples[type].count += 1;
	}
}

void CGfxProfiler::LogGfxMemory(void)
{
	LogOutput(LOG_TAG_RENDERER, "GfxMemory\n");
	LogOutput(LOG_TAG_RENDERER, "\tTextureData = %dKB\n", textureDataSize / 1024);
	LogOutput(LOG_TAG_RENDERER, "\tUniformBuffer = %dKB\n", uniformBufferSize / 1024);
	LogOutput(LOG_TAG_RENDERER, "\tVertexBuffer = %dKB\n", vertexBufferSize / 1024);
	LogOutput(LOG_TAG_RENDERER, "\tIndexBuffer = %dKB\n", indexBufferSize / 1024);
	LogOutput(LOG_TAG_RENDERER, "\tInstanceBuffer = %dKB\n", instanceBufferSize / 1024);
	LogOutput(LOG_TAG_RENDERER, "\tDrawIndirectBuffer = %dKB\n", drawIndirectBufferSize / 1024);
	LogOutput(LOG_TAG_RENDERER, "\tTotal memory = %dKB\n", (textureDataSize + uniformBufferSize + vertexBufferSize + indexBufferSize + instanceBufferSize) / 1024);
}

void CGfxProfiler::LogProfiler(int frameCount)
{
	if (bEnableProfiler) {
		LogOutput(LOG_TAG_RENDERER, "GfxProfiler\n");

		float totalTime = 0.0f;
		for (int index = 0; index < SampleType::SAMPLE_TYPE_COUNT; index++) {
			if (samples[index].name) {
				LogOutput(LOG_TAG_RENDERER, "\t%s time = %3.3fms count = %d\n", samples[index].name, samples[index].timeTotal / frameCount / 1000.0f, samples[index].count / frameCount);
				totalTime += samples[index].timeTotal / frameCount / 1000.0f;
			}
		}

		LogOutput(LOG_TAG_RENDERER, "\tTotal time=%3.3fms\n", totalTime);
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
#ifdef DEBUG
	uint32_t err = GfxRenderer()->GetLastError();

	if (err != NO_ERROR) {
		LogOutput(LOG_TAG_RENDERER, "%s error=0x%x\n", m_name, err);
	}
#endif
	CGfxProfiler::EndSample(m_type);
}
