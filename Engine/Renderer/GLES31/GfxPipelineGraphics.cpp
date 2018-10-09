#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxPipelineGraphics.h"


CGfxPipelineGraphics::CGfxPipelineGraphics(uint32_t name)
	: CGfxPipelineBase(name)
{
	GLInitState(&m_state);
}

CGfxPipelineGraphics::~CGfxPipelineGraphics(void)
{
	Destroy();
}

bool CGfxPipelineGraphics::Create(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const GLstate &state)
{
	if (pVertexShader == NULL) {
		return false;
	}

	if (pVertexShader->IsValid() == false) {
		return false;
	}

	if (pVertexShader->GetKind() != shaderc_vertex_shader) {
		return false;
	}

	if (pFragmentShader == NULL) {
		return false;
	}

	if (pFragmentShader->IsValid() == false) {
		return false;
	}

	if (pFragmentShader->GetKind() != shaderc_fragment_shader) {
		return false;
	}

	Destroy();
	CreateProgram(pVertexShader, pFragmentShader);
	CreateState(state);

	return true;
}

void CGfxPipelineGraphics::CreateState(const GLstate &state)
{
	memcpy(&m_state, &state, sizeof(state));
}

void CGfxPipelineGraphics::CreateProgram(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader)
{
	m_pShaders[shaderc_vertex_shader] = (CGfxShader *)pVertexShader;
	m_pShaders[shaderc_fragment_shader] = (CGfxShader *)pFragmentShader;

	glUseProgramStages(m_pipeline, glGetProgramStage(shaderc_vertex_shader), m_pShaders[shaderc_vertex_shader]->GetProgram());
	glUseProgramStages(m_pipeline, glGetProgramStage(shaderc_fragment_shader), m_pShaders[shaderc_fragment_shader]->GetProgram());
}

void CGfxPipelineGraphics::Destroy(void)
{
	m_pShaders[shaderc_vertex_shader] = NULL;
	m_pShaders[shaderc_fragment_shader] = NULL;

	glUseProgramStages(m_pipeline, glGetProgramStage(shaderc_vertex_shader), 0);
	glUseProgramStages(m_pipeline, glGetProgramStage(shaderc_fragment_shader), 0);
}

void CGfxPipelineGraphics::SetEnableCullFace(bool bEnable, uint32_t cullFace, uint32_t frontFace)
{
	m_state.bEnableCullFace = bEnable ? GL_TRUE : GL_FALSE;
	m_state.cullFace = cullFace;
	m_state.frontFace = frontFace;
}

void CGfxPipelineGraphics::SetEnableDepthTest(bool bEnable, uint32_t depthFunc)
{
	m_state.bEnableDepthTest = bEnable ? GL_TRUE : GL_FALSE;
	m_state.depthFunc = depthFunc;
}

void CGfxPipelineGraphics::SetEnableDepthWrite(bool bEnable)
{
	m_state.bEnableDepthWrite = bEnable ? GL_TRUE : GL_FALSE;
}

void CGfxPipelineGraphics::SetEnableColorWrite(bool bEnableRed, bool bEnableGreen, bool bEnableBlue, bool bEnableAlpha)
{
	m_state.bEnableColorWrite[0] = bEnableRed ? GL_TRUE : GL_FALSE;
	m_state.bEnableColorWrite[1] = bEnableGreen ? GL_TRUE : GL_FALSE;
	m_state.bEnableColorWrite[2] = bEnableBlue ? GL_TRUE : GL_FALSE;
	m_state.bEnableColorWrite[3] = bEnableAlpha ? GL_TRUE : GL_FALSE;
}

void CGfxPipelineGraphics::SetEnableBlend(bool bEnable, uint32_t srcFactor, uint32_t dstFactor)
{
	m_state.bEnableBlend = bEnable ? GL_TRUE : GL_FALSE;
	m_state.srcBlendFactor = srcFactor;
	m_state.dstBlendFactor = dstFactor;
}

void CGfxPipelineGraphics::SetEnablePolygonOffset(bool bEnable, float factor, float units)
{
	m_state.bEnablePolygonOffset = bEnable ? GL_TRUE : GL_FALSE;
	m_state.polygonOffsetFactor = factor;
	m_state.polygonOffsetUnits = units;
}

bool CGfxPipelineGraphics::IsEnableBlend(void) const
{
	return m_state.bEnableBlend == GL_TRUE;
}

void CGfxPipelineGraphics::BindPipeline(void) const
{
	GLBindState(&m_state);
	GLBindProgramPipeline(m_pipeline);
}
