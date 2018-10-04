#pragma once
#include "GfxRenderer.h"


class CGfxShader
{
	friend class CGfxRenderer;
	friend class CGfxShaderManager;


private:
	CGfxShader(uint32_t name);
	virtual ~CGfxShader(void);

public:
	uint32_t GetName(void) const;


private:
	bool Load(const char *szFileName, shaderc_shader_kind kind);
	bool Create(const uint32_t *words, size_t numWords, shaderc_shader_kind kind);
	void Destroy(void);

public:
	bool IsValid(void) const;

public:
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
