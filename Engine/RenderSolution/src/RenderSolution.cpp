#include "EngineHeader.h"


static const ATTRIBUTE vertexAttributes[VERTEX_ATTRIBUTE_COUNT] = {
	{ VERTEX_ATTRIBUTE_POSITION,  4, 3, 0, GFX_DATAFORMAT_VEC3_SFLOAT, "inPosition"  },
	{ VERTEX_ATTRIBUTE_NORMAL,    4, 3, 1, GFX_DATAFORMAT_VEC3_SFLOAT, "inNormal"    },
	{ VERTEX_ATTRIBUTE_BINORMAL,  4, 3, 2, GFX_DATAFORMAT_VEC3_SFLOAT, "inBinormal"  },
	{ VERTEX_ATTRIBUTE_COLOR,     4, 3, 3, GFX_DATAFORMAT_VEC3_SFLOAT, "inColor"     },
	{ VERTEX_ATTRIBUTE_TEXCOORD0, 4, 2, 4, GFX_DATAFORMAT_VEC2_SFLOAT, "inTexcoord0" },
	{ VERTEX_ATTRIBUTE_TEXCOORD1, 4, 2, 5, GFX_DATAFORMAT_VEC2_SFLOAT, "inTexcoord1" },
};

static const ATTRIBUTE instanceAttributes[INSTANCE_ATTRIBUTE_COUNT] = {
	// Transform
	{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL0, 4, 4, 8,  GFX_DATAFORMAT_VEC4_SFLOAT, "inInstanceTransformMatrixCol0" },
	{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL1, 4, 4, 9,  GFX_DATAFORMAT_VEC4_SFLOAT, "inInstanceTransformMatrixCol1" },
	{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL2, 4, 4, 10, GFX_DATAFORMAT_VEC4_SFLOAT, "inInstanceTransformMatrixCol2" },
	{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL3, 4, 4, 11, GFX_DATAFORMAT_VEC4_SFLOAT, "inInstanceTransformMatrixCol3" },

	// PointLight
	{ INSTANCE_ATTRIBUTE_POINTLIGHT_COLOR,      4, 4, 12, GFX_DATAFORMAT_VEC4_SFLOAT, "inInstancePointLightColor"       },
	{ INSTANCE_ATTRIBUTE_POINTLIGHT_ATTENUATION,4, 4, 13, GFX_DATAFORMAT_VEC4_SFLOAT, "inInstancePointLightAttenuation" },
};


CRenderSolution::CRenderSolution(GfxApi api, RenderSolution solution, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format)
	: m_pRenderer(nullptr)
	, m_pRenderSolution{ nullptr }
	, m_pCurrentRenderSolution(nullptr)
{
	SetVertexAttributes(vertexAttributes, VERTEX_ATTRIBUTE_COUNT);
	SetInstanceAttributes(instanceAttributes, INSTANCE_ATTRIBUTE_COUNT);

	switch ((int)api) {
	case GFX_API_GLES3:
		m_pRenderer = new CGLES3Renderer(hInstance, hWnd, hDC, width, height, format);
		break;

	case GFX_API_VULKAN:
		m_pRenderer = new CVKRenderer(hInstance, hWnd, hDC, width, height, format);
		break;
	}

	CRenderSolutionBase::CreateInstance();
	m_pRenderSolution[RENDER_SOLUTION_DEFAULT] = new CRenderSolutionDefault;
	m_pRenderSolution[RENDER_SOLUTION_FORWARD] = new CRenderSolutionForward;
	m_pRenderSolution[RENDER_SOLUTION_DEFERRED] = new CRenderSolutionDeferred;
	m_pRenderSolution[RENDER_SOLUTION_TILED_BASE_FORWARD] = new CRenderSolutionTiledBaseForward;
	m_pRenderSolution[RENDER_SOLUTION_TILED_BASE_DEFERRED] = new CRenderSolutionTiledBaseDeferred;

	m_pCurrentRenderSolution = m_pRenderSolution[RENDER_SOLUTION_DEFAULT];
	m_pCurrentRenderSolution->Create();
}

CRenderSolution::~CRenderSolution(void)
{
	delete m_pRenderSolution[RENDER_SOLUTION_DEFAULT];
	delete m_pRenderSolution[RENDER_SOLUTION_FORWARD];
	delete m_pRenderSolution[RENDER_SOLUTION_DEFERRED];
	delete m_pRenderSolution[RENDER_SOLUTION_TILED_BASE_FORWARD];
	delete m_pRenderSolution[RENDER_SOLUTION_TILED_BASE_DEFERRED];
	CRenderSolutionBase::DestroyInstance();

	delete m_pRenderer;
}

void CRenderSolution::SetRenderSolution(RenderSolution solution, int samples)
{
	m_pCurrentRenderSolution->Destroy();
	m_pCurrentRenderSolution = m_pRenderSolution[solution];
	m_pCurrentRenderSolution->Create(samples);
}

CRenderSolutionBase* CRenderSolution::GetRenderSolution(void) const
{
	return m_pCurrentRenderSolution;
}
