#pragma once
#include "PreHeader.h"

#include "RenderSolutionBase.h"
#include "RenderSolutionDefault.h"
#include "RenderSolutionDeferred.h"
#include "RenderSolutionForward.h"
#include "RenderSolutionTiledBaseForward.h"
#include "RenderSolutionTiledBaseDeferred.h"


#define VERTEX_ATTRIBUTE_POSITION                          0x00000001
#define VERTEX_ATTRIBUTE_NORMAL                            0x00000002
#define VERTEX_ATTRIBUTE_BINORMAL                          0x00000004
#define VERTEX_ATTRIBUTE_COLOR                             0x00000008
#define VERTEX_ATTRIBUTE_TEXCOORD0                         0x00000010
#define VERTEX_ATTRIBUTE_TEXCOORD1                         0x00000020
#define VERTEX_ATTRIBUTE_COUNT                             6

#define INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL0           0x00000001
#define INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL1           0x00000002
#define INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL2           0x00000004
#define INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL3           0x00000008
#define INSTANCE_ATTRIBUTE_POINTLIGHT_COLOR                0x00000010
#define INSTANCE_ATTRIBUTE_POINTLIGHT_ATTENUATION          0x00000020
#define INSTANCE_ATTRIBUTE_COUNT                           6

#define INSTANCE_ATTRIBUTE_TRANSFORM (INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL0 | INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL1 | INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL2 | INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL3)
#define INSTANCE_FORMAT_TRANSFORM INSTANCE_ATTRIBUTE_TRANSFORM


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
