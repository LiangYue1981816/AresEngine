#pragma once
#include <spirv_cfg.hpp>
#include <spirv_glsl.hpp>
#include "GfxRenderer.h"


class CGfxProgram
{
	friend class CGfxRenderer;
	friend class CGfxMaterial;
	friend class CGfxProgramManager;


private:
	CGfxProgram(void);
	virtual ~CGfxProgram(void);


private:
	bool Load(const char *szVertexFileName, const char *szFragmentFileName);
	void Free(void);

private:
	bool LoadShader(const char *szFileName, GLenum type, GLuint &shader, spirv_cross::CompilerGLSL *&pShaderCompiler);
	bool CreateProgram(void);
	bool CreateLocations(void);

	bool SetUniformLocation(const char *szName);
	bool SetTextureLocation(const char *szName);

private:
	void UseProgram(void) const;
	
	bool BindUniformBuffer(GLuint name, GLuint buffer, GLsizeiptr size, GLintptr offset = 0) const;
	bool BindTexture2D(GLuint name, GLuint texture, GLuint sampler, GLuint unit) const;
	bool BindTextureArray(GLuint name, GLuint texture, GLuint sampler, GLuint unit) const;
	bool BindTextureCubeMap(GLuint name, GLuint texture, GLuint sampler, GLuint unit) const;

public:
	bool IsValid(void) const;

	bool IsUniformValid(GLuint name) const;
	bool IsTextureValid(GLuint name) const;


private:
	eastl::unordered_map<uint32_t, eastl::string> m_names;
	eastl::unordered_map<uint32_t, uint32_t> m_uniformBlockLocations;
	eastl::unordered_map<uint32_t, uint32_t> m_sampledImageLocations;

private:
	GLuint m_program;
	GLuint m_vertexShader;
	GLuint m_fragmentShader;
	spirv_cross::CompilerGLSL *m_pShaderCompilers[2];
};
