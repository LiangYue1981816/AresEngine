#pragma once
#include "GfxRenderer.h"


class CGfxShader
{
	friend class CGfxRenderer;
	friend class CGfxProgram;
	friend class CGfxShaderManager;


private:
	CGfxShader(uint32_t name);
	virtual ~CGfxShader(void);

	uint32_t GetName(void) const;


private:
	bool Create(const uint32_t *words, size_t numWords, shaderc_shader_kind kind);
	void Destroy(void);

private:
	GLuint GetKind(void) const;
	GLuint GetProgram(void) const;
	spirv_cross::CompilerGLSL* GetCompiler(void) const;


private:
	uint32_t m_name;

private:
	GLuint m_kind;
	GLuint m_program;
	spirv_cross::CompilerGLSL *m_pShaderCompiler;
};
