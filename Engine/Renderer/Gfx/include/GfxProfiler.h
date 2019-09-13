#pragma once
#include "PreHeader.h"


class CALL_API CGfxProfiler
{
public:
	typedef struct Sample {
		Sample(void)
		{
			name = nullptr;
			Reset();
		}

		void Reset(void)
		{
			timeBegin = 0;
			timeEnd = 0;
			timeTotal = 0;
			count = 0;
		}

		const char* name;
		uint32_t timeBegin;
		uint32_t timeEnd;
		uint32_t timeTotal;
		uint32_t count;
	} Sample;

	typedef enum SampleType {
		SAMPLE_TYPE_COMMAND_BEGIN_RECORD = 0,
		SAMPLE_TYPE_COMMAND_END_RECORD,
		SAMPLE_TYPE_COMMAND_SET_IMAGELAYOUT,
		SAMPLE_TYPE_COMMAND_BEGIN_RENDERPASS,
		SAMPLE_TYPE_COMMAND_NEXT_SUBPASS,
		SAMPLE_TYPE_COMMAND_END_RENDERPASS,
		SAMPLE_TYPE_COMMAND_RESOLVE,
		SAMPLE_TYPE_COMMAND_INVALIDATE_FRAMEBUFFER,
		SAMPLE_TYPE_COMMAND_BIND_FRAMEBUFFER,
		SAMPLE_TYPE_COMMAND_BIND_PIPELINECOMPUTE,
		SAMPLE_TYPE_COMMAND_BIND_PIPELINEGRAPHICS,
		SAMPLE_TYPE_COMMAND_BIND_DESCRIPTORSET,
		SAMPLE_TYPE_COMMAND_BIND_MESH,
		SAMPLE_TYPE_COMMAND_BIND_MESHDRAW,
		SAMPLE_TYPE_COMMAND_UNIFORM1I,
		SAMPLE_TYPE_COMMAND_UNIFORM2I,
		SAMPLE_TYPE_COMMAND_UNIFORM3I,
		SAMPLE_TYPE_COMMAND_UNIFORM4I,
		SAMPLE_TYPE_COMMAND_UNIFORM1F,
		SAMPLE_TYPE_COMMAND_UNIFORM2F,
		SAMPLE_TYPE_COMMAND_UNIFORM3F,
		SAMPLE_TYPE_COMMAND_UNIFORM4F,
		SAMPLE_TYPE_COMMAND_UNIFORM1IV,
		SAMPLE_TYPE_COMMAND_UNIFORM2IV,
		SAMPLE_TYPE_COMMAND_UNIFORM3IV,
		SAMPLE_TYPE_COMMAND_UNIFORM4IV,
		SAMPLE_TYPE_COMMAND_UNIFORM1FV,
		SAMPLE_TYPE_COMMAND_UNIFORM2FV,
		SAMPLE_TYPE_COMMAND_UNIFORM3FV,
		SAMPLE_TYPE_COMMAND_UNIFORM4FV,
		SAMPLE_TYPE_COMMAND_UNIFORMMATRIX2FV,
		SAMPLE_TYPE_COMMAND_UNIFORMMATRIX3FV,
		SAMPLE_TYPE_COMMAND_UNIFORMMATRIX4FV,
		SAMPLE_TYPE_COMMAND_SET_SCISSOR,
		SAMPLE_TYPE_COMMAND_SET_VIEWPORT,
		SAMPLE_TYPE_COMMAND_CLEAR_DEPTH,
		SAMPLE_TYPE_COMMAND_CLEAR_COLOR,
		SAMPLE_TYPE_COMMAND_DRAW_INSTANCE,
		SAMPLE_TYPE_COMMAND_DRAW_INDIRECT,
		SAMPLE_TYPE_COMMAND_UPDATE_INSTANCEBUFFER,
		SAMPLE_TYPE_COMMAND_EXECUTE,
		SAMPLE_TYPE_COUNT
	} SampleType;


public:
	static void SetEnableProfiler(bool bEnable);

	static void IncTextureDataSize(size_t size);
	static void DecTextureDataSize(size_t size);

	static void IncUniformBufferSize(size_t size);
	static void DecUniformBufferSize(size_t size);

	static void IncVertexBufferSize(size_t size);
	static void DecVertexBufferSize(size_t size);

	static void IncIndexBufferSize(size_t size);
	static void DecIndexBufferSize(size_t size);

	static void IncInstanceBufferSize(size_t size);
	static void DecInstanceBufferSize(size_t size);

	static void IncIndirectBufferSize(size_t size);
	static void DecIndirectBufferSize(size_t size);

	static void IncTransferBufferSize(size_t size);
	static void DecTransferBufferSize(size_t size);

	static void ResetSamples(void);
	static void BeginSample(SampleType type, const char* name);
	static void EndSample(SampleType type);

	static void LogGfxMemory(void);
	static void LogProfiler(int frameCount);

private:
	static bool bEnableProfiler;

	static size_t textureDataSize;
	static size_t uniformBufferSize;
	static size_t vertexBufferSize;
	static size_t indexBufferSize;
	static size_t instanceBufferSize;
	static size_t indirectBufferSize;
	static size_t transferBufferSize;

	static Sample samples[SampleType::SAMPLE_TYPE_COUNT];
};

class CALL_API CGfxProfilerSample
{
public:
	CGfxProfilerSample(CGfxProfiler::SampleType type, const char* name);
	~CGfxProfilerSample(void);


private:
	CGfxProfiler::SampleType m_type;
	const char* m_name;
};
