#pragma once
#include "GfxRenderer.h"


class CGfxProgram
{
	friend class CGfxRenderer;
	friend class CGfxProgramManager;


private:
	typedef struct GLstate {
		GLboolean bEnableCullFace;
		GLboolean bEnableDepthTest;
		GLboolean bEnableDepthWrite;
		GLboolean bEnableColorWrite[4];
		GLboolean bEnableBlend;
		GLboolean bEnablePolygonOffset;
		GLenum cullFace;
		GLenum frontFace;
		GLenum depthFunc;
		GLenum srcBlendFactor;
		GLenum dstBlendFactor;
		GLfloat polygonOffsetFactor;
		GLfloat polygonOffsetUnits;
	} GLstate;


private:
	CGfxProgram(uint64_t name);
	virtual ~CGfxProgram(void);

	uint64_t GetName(void) const;


private:
	bool CreateCompute(CGfxShader *pComputeShader);
	bool CreateGraphics(CGfxShader *pVertexShader, CGfxShader *pFragmentShader);
	bool CreateProgram(void);
	bool CreateLocations(void);
	void Destroy(void);

private:
	bool SetUniformLocation(const char *szName, GLuint program);
	bool SetTextureLocation(const char *szName, GLuint program);

private:
	void UseProgram(void) const;

	bool BindUniformBuffer(GLuint name, GLuint buffer, GLsizeiptr size, GLintptr offset = 0) const;
	bool BindTexture2D(GLuint name, GLuint texture, GLuint sampler, GLuint unit) const;
	bool BindTextureArray(GLuint name, GLuint texture, GLuint sampler, GLuint unit) const;
	bool BindTextureCubeMap(GLuint name, GLuint texture, GLuint sampler, GLuint unit) const;

public:
	void SetEnableCullFace(bool bEnable, GLenum cullFace, GLenum frontFace);
	void SetEnableDepthTest(bool bEnable, GLenum depthFunc);
	void SetEnableDepthWrite(bool bEnable);
	void SetEnableColorWrite(bool bEnableRed, bool bEnableGreen, bool bEnableBlue, bool bEnableAlpha);
	void SetEnableBlend(bool bEnable, GLenum srcFactor, GLenum dstFactor);
	void SetEnablePolygonOffset(bool bEnable, GLfloat factor, GLfloat units);

	bool IsEnableBlend(void) const;
	bool IsUniformValid(GLuint name) const;
	bool IsTextureValid(GLuint name) const;


private:
	uint64_t m_name;

private:
	eastl::unordered_map<uint32_t, eastl::string> m_names;
	eastl::unordered_map<uint32_t, uint32_t> m_uniformBlockLocations;
	eastl::unordered_map<uint32_t, uint32_t> m_sampledImageLocations;

private:
	GLstate m_state;
	GLuint m_program;
	CGfxShader *m_pShaders[shaderc_shader_kind::shaderc_compute_shader - shaderc_shader_kind::shaderc_vertex_shader + 1];
};
