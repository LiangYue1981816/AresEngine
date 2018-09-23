#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxProfiler.h"


bool CGfxProfiler::bEnableProfiler = false;
CGfxProfiler::Sample CGfxProfiler::samples[SampleType::SAMPLE_TYPE_COUNT];

void CGfxProfiler::SetEnable(bool bEnable)
{
	bEnableProfiler = bEnable;
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

void CGfxProfiler::Log(void)
{
	if (bEnableProfiler) {
		LogOutput("GfxRenderer", "Profiler\n");

		float totalTime = 0.0f;
		for (int index = 0; index < SampleType::SAMPLE_TYPE_COUNT; index++) {
			if (samples[index].name) {
				LogOutput("GfxRenderer", "\t%s time=%fms count=%d\n", samples[index].name, samples[index].timeTotal / 1000.0f, samples[index].timeCount);
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
