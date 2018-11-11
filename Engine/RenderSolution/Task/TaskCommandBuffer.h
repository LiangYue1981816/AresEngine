#pragma once
#include "Engine.h"
#include "TaskGraph.h"


class CTaskCommandBuffer : public CTask
{
public:
	CTaskCommandBuffer(void)
		: m_indexThread(0)
		, m_indexQueue(0)
		, m_namePass(0)
	{

	}
	virtual ~CTaskCommandBuffer(void)
	{

	}


public:
	void SetParams(CGfxCommandBufferPtr &ptrCommandBuffer, int indexThread, int indexQueue, uint32_t namePass)
	{
		m_ptrCommandBuffer = ptrCommandBuffer;
		m_indexThread = indexThread;
		m_indexQueue = indexQueue;
		m_namePass = namePass;
	}

	virtual void TaskFunc(void *pParams)
	{
		if (CGfxCamera *pCamera = (CGfxCamera *)pParams) {
			pCamera->CmdDraw(m_ptrCommandBuffer, m_indexThread, m_indexQueue, m_namePass);
		}
	}


private:
	CGfxCommandBufferPtr m_ptrCommandBuffer;

private:
	int m_indexThread;
	int m_indexQueue;
	uint32_t m_namePass;
};
