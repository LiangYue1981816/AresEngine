#include "EngineHeader.h"
#include "RenderHeader.h"


CStorageHistogram::CStorageHistogram(void)
	: m_offset(0)
{
	m_ptrHistogramBuffer = GfxRenderer()->NewStorageBuffer(CGfxSwapChain::SWAPCHAIN_FRAME_COUNT * GfxRenderer()->AlignStorageBufferOffset(HISTOGRAM_SIZE * sizeof(int)));
}

CStorageHistogram::~CStorageHistogram(void)
{

}

uint32_t CStorageHistogram::GetStorageBufferSize(void) const
{
	return HISTOGRAM_SIZE * sizeof(int);
}

uint32_t CStorageHistogram::GetStorageBufferOffset(void) const
{
	return m_offset;
}

CGfxStorageBufferPtr CStorageHistogram::GetStorageBuffer(void) const
{
	return m_ptrHistogramBuffer;
}

void CStorageHistogram::Clear(void)
{
	static const int data[HISTOGRAM_SIZE] = { 0 };
	m_offset = GfxRenderer()->GetSwapChain()->GetFrameIndex() * GfxRenderer()->AlignStorageBufferOffset(HISTOGRAM_SIZE * sizeof(int));
	m_ptrHistogramBuffer->BufferData(m_offset, sizeof(data), data);
}
