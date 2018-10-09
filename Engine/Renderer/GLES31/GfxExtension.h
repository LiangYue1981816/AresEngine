#pragma once
#include "GfxRenderer.h"


#pragma region OpenGL extension
extern void glReadBuffers(int n, const uint32_t *bufs);
extern uint32_t glGetShaderType(shaderc_shader_kind kind);
extern uint32_t glGetProgramStage(shaderc_shader_kind kind);
#pragma endregion

#pragma region OpenGL state cache
typedef struct GLstate {
	uint32_t bEnableCullFace;
	uint32_t bEnableDepthTest;
	uint32_t bEnableDepthWrite;
	uint32_t bEnableColorWrite[4];
	uint32_t bEnableBlend;
	uint32_t bEnablePolygonOffset;
	uint32_t cullFace;
	uint32_t frontFace;
	uint32_t depthFunc;
	uint32_t srcBlendFactor;
	uint32_t dstBlendFactor;
	float polygonOffsetFactor;
	float polygonOffsetUnits;
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
extern void GLSampleMaski(GLuint maskNumber, GLbitfield mask);
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

extern void GLBindFramebuffer(GLenum target, GLuint framebuffer);
extern void GLBindProgramPipeline(GLuint pipeline);

extern void GLBindVertexArray(GLuint array);
extern void GLBindBuffer(GLenum target, GLuint buffer);

extern void GLBindBufferBase(GLenum target, GLuint index, GLuint buffer);
extern void GLBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);

extern void GLActiveTexture(GLuint unit);
extern void GLBindSampler(GLuint unit, GLuint sampler);
extern void GLBindTexture(GLuint unit, GLenum target, GLuint texture);
#pragma endregion
