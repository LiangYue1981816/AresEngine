#pragma once
#include "TaskGraph.h"


class CTaskCommandBuffer : public CTask
{
public:
	CTaskCommandBuffer(void)
	{

	}
	virtual ~CTaskCommandBuffer(void)
	{

	}


public:
	virtual void TaskFunc(void *pParams)
	{
		if (CGfxCamera *pCamera = (CGfxCamera *)pParams) {
			pCamera->CmdDraw();
		}
	}
};
