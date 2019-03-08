#include "GLES3Renderer.h"


CGLES3PipelineCompute::CGLES3PipelineCompute(uint32_t name)
	: CGfxPipelineCompute(name)
	, m_name(name)
{

}

CGLES3PipelineCompute::~CGLES3PipelineCompute(void)
{
	Destroy();
}

uint32_t CGLES3PipelineCompute::GetName(void) const
{
	return m_name;
}

HANDLE CGLES3PipelineCompute::GetPipeline(void) const
{
	return (HANDLE)m_program;
}

bool CGLES3PipelineCompute::Create(const CGfxShader *pComputeShader)
{
	if (pComputeShader == nullptr) {
		return false;
	}

	if (pComputeShader->IsValid() == false) {
		return false;
	}

	if (pComputeShader->GetKind() != compute_shader) {
		return false;
	}

	Destroy();
	{
		do {
			m_pShaders[compute_shader] = (CGLES3Shader *)pComputeShader;

			m_program = glCreateProgram();
			glAttachShader(m_program, (uint32_t)m_pShaders[compute_shader]->GetShader());
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

void CGLES3PipelineCompute::Destroy(void)
{
	if (m_program) {
		glDeleteProgram(m_program);
	}

	m_program = 0;
	m_pShaders[compute_shader] = nullptr;

	m_uniformLocations.clear();
	m_uniformBlockBindings.clear();
	m_sampledImageLocations.clear();
}

bool CGLES3PipelineCompute::IsTextureValid(uint32_t name) const
{
	return CGLES3Pipeline::IsTextureValid(name);
}

bool CGLES3PipelineCompute::IsUniformValid(uint32_t name) const
{
	return CGLES3Pipeline::IsUniformValid(name);
}

bool CGLES3PipelineCompute::IsUniformBlockValid(uint32_t name) const
{
	return CGLES3Pipeline::IsUniformBlockValid(name);
}

void CGLES3PipelineCompute::Bind(void) const
{
	GLUseProgram(m_program);
}
