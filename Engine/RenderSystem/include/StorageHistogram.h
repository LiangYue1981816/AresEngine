#pragma once
#include "PreHeader.h"


class CStorageHistogram
{
public:
	CStorageHistogram(void);
	virtual ~CStorageHistogram(void);


public:
	uint32_t GetStorageBufferSize(void) const;
	uint32_t GetStorageBufferOffset(void) const;
	CGfxStorageBufferPtr GetStorageBuffer(void) const;

public:
	void Clear(void);


private:
	uint32_t m_offset;

private:
	CGfxStorageBufferPtr m_ptrHistogramBuffer;
};
