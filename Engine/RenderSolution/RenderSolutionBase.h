#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "PreHeader.h"


typedef enum RenderSolution {
	RENDER_SOLUTION_DEFAULT = 0,
	RENDER_SOLUTION_DEFERRED,
	RENDER_SOLUTION_FORWARD,
	RENDER_SOLUTION_FORWARD_PLUS,
	RENDER_SOLUTION_COUNT
} RenderSolution;


class CRenderSolutionBase
{
	friend class CEngine;


protected:
	CRenderSolutionBase(void);
	virtual ~CRenderSolutionBase(void);


protected:
	virtual void CreateFrameBuffer(void) = 0;
	virtual void DestroyFrameBuffer(void) = 0;

	virtual void CreateFrameBufferMSAA(int samples) = 0;
	virtual void DestroyFrameBufferMSAA(void) = 0;

public:
	virtual void SetEnableMSAA(bool bEnable, int samples = 4) = 0;

public:
	virtual void Render(int indexQueue, float deltaTime) = 0;
	virtual void Present(int indexQueue) = 0;
	virtual void Clearup(int indexQueue) = 0;
};
