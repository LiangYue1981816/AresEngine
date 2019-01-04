#pragma once
#include "GLES3Renderer.h"


#pragma region OpenGL extension
extern void glReadBuffers(int n, const uint32_t *bufs);
extern uint32_t glGetShaderType(shader_kind kind);
extern uint32_t glGetProgramStage(shader_kind kind);
#pragma endregion

#pragma region OpenGL state cache
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

extern void GLBindState(const PipelineState *state);

extern void GLBindVertexArray(GLuint array);
extern void GLBindBuffer(GLenum target, GLuint buffer);

extern void GLBindBufferBase(GLenum target, GLuint index, GLuint buffer);
extern void GLBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLint offset, GLsizei size);

extern void GLActiveTexture(GLuint unit);
extern void GLBindSampler(GLuint unit, GLuint sampler);
extern void GLBindTexture(GLuint unit, GLenum target, GLuint texture);

extern void GLBindFramebuffer(GLenum target, GLuint framebuffer);
extern void GLBindFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);

extern void GLReadBuffers(GLenum target, GLsizei n, const GLenum *bufs);
extern void GLDrawBuffers(GLenum target, GLsizei n, const GLenum *bufs);

extern void GLUseProgram(GLuint program);
extern void GLUniform1i(GLint location, GLint v0);
extern void GLUniform2i(GLint location, GLint v0, GLint v1);
extern void GLUniform3i(GLint location, GLint v0, GLint v1, GLint v2);
extern void GLUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
extern void GLUniform1f(GLint location, GLfloat v0);
extern void GLUniform2f(GLint location, GLfloat v0, GLfloat v1);
extern void GLUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern void GLUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern void GLUniform1iv(GLint location, GLsizei count, const GLint *value);
extern void GLUniform2iv(GLint location, GLsizei count, const GLint *value);
extern void GLUniform3iv(GLint location, GLsizei count, const GLint *value);
extern void GLUniform4iv(GLint location, GLsizei count, const GLint *value);
extern void GLUniform1fv(GLint location, GLsizei count, const GLfloat *value);
extern void GLUniform2fv(GLint location, GLsizei count, const GLfloat *value);
extern void GLUniform3fv(GLint location, GLsizei count, const GLfloat *value);
extern void GLUniform4fv(GLint location, GLsizei count, const GLfloat *value);
extern void GLUniformMatrix2fv(GLint location, GLsizei count, const GLfloat *value);
extern void GLUniformMatrix3fv(GLint location, GLsizei count, const GLfloat *value);
extern void GLUniformMatrix4fv(GLint location, GLsizei count, const GLfloat *value);

#if GLES_VER == 310
extern void GLBindProgramPipeline(GLuint pipeline);
extern void GLProgramUniform1i(GLuint program, GLint location, GLint v0);
extern void GLProgramUniform2i(GLuint program, GLint location, GLint v0, GLint v1);
extern void GLProgramUniform3i(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
extern void GLProgramUniform4i(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
extern void GLProgramUniform1f(GLuint program, GLint location, GLfloat v0);
extern void GLProgramUniform2f(GLuint program, GLint location, GLfloat v0, GLfloat v1);
extern void GLProgramUniform3f(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern void GLProgramUniform4f(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern void GLProgramUniform1iv(GLuint program, GLint location, GLsizei count, const GLint *value);
extern void GLProgramUniform2iv(GLuint program, GLint location, GLsizei count, const GLint *value);
extern void GLProgramUniform3iv(GLuint program, GLint location, GLsizei count, const GLint *value);
extern void GLProgramUniform4iv(GLuint program, GLint location, GLsizei count, const GLint *value);
extern void GLProgramUniform1fv(GLuint program, GLint location, GLsizei count, const GLfloat *value);
extern void GLProgramUniform2fv(GLuint program, GLint location, GLsizei count, const GLfloat *value);
extern void GLProgramUniform3fv(GLuint program, GLint location, GLsizei count, const GLfloat *value);
extern void GLProgramUniform4fv(GLuint program, GLint location, GLsizei count, const GLfloat *value);
extern void GLProgramUniformMatrix2fv(GLuint program, GLint location, GLsizei count, const GLfloat *value);
extern void GLProgramUniformMatrix3fv(GLuint program, GLint location, GLsizei count, const GLfloat *value);
extern void GLProgramUniformMatrix4fv(GLuint program, GLint location, GLsizei count, const GLfloat *value);
#endif
#pragma endregion
