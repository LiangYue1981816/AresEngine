#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxPipelineGraphics.h"


CGfxPipelineGraphics::CGfxPipelineGraphics(uint32_t name)
	: CGfxPipelineBase(name)
{
	m_state.bEnableCullFace = GL_TRUE;
	m_state.bEnableDepthTest = GL_TRUE;
	m_state.bEnableDepthWrite = GL_TRUE;
	m_state.bEnableColorWrite[0] = GL_TRUE;
	m_state.bEnableColorWrite[1] = GL_TRUE;
	m_state.bEnableColorWrite[2] = GL_TRUE;
	m_state.bEnableColorWrite[3] = GL_TRUE;
	m_state.bEnableBlend = GL_FALSE;
	m_state.bEnablePolygonOffset = GL_FALSE;
	m_state.cullFace = GL_BACK;
	m_state.frontFace = GL_CCW;
	m_state.depthFunc = GL_LESS;
	m_state.srcBlendFactor = GL_SRC_ALPHA;
	m_state.dstBlendFactor = GL_ONE_MINUS_SRC_ALPHA;
	m_state.polygonOffsetFactor = 0.0f;
	m_state.polygonOffsetUnits = 0.0f;
}

CGfxPipelineGraphics::~CGfxPipelineGraphics(void)
{

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
	CreateLocations(pVertexShader);
	CreateLocations(pFragmentShader);
	CreateState(state);

	return true;
}

bool CGfxPipelineGraphics::CreateProgram(const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader)
{
	glBindProgramPipeline(m_pipeline);
	{
		glUseProgramStages(m_pipeline, glGetShaderKind(shaderc_vertex_shader), pVertexShader->GetProgram());
		glUseProgramStages(m_pipeline, glGetShaderKind(shaderc_fragment_shader), pFragmentShader->GetProgram());
	}
	glBindProgramPipeline(0);

	return true;
}

bool CGfxPipelineGraphics::CreateState(const GLstate &state)
{
	memcpy(&m_state, &state, sizeof(state));
	return true;
}

void CGfxPipelineGraphics::SetEnableCullFace(bool bEnable, GLenum cullFace, GLenum frontFace)
{
	m_state.bEnableCullFace = bEnable ? GL_TRUE : GL_FALSE;
	m_state.cullFace = cullFace;
	m_state.frontFace = frontFace;
}

void CGfxPipelineGraphics::SetEnableDepthTest(bool bEnable, GLenum depthFunc)
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

void CGfxPipelineGraphics::SetEnableBlend(bool bEnable, GLenum srcFactor, GLenum dstFactor)
{
	m_state.bEnableBlend = bEnable ? GL_TRUE : GL_FALSE;
	m_state.srcBlendFactor = srcFactor;
	m_state.dstBlendFactor = dstFactor;
}

void CGfxPipelineGraphics::SetEnablePolygonOffset(bool bEnable, GLfloat factor, GLfloat units)
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
	if (m_state.bEnableCullFace) {
		glEnable(GL_CULL_FACE);
	}
	else {
		glDisable(GL_CULL_FACE);
	}

	if (m_state.bEnableDepthTest) {
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}

	if (m_state.bEnableDepthWrite) {
		glDepthMask(GL_TRUE);
	}
	else {
		glDepthMask(GL_FALSE);
	}

	if (m_state.bEnableBlend) {
		glEnable(GL_BLEND);
	}
	else {
		glDisable(GL_BLEND);
	}

	if (m_state.bEnablePolygonOffset) {
		glEnable(GL_POLYGON_OFFSET_FILL);
	}
	else {
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	glCullFace(m_state.cullFace);
	glFrontFace(m_state.frontFace);
	glDepthFunc(m_state.depthFunc);
	glBlendFunc(m_state.srcBlendFactor, m_state.dstBlendFactor);
	glPolygonOffset(m_state.polygonOffsetFactor, m_state.polygonOffsetUnits);
	glColorMask(m_state.bEnableColorWrite[0], m_state.bEnableColorWrite[1], m_state.bEnableColorWrite[2], m_state.bEnableColorWrite[3]);

	glBindProgramPipeline(m_pipeline);
}
