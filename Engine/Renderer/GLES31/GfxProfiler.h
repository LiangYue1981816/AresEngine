#pragma once
#include "GfxRenderer.h"


class CGfxProfiler
{
public:
	typedef struct Sample {
		Sample(void)
		{
			Reset();
		}

		void Reset(void)
		{
			timeBegin = 0;
			timeEnd = 0;
			timeTotal = 0;
			timeCount = 0;
		}

		const char *name;
		GLuint timeBegin;
		GLuint timeEnd;
		GLuint timeTotal;
		GLuint timeCount;
	} Sample;

	typedef enum SampleType {
		SAMPLE_TYPE_COMMAND_BEGIN_PASS = 0,
		SAMPLE_TYPE_COMMAND_END_PASS,
		SAMPLE_TYPE_COMMAND_SET_SCISSOR,
		SAMPLE_TYPE_COMMAND_SET_VIEWPORT,
		SAMPLE_TYPE_COMMAND_SET_CULLFACE,
		SAMPLE_TYPE_COMMAND_SET_DEPTHTEST,
		SAMPLE_TYPE_COMMAND_SET_DEPTHWRITE,
		SAMPLE_TYPE_COMMAND_SET_COLORWRITE,
		SAMPLE_TYPE_COMMAND_SET_BLEND,
		SAMPLE_TYPE_COMMAND_SET_POLYGONOFFSET,
		SAMPLE_TYPE_COMMAND_BIND_CAMERA,
		SAMPLE_TYPE_COMMAND_BIND_MESH,
		SAMPLE_TYPE_COMMAND_BIND_MATERIAL,
		SAMPLE_TYPE_COMMAND_BIND_INPUTTEXTURE,
		SAMPLE_TYPE_COMMAND_CLEAR_DEPTH,
		SAMPLE_TYPE_COMMAND_CLEAR_COLOR,
		SAMPLE_TYPE_COMMAND_DRAW_INSTANCE,
		SAMPLE_TYPE_COMMAND_DRAW_INDIRECT,
		SAMPLE_TYPE_COMMAND_DRAW_ELEMENTS,

		SAMPLE_TYPE_COUNT
	} SampleType;


public:
	static void SetEnable(bool bEnable);

	static void ResetSamples(void);
	static void BeginSample(SampleType type, const char *name);
	static void EndSample(SampleType type);

	static void Log(void);

private:
	static bool bEnableProfiler;
	static Sample samples[SampleType::SAMPLE_TYPE_COUNT];
};

class CGfxProfilerSample
{
public:
	CGfxProfilerSample(CGfxProfiler::SampleType type, const char *name);
	~CGfxProfilerSample(void);


private:
	CGfxProfiler::SampleType m_type;
	const char *m_name;
};
