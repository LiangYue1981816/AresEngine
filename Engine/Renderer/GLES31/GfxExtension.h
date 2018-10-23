#pragma once
#include "GfxRenderer.h"


#pragma region OpenGL extension
extern void glReadBuffers(int n, const uint32_t *bufs);
extern uint32_t glGetShaderType(shaderc_shader_kind kind);
extern uint32_t glGetProgramStage(shaderc_shader_kind kind);
#pragma endregion

#pragma region OpenGL state cache
typedef struct GLstate {
	uint32_t bEnableCullFace = GL_TRUE;
	uint32_t cullFace = GL_BACK;
	uint32_t frontFace = GL_CCW;

	uint32_t bEnableStencilTest = GL_FALSE;
	uint32_t stencilFunc = GL_ALWAYS;
	uint32_t stencilRef = 0;
	uint32_t stencilMask = 1;
	uint32_t stencilOpSFail = GL_KEEP;
	uint32_t stencilOpDFail = GL_KEEP;
	uint32_t stencilOpDPass = GL_KEEP;

	uint32_t bEnableDepthTest = GL_TRUE;
	uint32_t bEnableDepthWrite = GL_TRUE;
	uint32_t depthFunc = GL_LESS;
	float depthRangeNear = 0.0f;
	float depthRangeFar = 1.0f;

	uint32_t bEnableColorWrite[4] = { GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE };

	uint32_t bEnableAlphaToCoverage = GL_FALSE;
	uint32_t bEnableBlend = GL_FALSE;
	uint32_t blendSrcFactor = GL_ONE;
	uint32_t blendDstFactor = GL_ZERO;
	uint32_t blendEquation = GL_FUNC_ADD;
	float blendColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	uint32_t bEnablePolygonOffset = GL_FALSE;
	float polygonOffsetFactor = 0.0f;
	float polygonOffsetUnits = 0.0f;
} GLstate;

extern void GLInitState(GLstate *state);
extern void GLBindState(const GLstate *state);

extern void GLResetContext(void);

extern void GLEnable(GLenum cap);
extern void GLDisable(GLenum cap);

extern void GLScissor(GLint x, GLint y, GLsizei width, GLsizei height);
extern void GLViewport(GLint x, GLint y, GLsizei width, GLsizei height);

extern void GLClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern void GLClearDepthf(GLfloat depth);
extern void GLClearStencil(GLint stencil);

extern void GLCullFace(GLenum mode);
extern void GLFrontFace(GLenum mode);
extern void GLLineWidth(GLfloat width);
extern void GLPolygonOffset(GLfloat factor, GLfloat units);
extern void GLDepthRangef(GLfloat n, GLfloat f);
extern void GLDepthFunc(GLenum func);
extern void GLDepthMask(GLboolean flag);
extern void GLColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);

extern void GLStencilFunc(GLenum func, GLint ref, GLuint mask);
extern void GLStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
extern void GLStencilMask(GLuint mask);

extern void GLBlendFunc(GLenum sfactor, GLenum dfactor);
extern void GLBlendEquation(GLenum mode);
extern void GLBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

extern void GLBindVertexArray(GLuint array);
extern void GLBindBuffer(GLenum target, GLuint buffer);

extern void GLBindBufferBase(GLenum target, GLuint index, GLuint buffer);
extern void GLBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLint offset, GLsizei size);

extern void GLActiveTexture(GLuint unit);
extern void GLBindSampler(GLuint unit, GLuint sampler);
extern void GLBindTexture(GLuint unit, GLenum target, GLuint texture);

extern void GLBindProgramPipeline(GLuint pipeline);
extern void GLBindFramebuffer(GLenum target, GLuint framebuffer);
extern void GLBindFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);

extern void GLReadBuffers(GLenum target, GLsizei n, const GLenum *bufs);
extern void GLDrawBuffers(GLenum target, GLsizei n, const GLenum *bufs);

extern void GLProgramUniform1i(GLuint program, GLint location, GLint v0);
extern void GLProgramUniform2i(GLuint program, GLint location, GLint v0, GLint v1);
extern void GLProgramUniform3i(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
extern void GLProgramUniform4i(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
extern void GLProgramUniform1f(GLuint program, GLint location, GLfloat v0);
extern void GLProgramUniform2f(GLuint program, GLint location, GLfloat v0, GLfloat v1);
extern void GLProgramUniform3f(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern void GLProgramUniform4f(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
#pragma endregion
