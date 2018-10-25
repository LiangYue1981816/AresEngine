#pragma once
#include "Engine.h"
#include "TaskGraph.h"


class CTaskCommandBuffer : public CTask
{
public:
	CTaskCommandBuffer(void)
		: m_pCommandBuffer(nullptr)

		, m_indexThread(0)
		, m_indexQueue(0)
		, m_namePass(0)
	{

	}
	virtual ~CTaskCommandBuffer(void)
	{

	}


public:
	void SetParams(CGfxCommandBuffer *pCommandBuffer, int indexThread, int indexQueue, uint32_t namePass)
	{
		m_pCommandBuffer = pCommandBuffer;
		m_indexThread = indexThread;
		m_indexQueue = indexQueue;
		m_namePass = namePass;
	}

	virtual void TaskFunc(void *pParams)
	{
		if (CGfxCamera *pCamera = (CGfxCamera *)pParams) {
			pCamera->CmdDraw(m_pCommandBuffer, m_indexThread, m_indexQueue, m_namePass);
		}
	}


private:
	CGfxCommandBuffer *m_pCommandBuffer;

private:
	int m_indexThread;
	int m_indexQueue;
	uint32_t m_namePass;
};
