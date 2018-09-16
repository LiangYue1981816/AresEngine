#pragma once
#include <gl31.h>
#include "GfxCommandBuffer.h"


class CGfxCommandExecute : public CGfxCommandBase
{
public:
	CGfxCommandExecute(CGfxCommandBuffer *pCommandBuffer)
		: m_pCommandBuffer(pCommandBuffer)
	{

	}
	virtual ~CGfxCommandExecute(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		if (m_pCommandBuffer) {
			m_pCommandBuffer->Execute();
		}
	}


private:
	CGfxCommandBuffer *m_pCommandBuffer;
};
