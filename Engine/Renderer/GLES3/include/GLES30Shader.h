#if GLES_VER == 300

#pragma once
#include "GLES3Renderer.h"


class CGLES3Shader : public CGfxShader
{
	friend class CGLES3ShaderManager;


private:
	CGLES3Shader(uint32_t name);
	virtual ~CGLES3Shader(void);


public:
	uint32_t GetName(void) const;

public:
	HANDLE GetShader(void) const;

public:
	uint32_t GetKind(void) const;
	const spirv_cross::CompilerGLSL* GetShaderCompiler(void) const;

public:
	bool Create(const uint32_t *words, size_t numWords, shader_kind kind);
	void Destroy(void);

public:
	bool IsValid(void) const;


private:
	uint32_t m_name;

private:
	uint32_t m_kind;
	uint32_t m_shader;

private:
	spirv_cross::CompilerGLSL *m_pShaderCompiler;
};

#endif
