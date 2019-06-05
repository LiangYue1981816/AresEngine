#pragma once
#include "PreHeader.h"

#include "RenderSolutionBase.h"
#include "RenderSolutionDefault.h"
#include "RenderSolutionDeferred.h"
#include "RenderSolutionForward.h"
#include "RenderSolutionTiledBaseForward.h"
#include "RenderSolutionTiledBaseDeferred.h"


typedef enum RenderSolution {
	RENDER_SOLUTION_DEFAULT = 0,
	RENDER_SOLUTION_FORWARD,
	RENDER_SOLUTION_DEFERRED,
	RENDER_SOLUTION_TILED_BASE_FORWARD,
	RENDER_SOLUTION_TILED_BASE_DEFERRED,
	RENDER_SOLUTION_COUNT
} RenderSolution;


class CALL_API CRenderSolution
{
	friend class CEngine;


private:
	CRenderSolution(GfxApi api, RenderSolution solution, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format);
	virtual ~CRenderSolution(void);


public:
	void SetRenderSolution(RenderSolution solution, int samples = 1);
	CRenderSolutionBase* GetRenderSolution(void) const;


private:
	CGfxRenderer* m_pRenderer;

	CRenderSolutionBase* m_pRenderSolution[RENDER_SOLUTION_COUNT];
	CRenderSolutionBase* m_pCurrentRenderSolution;
};
