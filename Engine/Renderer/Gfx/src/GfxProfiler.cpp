#include "GfxHeader.h"


bool CGfxProfiler::bEnableProfiler = false;

size_t CGfxProfiler::textureDataSize = 0;
size_t CGfxProfiler::uniformBufferSize = 0;
size_t CGfxProfiler::storageBufferSize = 0;
size_t CGfxProfiler::vertexBufferSize = 0;
size_t CGfxProfiler::indexBufferSize = 0;
size_t CGfxProfiler::instanceBufferSize = 0;
size_t CGfxProfiler::indirectBufferSize = 0;
size_t CGfxProfiler::transferBufferSize = 0;

CGfxProfiler::Sample CGfxProfiler::samples[SampleType::SAMPLE_TYPE_COUNT] = {
	"CommandBeginRecord",              //SAMPLE_TYPE_COMMAND_BEGIN_RECORD = 0,
	"CommandEndRecord",                //SAMPLE_TYPE_COMMAND_END_RECORD,
	"CommandSetImageLayout",           //SAMPLE_TYPE_COMMAND_SET_IMAGELAYOUT,
	"CommandSetBufferBarrier",         //SAMPLE_TYPE_COMMAND_SET_BUFFERBARRIER,
	"CommandBeginRenderPass",          //SAMPLE_TYPE_COMMAND_BEGIN_RENDERPASS,
	"CommandNextSubPass",              //SAMPLE_TYPE_COMMAND_NEXT_SUBPASS,
	"CommandEndRenderPass",            //SAMPLE_TYPE_COMMAND_END_RENDERPASS,
	"CommandReslove",                  //SAMPLE_TYPE_COMMAND_RESOLVE,
	"CommandInvalidateFrameBuffer",    //SAMPLE_TYPE_COMMAND_INVALIDATE_FRAMEBUFFER,
	"CommandBindFrameBuffer",          //SAMPLE_TYPE_COMMAND_BIND_FRAMEBUFFER,
	"CommandBindPipelineCompute",      //SAMPLE_TYPE_COMMAND_BIND_PIPELINECOMPUTE,
	"CommandBindPipelineGraphics",     //SAMPLE_TYPE_COMMAND_BIND_PIPELINEGRAPHICS,
	"CommandBindDescriptorSet",        //SAMPLE_TYPE_COMMAND_BIND_DESCRIPTORSET,
	"CommandBindIndexBuffer",          //SAMPLE_TYPE_COMMAND_BIND_INDEXBUFFER,
	"CommandBindVertexBuffer",         //SAMPLE_TYPE_COMMAND_BIND_VERTEXBUFFER,
	"CommandBindInstanceBuffer",       //SAMPLE_TYPE_COMMAND_BIND_INSTANCEBUFFER,
	"CommandUniform1i",                //SAMPLE_TYPE_COMMAND_UNIFORM1I,
	"CommandUniform2i",                //SAMPLE_TYPE_COMMAND_UNIFORM2I,
	"CommandUniform3i",                //SAMPLE_TYPE_COMMAND_UNIFORM3I,
	"CommandUniform4i",                //SAMPLE_TYPE_COMMAND_UNIFORM4I,
	"CommandUniform1f",                //SAMPLE_TYPE_COMMAND_UNIFORM1F,
	"CommandUniform2f",                //SAMPLE_TYPE_COMMAND_UNIFORM2F,
	"CommandUniform3f",                //SAMPLE_TYPE_COMMAND_UNIFORM3F,
	"CommandUniform4f",                //SAMPLE_TYPE_COMMAND_UNIFORM4F,
	"CommandUniform1iv",               //SAMPLE_TYPE_COMMAND_UNIFORM1IV,
	"CommandUniform2iv",               //SAMPLE_TYPE_COMMAND_UNIFORM2IV,
	"CommandUniform3iv",               //SAMPLE_TYPE_COMMAND_UNIFORM3IV,
	"CommandUniform4iv",               //SAMPLE_TYPE_COMMAND_UNIFORM4IV,
	"CommandUniform1fv",               //SAMPLE_TYPE_COMMAND_UNIFORM1FV,
	"CommandUniform2fv",               //SAMPLE_TYPE_COMMAND_UNIFORM2FV,
	"CommandUniform3fv",               //SAMPLE_TYPE_COMMAND_UNIFORM3FV,
	"CommandUniform4fv",               //SAMPLE_TYPE_COMMAND_UNIFORM4FV,
	"CommandUniformMatrix2fv",         //SAMPLE_TYPE_COMMAND_UNIFORMMATRIX2FV,
	"CommandUniformMatrix3fv",         //SAMPLE_TYPE_COMMAND_UNIFORMMATRIX3FV,
	"CommandUniformMatrix4fv",         //SAMPLE_TYPE_COMMAND_UNIFORMMATRIX4FV,
	"CommandSetScissor",               //SAMPLE_TYPE_COMMAND_SET_SCISSOR,
	"CommandSetViewport",              //SAMPLE_TYPE_COMMAND_SET_VIEWPORT,
	"CommandClearDepth",               //SAMPLE_TYPE_COMMAND_CLEAR_DEPTH,
	"CommandClearColor",               //SAMPLE_TYPE_COMMAND_CLEAR_COLOR,
	"CommandDispatch",                 //SAMPLE_TYPE_COMMAND_DISPATCH,
	"CommandDrawInstance",             //SAMPLE_TYPE_COMMAND_DRAW_INSTANCE,
	"CommandDrawIndirect",             //SAMPLE_TYPE_COMMAND_DRAW_INDIRECT,
	"CommandExecute",                  //SAMPLE_TYPE_COMMAND_EXECUTE,
};

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

