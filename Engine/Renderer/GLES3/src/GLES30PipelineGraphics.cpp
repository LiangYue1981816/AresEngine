#if GLES_VER == 300

#include "GLES3Renderer.h"


CGLES3PipelineGraphics::CGLES3PipelineGraphics(uint32_t name)
	: CGfxPipelineGraphics(name)
	, m_name(name)
{

}

CGLES3PipelineGraphics::~CGLES3PipelineGraphics(void)
{
	Destroy();
}

uint32_t CGLES3PipelineGraphics::GetName(void) const
{
	return m_name;
}

bool CGLES3PipelineGraphics::Create(const CGfxRenderPass *pRenderPass, const CGfxShader *pVertexShader, const CGfxShader *pFragmentShader, const PipelineState &state)
{
	if (pRenderPass == nullptr) {
		return false;
	}

	if (pVertexShader == nullptr) {
		return false;
	}

	if (pVertexShader->IsValid() == false) {
		return false;
	}

	if (pVertexShader->GetKind() != vertex_shader) {
		return false;
	}

	if (pFragmentShader == nullptr) {
		return false;
	}

	if (pFragmentShader->IsValid() == false) {
		return false;
	}

	if (pFragmentShader->GetKind() != fragment_shader) {
		return false;
	}

	Destroy();
	{
		do {
			m_state = state;
			m_pShaders[vertex_shader] = (CGLES3Shader *)pVertexShader;
			m_pShaders[fragment_shader] = (CGLES3Shader *)pFragmentShader;

			m_program = glCreateProgram();
			glAttachShader(m_program, m_pShaders[vertex_shader]->GetShader());
			glAttachShader(m_program, m_pShaders[fragment_shader]->GetShader());
			glLinkProgram(m_program);

			GLint success;
			glGetProgramiv(m_program, GL_LINK_STATUS, &success);

			if (success == GL_FALSE) {
				GLsizei length = 0;
				char szError[128 * 1024] = { 0 };

				glGetProgramInfoLog(m_program, sizeof(szError), &length, szError);

				LogOutput(nullptr, "Program Link Error:\n");
				LogOutput(nullptr, "%s\n", szError);

				break;
			}

			if (CreateLayouts() == false) {
				break;
			}

			return true;
		} while (false);
	}
	Destroy();
	return true;
}

void CGLES3PipelineGraphics::Destroy(void)
{
	if (m_program) {
		glDeleteProgram(m_program);
	}

	m_program = 0;
	m_pShaders[vertex_shader] = nullptr;
	m_pShaders[fragment_shader] = nullptr;
}

bool CGLES3PipelineGraphics::IsTextureValid(uint32_t name) const
{
	return CGLES3Pipeline::IsTextureValid(name);
}

bool CGLES3PipelineGraphics::IsUniformValid(uint32_t name) const
{
	return CGLES3Pipeline::IsUniformValid(name);
}

bool CGLES3PipelineGraphics::IsUniformBlockValid(uint32_t name) const
{
	return CGLES3Pipeline::IsUniformBlockValid(name);
}

void CGLES3PipelineGraphics::Bind(void)
{
	GLBindState(&m_state);
	GLUseProgram(m_program);
}

#endif
