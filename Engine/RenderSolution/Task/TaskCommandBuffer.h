#pragma once
#include "Engine.h"
#include "TaskGraph.h"


class CTaskCommandBuffer : public CTask
{
public:
	CTaskCommandBuffer(void)
		: m_indexThread(0)
		, m_namePass(0)
	{

	}
	virtual ~CTaskCommandBuffer(void)
	{

	}


public:
	void SetParams(int indexThread, uint32_t namePass)
	{
		m_indexThread = indexThread;
		m_namePass = namePass;
	}

	virtual void TaskFunc(void *pParams)
	{
		if (CGfxCamera *pCamera = (CGfxCamera *)pParams) {
			pCamera->CmdDraw(m_indexThread, m_namePass);
		}
	}


private:
	int m_indexThread;
	uint32_t m_namePass;
};