void CGfxProfiler::IncStorageBufferSize(size_t size)
{
	storageBufferSize += size;
}

void CGfxProfiler::DecStorageBufferSize(size_t size)
{
	storageBufferSize -= size;
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

void CGfxProfiler::IncIndirectBufferSize(size_t size)
{
	indirectBufferSize += size;
}

void CGfxProfiler::DecIndirectBufferSize(size_t size)
{
	indirectBufferSize -= size;
}

void CGfxProfiler::IncTransferBufferSize(size_t size)
{
	transferBufferSize += size;
}

void CGfxProfiler::DecTransferBufferSize(size_t size)
{
	transferBufferSize -= size;
}

void CGfxProfiler::ResetSamples(void)
{
	for (int indexSample = 0; indexSample < SampleType::SAMPLE_TYPE_COUNT; indexSample++) {
		samples[indexSample].Reset();
	}
}

void CGfxProfiler::BeginSample(SampleType type)
{
	if (bEnableProfiler) {
		samples[type].Begin(Tick());
	}
}

void CGfxProfiler::EndSample(SampleType type)
{
	if (bEnableProfiler) {
		samples[type].End(Tick());
	}
}

const char* CGfxProfiler::GetSampleName(SampleType type)
{
	return samples[type].name;
}

void CGfxProfiler::LogGfxMemory(void)
{
	LogOutput(LOG_INFO, LOG_TAG_RENDERER, "GfxMemory\n");
	LogOutput(LOG_INFO, LOG_TAG_RENDERER, "\tTextureData = %dKB\n", textureDataSize / 1024);
	LogOutput(LOG_INFO, LOG_TAG_RENDERER, "\tUniformBuffer = %dKB\n", uniformBufferSize / 1024);
	LogOutput(LOG_INFO, LOG_TAG_RENDERER, "\tStorageBuffer = %dKB\n", storageBufferSize / 1024);
	LogOutput(LOG_INFO, LOG_TAG_RENDERER, "\tVertexBuffer = %dKB\n", vertexBufferSize / 1024);
	LogOutput(LOG_INFO, LOG_TAG_RENDERER, "\tIndexBuffer = %dKB\n", indexBufferSize / 1024);
	LogOutput(LOG_INFO, LOG_TAG_RENDERER, "\tInstanceBuffer = %dKB\n", instanceBufferSize / 1024);
	LogOutput(LOG_INFO, LOG_TAG_RENDERER, "\tIndirectBuffer = %dKB\n", indirectBufferSize / 1024);
	LogOutput(LOG_INFO, LOG_TAG_RENDERER, "\tTransferBuffer = %dKB\n", transferBufferSize / 1024);
	LogOutput(LOG_INFO, LOG_TAG_RENDERER, "\tTotal memory = %dKB\n", (textureDataSize + uniformBufferSize + storageBufferSize + vertexBufferSize + indexBufferSize + instanceBufferSize + indirectBufferSize + transferBufferSize) / 1024);
}

void CGfxProfiler::LogProfiler(int frameCount)
{
	if (bEnableProfiler) {
		LogOutput(LOG_INFO, LOG_TAG_RENDERER, "GfxProfiler\n");

		float totalTime = 0.0f;
		for (int indexSample = 0; indexSample < SampleType::SAMPLE_TYPE_COUNT; indexSample++) {
			if (samples[indexSample].count) {
				LogOutput(LOG_INFO, LOG_TAG_RENDERER, "\t%s time = %3.3fms count = %d\n", samples[indexSample].name, samples[indexSample].timeTotal / frameCount / 1000.0f, samples[indexSample].count / frameCount);
				totalTime += samples[indexSample].timeTotal / frameCount / 1000.0f;
			}
		}

		LogOutput(LOG_INFO, LOG_TAG_RENDERER, "\tTotal time=%3.3fms\n", totalTime);
	}
}


CGfxProfilerSample::CGfxProfilerSample(CGfxProfiler::SampleType type)
	: m_type(type)
{
	CGfxProfiler::BeginSample(m_type);
}

CGfxProfilerSample::~CGfxProfilerSample(void)
{
#ifdef DEBUG
	uint32_t err = GfxRenderer()->GetLastError();

	if (err != NO_ERROR) {
		LogOutput(LOG_ERROR, LOG_TAG_RENDERER, "%s error=0x%x\n", CGfxProfiler::GetSampleName(m_type), err);
	}
#endif
	CGfxProfiler::EndSample(m_type);
}
