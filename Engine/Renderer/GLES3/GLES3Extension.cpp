#include "GLES3Renderer.h"
#include "GLES3Extension.h"


#pragma region OpenGL extension
void glReadBuffers(int n, const uint32_t *bufs)
{
	for (int index = 0; index < n; index++) {
		glReadBuffer(bufs[index]);
	}
}

uint32_t glGetShaderType(shader_kind kind)
{
	switch (kind) {
	case vertex_shader:
		return GL_VERTEX_SHADER;
	case fragment_shader:
		return GL_FRAGMENT_SHADER;
	case compute_shader:
		return GL_COMPUTE_SHADER;
	default:
		return GL_INVALID_ENUM;
	}
}

uint32_t glGetProgramStage(shader_kind kind)
{
	switch (kind) {
	case vertex_shader:
		return GL_VERTEX_SHADER_BIT;
	case fragment_shader:
		return GL_FRAGMENT_SHADER_BIT;
	case compute_shader:
		return GL_COMPUTE_SHADER_BIT;
	default:
		return GL_INVALID_ENUM;
	}
}
#pragma endregion

#pragma region OpenGL state cache
typedef struct ScissorParam {
	int x;
	int y;
	int width;
	int height;
} ScissorParam;

typedef struct ViewportParam {
	int x;
	int y;
	int width;
	int height;
} ViewportParam;

typedef struct ClearColorParam {
	float red;
	float green;
	float blue;
	float alpha;
} ClearColorParam;

typedef struct ClearDepthParam {
	float depth;
} ClearDepthParam;

typedef struct ClearStencilParam {
	int stencil;
} ClearStencilParam;

typedef struct CullFaceParam {
	uint32_t mode;
} CullFaceParam;

typedef struct FrontFaceParam {
	uint32_t mode;
} FrontFaceParam;

typedef struct LineWidthParam {
	float width;
} LineWidthParam;

typedef struct PolygonOffsetParam {
	float factor;
	float units;
} PolygonOffsetParam;

typedef struct DepthRangefParam {
	float n;
	float f;
} DepthRangefParam;

typedef struct DepthFuncParam {
	uint32_t func;
} DepthFuncParam;

typedef struct DepthMaskParam {
	uint32_t flag;
} DepthMaskParam;

typedef struct ColorMaskParam {
	uint32_t red;
	uint32_t green;
	uint32_t blue;
	uint32_t alpha;
} ColorMaskParam;

typedef struct StencilFuncParam {
	uint32_t func;
	uint32_t ref;
	uint32_t mask;
} StencilFuncParam;

typedef struct StencilOpParam {
	uint32_t sfail;
	uint32_t dpfail;
	uint32_t dppass;
} StencilOpParam;

typedef struct StencilMaskParam {
	uint32_t mask;
} StencilMaskParam;

typedef struct BlendFuncParam {
	uint32_t sfactor;
	uint32_t dfactor;
} BlendFuncParam;

typedef struct BlendEquationParam {
	uint32_t mode;
} BlendEquationParam;

typedef struct BlendColorParam {
	float red;
	float green;
	float blue;
	float alpha;
} BlendColorParam;

typedef struct FrameBufferAttachmentParam {
	uint32_t textarget;
	uint32_t texture;
	int level;
} FrameBufferAttachmentParam;

typedef struct FrameBufferParam {
	uint32_t framebuffer;
	eastl::unordered_map<uint32_t, eastl::unordered_map<uint32_t, FrameBufferAttachmentParam>> attachments;
	eastl::unordered_map<uint32_t, eastl::vector<uint32_t>> readbuffers;
	eastl::unordered_map<uint32_t, eastl::vector<uint32_t>> drawbuffers;
} FrameBufferParam;

typedef struct ProgramPipelineParam {
	uint32_t program;
	uint32_t pipeline;
	eastl::unordered_map<int, eastl::unordered_map<int, int>> uniform1i;
	eastl::unordered_map<int, eastl::unordered_map<int, int[2]>> uniform2i;
	eastl::unordered_map<int, eastl::unordered_map<int, int[3]>> uniform3i;
	eastl::unordered_map<int, eastl::unordered_map<int, int[4]>> uniform4i;
	eastl::unordered_map<int, eastl::unordered_map<int, float>> uniform1f;
	eastl::unordered_map<int, eastl::unordered_map<int, float[2]>> uniform2f;
	eastl::unordered_map<int, eastl::unordered_map<int, float[3]>> uniform3f;
	eastl::unordered_map<int, eastl::unordered_map<int, float[4]>> uniform4f;
	eastl::unordered_map<int, eastl::unordered_map<int, eastl::vector<int>>> uniform1iv;
	eastl::unordered_map<int, eastl::unordered_map<int, eastl::vector<int>>> uniform2iv;
	eastl::unordered_map<int, eastl::unordered_map<int, eastl::vector<int>>> uniform3iv;
	eastl::unordered_map<int, eastl::unordered_map<int, eastl::vector<int>>> uniform4iv;
	eastl::unordered_map<int, eastl::unordered_map<int, eastl::vector<float>>> uniform1fv;
	eastl::unordered_map<int, eastl::unordered_map<int, eastl::vector<float>>> uniform2fv;
	eastl::unordered_map<int, eastl::unordered_map<int, eastl::vector<float>>> uniform3fv;
	eastl::unordered_map<int, eastl::unordered_map<int, eastl::vector<float>>> uniform4fv;
	eastl::unordered_map<int, eastl::unordered_map<int, eastl::vector<float>>> uniformMatrix2fv;
	eastl::unordered_map<int, eastl::unordered_map<int, eastl::vector<float>>> uniformMatrix3fv;
	eastl::unordered_map<int, eastl::unordered_map<int, eastl::vector<float>>> uniformMatrix4fv;
} ProgramPipelineParam;

typedef struct VertexArrayParam {
	uint32_t array;
} VertexArrayParam;

typedef struct BufferBaseParam {
	uint32_t index;
	uint32_t buffer;
} BufferBaseParam;

typedef struct BufferRangeParam {
	uint32_t index;
	uint32_t buffer;
	int offset;
	int size;
} BufferRangeParam;

typedef struct TextureParam {
	uint32_t target;
	uint32_t texture;
} TextureParam;

static ScissorParam Scissor;
static ViewportParam Viewport;
static ClearColorParam ClearColor;
static ClearDepthParam ClearDepth;
static ClearStencilParam ClearStencil;
static CullFaceParam CullFace;
static FrontFaceParam FrontFace;
static LineWidthParam LineWidth;
static PolygonOffsetParam PolygonOffset;
static DepthRangefParam DepthRangef;
static DepthFuncParam DepthFunc;
static DepthMaskParam DepthMask;
static ColorMaskParam ColorMask;
static StencilFuncParam StencilFunc;
static StencilOpParam StencilOp;
static StencilMaskParam StencilMask;
static BlendFuncParam BlendFunc;
static BlendEquationParam BlendEquation;
static BlendColorParam BlendColor;
static VertexArrayParam VertexArray;
static ProgramPipelineParam ProgramPipeline;

static eastl::unordered_map<GLenum, GLboolean> Caps;
static eastl::unordered_map<GLenum, GLuint> Buffers;
static eastl::unordered_map<GLenum, BufferBaseParam> BufferBases;
static eastl::unordered_map<GLenum, BufferRangeParam> BufferRanges;
static eastl::unordered_map<GLenum, FrameBufferParam> FrameBuffers;
static eastl::unordered_map<GLuint, GLuint> Samplers;
static eastl::unordered_map<GLuint, TextureParam> Textures;
static eastl::unordered_map<GLuint, GLuint> ActiveTextures;

void GLResetContext(void)
{
	Caps.clear();
	Buffers.clear();
	BufferBases.clear();
	BufferRanges.clear();
	FrameBuffers.clear();
	Samplers.clear();
	Textures.clear();
	ActiveTextures.clear();
	ProgramPipeline.uniform1i.clear();
	ProgramPipeline.uniform2i.clear();
	ProgramPipeline.uniform3i.clear();
	ProgramPipeline.uniform4i.clear();
	ProgramPipeline.uniform1f.clear();
	ProgramPipeline.uniform2f.clear();
	ProgramPipeline.uniform3f.clear();
	ProgramPipeline.uniform4f.clear();
	ProgramPipeline.uniform1iv.clear();
	ProgramPipeline.uniform2iv.clear();
	ProgramPipeline.uniform3iv.clear();
	ProgramPipeline.uniform4iv.clear();
	ProgramPipeline.uniform1fv.clear();
	ProgramPipeline.uniform2fv.clear();
	ProgramPipeline.uniform3fv.clear();
	ProgramPipeline.uniform4fv.clear();
	ProgramPipeline.uniformMatrix2fv.clear();
	ProgramPipeline.uniformMatrix3fv.clear();
	ProgramPipeline.uniformMatrix4fv.clear();

	Scissor.x = GL_INVALID_VALUE;
	Scissor.y = GL_INVALID_VALUE;
	Scissor.width = GL_INVALID_VALUE;
	Scissor.height = GL_INVALID_VALUE;
	Viewport.x = GL_INVALID_VALUE;
	Viewport.y = GL_INVALID_VALUE;
	Viewport.width = GL_INVALID_VALUE;
	Viewport.height = GL_INVALID_VALUE;
	ClearColor.red = GL_INVALID_VALUE;
	ClearColor.green = GL_INVALID_VALUE;
	ClearColor.blue = GL_INVALID_VALUE;
	ClearColor.alpha = GL_INVALID_VALUE;
	ClearDepth.depth = GL_INVALID_VALUE;
	ClearStencil.stencil = GL_INVALID_VALUE;
	CullFace.mode = GL_INVALID_ENUM;
	FrontFace.mode = GL_INVALID_ENUM;
	LineWidth.width = GL_INVALID_VALUE;
	PolygonOffset.factor = GL_INVALID_VALUE;
	PolygonOffset.units = GL_INVALID_VALUE;
	DepthRangef.n = GL_INVALID_VALUE;
	DepthRangef.f = GL_INVALID_VALUE;
	DepthFunc.func = GL_INVALID_ENUM;
	DepthMask.flag = GL_INVALID_VALUE;
	ColorMask.red = GL_INVALID_VALUE;
	ColorMask.green = GL_INVALID_VALUE;
	ColorMask.blue = GL_INVALID_VALUE;
	ColorMask.alpha = GL_INVALID_VALUE;
	StencilFunc.func = GL_INVALID_ENUM;
	StencilFunc.mask = GL_INVALID_ENUM;
	StencilFunc.ref = GL_INVALID_ENUM;
	StencilOp.dpfail = GL_INVALID_ENUM;
	StencilOp.dppass = GL_INVALID_ENUM;
	StencilOp.sfail = GL_INVALID_ENUM;
	StencilMask.mask = GL_INVALID_ENUM;
	BlendFunc.sfactor = GL_INVALID_ENUM;
	BlendFunc.dfactor = GL_INVALID_ENUM;;
	BlendEquation.mode = GL_INVALID_ENUM;
	BlendColor.red = GL_INVALID_VALUE;
	BlendColor.green = GL_INVALID_VALUE;
	BlendColor.blue = GL_INVALID_VALUE;
	BlendColor.alpha = GL_INVALID_VALUE;
	VertexArray.array = GL_INVALID_VALUE;
	ProgramPipeline.program = GL_INVALID_VALUE;
	ProgramPipeline.pipeline = GL_INVALID_VALUE;
}

void GLEnable(GLenum cap)
{
	switch (cap) {
	case GL_BLEND:
	case GL_CULL_FACE:
	case GL_DEPTH_TEST:
	case GL_DITHER:
	case GL_POLYGON_OFFSET_FILL:
	case GL_PRIMITIVE_RESTART_FIXED_INDEX:
	case GL_RASTERIZER_DISCARD:
	case GL_SAMPLE_ALPHA_TO_COVERAGE:
	case GL_SAMPLE_COVERAGE:
	case GL_SAMPLE_MASK:
	case GL_SCISSOR_TEST:
	case GL_STENCIL_TEST:
		if (Caps.find(cap) == Caps.end() || Caps[cap] != GL_TRUE) {
			Caps[cap] = GL_TRUE;
			glEnable(cap);
		}
		break;
	}
}

void GLDisable(GLenum cap)
{
	switch (cap) {
	case GL_BLEND:
	case GL_CULL_FACE:
	case GL_DEPTH_TEST:
	case GL_DITHER:
	case GL_POLYGON_OFFSET_FILL:
	case GL_PRIMITIVE_RESTART_FIXED_INDEX:
	case GL_RASTERIZER_DISCARD:
	case GL_SAMPLE_ALPHA_TO_COVERAGE:
	case GL_SAMPLE_COVERAGE:
	case GL_SAMPLE_MASK:
	case GL_SCISSOR_TEST:
	case GL_STENCIL_TEST:
		if (Caps.find(cap) == Caps.end() || Caps[cap] != GL_FALSE) {
			Caps[cap] = GL_FALSE;
			glDisable(cap);
		}
		break;
	}
}

void GLScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
	if (Scissor.x != x || Scissor.y != y || Scissor.width != width || Scissor.height != height) {
		Scissor.x = x;
		Scissor.y = y;
		Scissor.width = width;
		Scissor.height = height;
		glScissor(x, y, width, height);
	}
}

void GLViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	if (Viewport.x != x || Viewport.y != y || Viewport.width != width || Viewport.height != height) {
		Viewport.x = x;
		Viewport.y = y;
		Viewport.width = width;
		Viewport.height = height;
		glViewport(x, y, width, height);
	}
}

void GLClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	if (ClearColor.red != red || ClearColor.green != green || ClearColor.blue != blue || ClearColor.alpha != alpha) {
		ClearColor.red = red;
		ClearColor.green = green;
		ClearColor.blue = blue;
		ClearColor.alpha = alpha;
		glClearColor(red, green, blue, alpha);
	}
}

void GLClearDepthf(GLfloat depth)
{
	if (ClearDepth.depth != depth) {
		ClearDepth.depth = depth;
		glClearDepthf(ClearDepth.depth);
	}
}

void GLClearStencil(GLint stencil)
{
	if (ClearStencil.stencil != stencil) {
		ClearStencil.stencil = stencil;
		GLClearStencil(stencil);
	}
}

void GLCullFace(GLenum mode)
{
	if (CullFace.mode != mode) {
		CullFace.mode = mode;
		glCullFace(mode);
	}
}

void GLFrontFace(GLenum mode)
{
	if (FrontFace.mode != mode) {
		FrontFace.mode = mode;
		glFrontFace(mode);
	}
}

void GLLineWidth(GLfloat width)
{
	if (LineWidth.width != width) {
		LineWidth.width = width;
		glLineWidth(width);
	}
}

void GLPolygonOffset(GLfloat factor, GLfloat units)
{
	if (PolygonOffset.factor != factor || PolygonOffset.units != units) {
		PolygonOffset.factor = factor;
		PolygonOffset.units = units;
		glPolygonOffset(factor, units);
	}
}

void GLDepthRangef(GLfloat n, GLfloat f)
{
	if (DepthRangef.n != n || DepthRangef.f != f) {
		DepthRangef.n = n;
		DepthRangef.f = f;
		glDepthRangef(n, f);
	}
}

void GLDepthFunc(GLenum func)
{
	if (DepthFunc.func != func) {
		DepthFunc.func = func;
		glDepthFunc(func);
	}
}

void GLDepthMask(GLboolean flag)
{
	if (DepthMask.flag != flag) {
		DepthMask.flag = flag;
		glDepthMask(flag);
	}
}

void GLColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
	if (ColorMask.red != red || ColorMask.green != green || ColorMask.blue != blue || ColorMask.alpha != alpha) {
		ColorMask.red = red;
		ColorMask.green = green;
		ColorMask.blue = blue;
		ColorMask.alpha = alpha;
		glColorMask(red, green, blue, alpha);
	}
}

void GLStencilFunc(GLenum func, GLint ref, GLuint mask)
{
	if (StencilFunc.func != func || StencilFunc.ref != ref || StencilFunc.mask != mask) {
		StencilFunc.func = func;
		StencilFunc.ref = ref;
		StencilFunc.mask = mask;
		glStencilFunc(func, ref, mask);
	}
}

void GLStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
{
	if (StencilOp.sfail != sfail || StencilOp.dpfail != dpfail || StencilOp.dppass != dppass) {
		StencilOp.sfail = sfail;
		StencilOp.dpfail = dpfail;
		StencilOp.dppass = dppass;
		glStencilOp(sfail, dpfail, dppass);
	}
}

void GLStencilMask(GLuint mask)
{
	if (StencilMask.mask != mask) {
		StencilMask.mask = mask;
		glStencilMask(mask);
	}
}

void GLBlendFunc(GLenum sfactor, GLenum dfactor)
{
	if (BlendFunc.sfactor != sfactor || BlendFunc.dfactor != dfactor) {
		BlendFunc.sfactor = sfactor;
		BlendFunc.dfactor = dfactor;
		glBlendFunc(sfactor, dfactor);
	}
}

void GLBlendEquation(GLenum mode)
{
	if (BlendEquation.mode != mode) {
		BlendEquation.mode = mode;
		glBlendEquation(mode);
	}
}

void GLBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	if (BlendColor.red != red || BlendColor.green != green || BlendColor.blue != blue || BlendColor.alpha != alpha) {
		BlendColor.red = red;
		BlendColor.green = green;
		BlendColor.blue = blue;
		BlendColor.alpha = alpha;
		glBlendColor(red, green, blue, alpha);
	}
}

void GLBindState(const PipelineState *state)
{
	if (state->bEnableCullFace) {
		GLEnable(GL_CULL_FACE);
		GLCullFace(state->cullFace);
		GLFrontFace(state->frontFace);
	}
	else {
		GLDisable(GL_CULL_FACE);
	}

	if (state->bEnableStencilTest) {
		GLEnable(GL_STENCIL_TEST);
		GLStencilFunc(state->stencilFunc, state->stencilRef, state->stencilMask);
		GLStencilOp(state->stencilOpSFail, state->stencilOpDFail, state->stencilOpDPass);
	}
	else {
		GLDisable(GL_STENCIL_TEST);
	}

	if (state->bEnableDepthTest) {
		GLEnable(GL_DEPTH_TEST);
		GLDepthFunc(state->depthFunc);
		GLDepthRangef(state->depthRangeNear, state->depthRangeFar);
	}
	else {
		GLDisable(GL_DEPTH_TEST);
	}

	if (state->bEnableDepthWrite) {
		GLDepthMask(GL_TRUE);
	}
	else {
		GLDepthMask(GL_FALSE);
	}

	if (state->bEnableAlphaToCoverage) {
		GLEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	}
	else {
		GLDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	}

	if (state->bEnableBlend) {
		GLEnable(GL_BLEND);
		GLBlendFunc(state->blendSrcFactor, state->blendDstFactor);
		GLBlendEquation(state->blendEquation);
		GLBlendColor(state->blendColor[0], state->blendColor[1], state->blendColor[2], state->blendColor[3]);
	}
	else {
		GLDisable(GL_BLEND);
	}

	if (state->bEnablePolygonOffset) {
		GLEnable(GL_POLYGON_OFFSET_FILL);
		GLPolygonOffset(state->polygonOffsetFactor, state->polygonOffsetUnits);
	}
	else {
		GLDisable(GL_POLYGON_OFFSET_FILL);
	}

	GLColorMask(
		state->bEnableColorWrite[0],
		state->bEnableColorWrite[1],
		state->bEnableColorWrite[2],
		state->bEnableColorWrite[3]);
}

void GLBindVertexArray(GLuint array)
{
	if (VertexArray.array != array) {
		VertexArray.array = array;
		glBindVertexArray(array);
	}
}

void GLBindBuffer(GLenum target, GLuint buffer)
{
	switch (target) {
	case GL_ARRAY_BUFFER:
	case GL_ATOMIC_COUNTER_BUFFER:
	case GL_COPY_READ_BUFFER:
	case GL_COPY_WRITE_BUFFER:
	case GL_DRAW_INDIRECT_BUFFER:
	case GL_DISPATCH_INDIRECT_BUFFER:
	case GL_ELEMENT_ARRAY_BUFFER:
	case GL_PIXEL_PACK_BUFFER:
	case GL_PIXEL_UNPACK_BUFFER:
	case GL_SHADER_STORAGE_BUFFER:
	case GL_TRANSFORM_FEEDBACK_BUFFER:
	case GL_UNIFORM_BUFFER:
		if (Buffers.find(target) == Buffers.end() || Buffers[target] != buffer) {
			Buffers[target] = buffer;
			glBindBuffer(target, buffer);
		}
		break;
	}
}

void GLBindBufferBase(GLenum target, GLuint index, GLuint buffer)
{
	switch (target) {
	case GL_ATOMIC_COUNTER_BUFFER:
	case GL_SHADER_STORAGE_BUFFER:
	case GL_TRANSFORM_FEEDBACK_BUFFER:
	case GL_UNIFORM_BUFFER:
		if (BufferBases.find(target) == BufferBases.end() || BufferBases[target].index != index || BufferBases[target].buffer != buffer) {
			BufferBases[target].index = index;
			BufferBases[target].buffer = buffer;
			glBindBufferBase(target, index, buffer);
		}
		break;
	}
}

void GLBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLint offset, GLsizei size)
{
	switch (target) {
	case GL_ATOMIC_COUNTER_BUFFER:
	case GL_SHADER_STORAGE_BUFFER:
	case GL_TRANSFORM_FEEDBACK_BUFFER:
	case GL_UNIFORM_BUFFER:
		if (BufferRanges.find(target) == BufferRanges.end() || BufferRanges[target].index != index || BufferRanges[target].buffer != buffer || BufferRanges[target].offset != offset || BufferRanges[target].size != size) {
			BufferRanges[target].index = index;
			BufferRanges[target].buffer = buffer;
			BufferRanges[target].offset = offset;
			BufferRanges[target].size = size;
			glBindBufferRange(target, index, buffer, offset, size);
		}
		break;
	}
}

void GLActiveTexture(GLuint unit)
{
	if (ActiveTextures.find(unit) == ActiveTextures.end()) {
		ActiveTextures[unit] = unit;
		glActiveTexture(GL_TEXTURE0 + unit);
	}
}

void GLBindSampler(GLuint unit, GLuint sampler)
{
	if (Samplers.find(unit) == Samplers.end() || Samplers[unit] != sampler) {
		Samplers[unit] = sampler;
		glBindSampler(unit, sampler);
	}
}

void GLBindTexture(GLuint unit, GLenum target, GLuint texture)
{
	if (Textures.find(unit) == Textures.end() || Textures[unit].target != target || Textures[unit].texture != texture) {
		Textures[unit].target = target;
		Textures[unit].texture = texture;
		glBindTexture(target, texture);
	}
}

void GLBindFramebuffer(GLenum target, GLuint framebuffer)
{
	switch (target) {
	case GL_FRAMEBUFFER:
	case GL_DRAW_FRAMEBUFFER:
	case GL_READ_FRAMEBUFFER:
		if (FrameBuffers.find(target) == FrameBuffers.end() || FrameBuffers[target].framebuffer != framebuffer) {
			FrameBuffers[target].framebuffer = framebuffer;
			glBindFramebuffer(target, framebuffer);
		}
		break;
	}
}

void GLBindFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
	if (FrameBuffers.find(target) != FrameBuffers.end()) {
		uint32_t framebuffer = FrameBuffers[target].framebuffer;
		if (FrameBuffers[target].attachments[framebuffer].find(attachment) == FrameBuffers[target].attachments[framebuffer].end() || FrameBuffers[target].attachments[framebuffer][attachment].textarget != textarget || FrameBuffers[target].attachments[framebuffer][attachment].texture != texture || FrameBuffers[target].attachments[framebuffer][attachment].level != level) {
			FrameBuffers[target].attachments[framebuffer][attachment].textarget = textarget;
			FrameBuffers[target].attachments[framebuffer][attachment].texture = texture;
			FrameBuffers[target].attachments[framebuffer][attachment].level = level;
			glFramebufferTexture2D(target, attachment, textarget, texture, level);
		}
	}
}

void GLReadBuffers(GLenum target, GLsizei n, const GLenum *bufs)
{
	if (FrameBuffers.find(target) != FrameBuffers.end()) {
		bool bReset = false;
		uint32_t framebuffer = FrameBuffers[target].framebuffer;

		if (n != FrameBuffers[target].readbuffers[framebuffer].size()) {
			bReset = true;
		}
		else {
			for (int index = 0; index < n; index++) {
				if (bufs[index] != FrameBuffers[target].readbuffers[framebuffer][index]) {
					bReset = true;
					break;
				}
			}
		}

		if (bReset) {
			FrameBuffers[target].readbuffers[framebuffer].clear();

			for (int index = 0; index < n; index++) {
				FrameBuffers[target].readbuffers[framebuffer].emplace_back(bufs[index]);
			}

			glReadBuffers(n, bufs);
		}
	}
	else {
		glReadBuffers(n, bufs);
	}
}

void GLDrawBuffers(GLenum target, GLsizei n, const GLenum *bufs)
{
	if (FrameBuffers.find(target) != FrameBuffers.end()) {
		bool bReset = false;
		uint32_t framebuffer = FrameBuffers[target].framebuffer;

		if (n != FrameBuffers[target].drawbuffers[framebuffer].size()) {
			bReset = true;
		}
		else {
			for (int index = 0; index < n; index++) {
				if (bufs[index] != FrameBuffers[target].drawbuffers[framebuffer][index]) {
					bReset = true;
					break;
				}
			}
		}

		if (bReset) {
			FrameBuffers[target].drawbuffers[framebuffer].clear();

			for (int index = 0; index < n; index++) {
				FrameBuffers[target].drawbuffers[framebuffer].emplace_back(bufs[index]);
			}

			glDrawBuffers(n, bufs);
		}
	}
	else {
		glDrawBuffers(n, bufs);
	}
}

void GLUseProgram(GLuint program)
{
	if (ProgramPipeline.program != program) {
		ProgramPipeline.program = program;
		ProgramPipeline.uniform1i[program].clear();
		ProgramPipeline.uniform2i[program].clear();
		ProgramPipeline.uniform3i[program].clear();
		ProgramPipeline.uniform4i[program].clear();
		ProgramPipeline.uniform1f[program].clear();
		ProgramPipeline.uniform2f[program].clear();
		ProgramPipeline.uniform3f[program].clear();
		ProgramPipeline.uniform4f[program].clear();
		ProgramPipeline.uniform1iv[program].clear();
		ProgramPipeline.uniform2iv[program].clear();
		ProgramPipeline.uniform3iv[program].clear();
		ProgramPipeline.uniform4iv[program].clear();
		ProgramPipeline.uniform1fv[program].clear();
		ProgramPipeline.uniform2fv[program].clear();
		ProgramPipeline.uniform3fv[program].clear();
		ProgramPipeline.uniform4fv[program].clear();
		ProgramPipeline.uniformMatrix2fv[program].clear();
		ProgramPipeline.uniformMatrix3fv[program].clear();
		ProgramPipeline.uniformMatrix4fv[program].clear();
		GLUseProgram(program);
	}
}

void GLUniform1i(GLint location, GLint v0)
{
	if (ProgramPipeline.uniform1i[ProgramPipeline.program].find(location) == ProgramPipeline.uniform1i[ProgramPipeline.program].end() ||
		ProgramPipeline.uniform1i[ProgramPipeline.program][location] != v0) {
		ProgramPipeline.uniform1i[ProgramPipeline.program][location] = v0;
		glUniform1i(location, v0);
	}
}

void GLUniform2i(GLint location, GLint v0, GLint v1)
{
	if (ProgramPipeline.uniform2i[ProgramPipeline.program].find(location) == ProgramPipeline.uniform2i[ProgramPipeline.program].end() ||
		ProgramPipeline.uniform2i[ProgramPipeline.program][location][0] != v0 ||
		ProgramPipeline.uniform2i[ProgramPipeline.program][location][1] != v1) {
		ProgramPipeline.uniform2i[ProgramPipeline.program][location][0] = v0;
		ProgramPipeline.uniform2i[ProgramPipeline.program][location][1] = v1;
		glUniform2i(location, v0, v1);
	}
}

void GLUniform3i(GLint location, GLint v0, GLint v1, GLint v2)
{
	if (ProgramPipeline.uniform3i[ProgramPipeline.program].find(location) == ProgramPipeline.uniform3i[ProgramPipeline.program].end() ||
		ProgramPipeline.uniform3i[ProgramPipeline.program][location][0] != v0 ||
		ProgramPipeline.uniform3i[ProgramPipeline.program][location][1] != v1 ||
		ProgramPipeline.uniform3i[ProgramPipeline.program][location][2] != v2) {
		ProgramPipeline.uniform3i[ProgramPipeline.program][location][0] = v0;
		ProgramPipeline.uniform3i[ProgramPipeline.program][location][1] = v1;
		ProgramPipeline.uniform3i[ProgramPipeline.program][location][2] = v2;
		glUniform3i(location, v0, v1, v2);
	}
}

void GLUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{
	if (ProgramPipeline.uniform4i[ProgramPipeline.program].find(location) == ProgramPipeline.uniform4i[ProgramPipeline.program].end() ||
		ProgramPipeline.uniform4i[ProgramPipeline.program][location][0] != v0 ||
		ProgramPipeline.uniform4i[ProgramPipeline.program][location][1] != v1 ||
		ProgramPipeline.uniform4i[ProgramPipeline.program][location][2] != v2 ||
		ProgramPipeline.uniform4i[ProgramPipeline.program][location][3] != v3) {
		ProgramPipeline.uniform4i[ProgramPipeline.program][location][0] = v0;
		ProgramPipeline.uniform4i[ProgramPipeline.program][location][1] = v1;
		ProgramPipeline.uniform4i[ProgramPipeline.program][location][2] = v2;
		ProgramPipeline.uniform4i[ProgramPipeline.program][location][3] = v3;
		glUniform4i(location, v0, v1, v2, v3);
	}
}

void GLUniform1f(GLint location, GLfloat v0)
{
	if (ProgramPipeline.uniform1f[ProgramPipeline.program].find(location) == ProgramPipeline.uniform1f[ProgramPipeline.program].end() ||
		ProgramPipeline.uniform1f[ProgramPipeline.program][location] != v0) {
		ProgramPipeline.uniform1f[ProgramPipeline.program][location] = v0;
		glUniform1f(location, v0);
	}
}

void GLUniform2f(GLint location, GLfloat v0, GLfloat v1)
{
	if (ProgramPipeline.uniform2f[ProgramPipeline.program].find(location) == ProgramPipeline.uniform2f[ProgramPipeline.program].end() ||
		ProgramPipeline.uniform2f[ProgramPipeline.program][location][0] != v0 ||
		ProgramPipeline.uniform2f[ProgramPipeline.program][location][1] != v1) {
		ProgramPipeline.uniform2f[ProgramPipeline.program][location][0] = v0;
		ProgramPipeline.uniform2f[ProgramPipeline.program][location][1] = v1;
		glUniform2f(location, v0, v1);
	}
}

void GLUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
	if (ProgramPipeline.uniform3f[ProgramPipeline.program].find(location) == ProgramPipeline.uniform3f[ProgramPipeline.program].end() ||
		ProgramPipeline.uniform3f[ProgramPipeline.program][location][0] != v0 ||
		ProgramPipeline.uniform3f[ProgramPipeline.program][location][1] != v1 ||
		ProgramPipeline.uniform3f[ProgramPipeline.program][location][2] != v2) {
		ProgramPipeline.uniform3f[ProgramPipeline.program][location][0] = v0;
		ProgramPipeline.uniform3f[ProgramPipeline.program][location][1] = v1;
		ProgramPipeline.uniform3f[ProgramPipeline.program][location][2] = v2;
		glUniform3f(location, v0, v1, v2);
	}
}

void GLUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	if (ProgramPipeline.uniform4f[ProgramPipeline.program].find(location) == ProgramPipeline.uniform4f[ProgramPipeline.program].end() ||
		ProgramPipeline.uniform4f[ProgramPipeline.program][location][0] != v0 ||
		ProgramPipeline.uniform4f[ProgramPipeline.program][location][1] != v1 ||
		ProgramPipeline.uniform4f[ProgramPipeline.program][location][2] != v2 ||
		ProgramPipeline.uniform4f[ProgramPipeline.program][location][3] != v3) {
		ProgramPipeline.uniform4f[ProgramPipeline.program][location][0] = v0;
		ProgramPipeline.uniform4f[ProgramPipeline.program][location][1] = v1;
		ProgramPipeline.uniform4f[ProgramPipeline.program][location][2] = v2;
		ProgramPipeline.uniform4f[ProgramPipeline.program][location][3] = v3;
		glUniform4f(location, v0, v1, v2, v3);
	}
}

void GLUniform1iv(GLint location, GLsizei count, const GLint *value)
{
	eastl::vector<int> values(value, value + count);
	if (ProgramPipeline.uniform1iv[ProgramPipeline.program].find(location) == ProgramPipeline.uniform1iv[ProgramPipeline.program].end() ||
		ProgramPipeline.uniform1iv[ProgramPipeline.program][location] != values) {
		ProgramPipeline.uniform1iv[ProgramPipeline.program][location] = values;
		glUniform1iv(location, count, value);
	}
}

void GLUniform2iv(GLint location, GLsizei count, const GLint *value)
{
	eastl::vector<int> values(value, value + count);
	if (ProgramPipeline.uniform2iv[ProgramPipeline.program].find(location) == ProgramPipeline.uniform2iv[ProgramPipeline.program].end() ||
		ProgramPipeline.uniform2iv[ProgramPipeline.program][location] != values) {
		ProgramPipeline.uniform2iv[ProgramPipeline.program][location] = values;
		glUniform2iv(location, count, value);
	}
}

void GLUniform3iv(GLint location, GLsizei count, const GLint *value)
{
	eastl::vector<int> values(value, value + count);
	if (ProgramPipeline.uniform3iv[ProgramPipeline.program].find(location) == ProgramPipeline.uniform3iv[ProgramPipeline.program].end() ||
		ProgramPipeline.uniform3iv[ProgramPipeline.program][location] != values) {
		ProgramPipeline.uniform3iv[ProgramPipeline.program][location] = values;
		glUniform3iv(location, count, value);
	}
}

void GLUniform4iv(GLint location, GLsizei count, const GLint *value)
{
	eastl::vector<int> values(value, value + count);
	if (ProgramPipeline.uniform4iv[ProgramPipeline.program].find(location) == ProgramPipeline.uniform4iv[ProgramPipeline.program].end() ||
		ProgramPipeline.uniform4iv[ProgramPipeline.program][location] != values) {
		ProgramPipeline.uniform4iv[ProgramPipeline.program][location] = values;
		glUniform4iv(location, count, value);
	}
}

void GLUniform1fv(GLint location, GLsizei count, const GLfloat *value)
{
	eastl::vector<float> values(value, value + count);
	if (ProgramPipeline.uniform1fv[ProgramPipeline.program].find(location) == ProgramPipeline.uniform1fv[ProgramPipeline.program].end() ||
		ProgramPipeline.uniform1fv[ProgramPipeline.program][location] != values) {
		ProgramPipeline.uniform1fv[ProgramPipeline.program][location] = values;
		glUniform1fv(location, count, value);
	}
}

void GLUniform2fv(GLint location, GLsizei count, const GLfloat *value)
{
	eastl::vector<float> values(value, value + count);
	if (ProgramPipeline.uniform2fv[ProgramPipeline.program].find(location) == ProgramPipeline.uniform2fv[ProgramPipeline.program].end() ||
		ProgramPipeline.uniform2fv[ProgramPipeline.program][location] != values) {
		ProgramPipeline.uniform2fv[ProgramPipeline.program][location] = values;
		glUniform2fv(location, count, value);
	}
}

void GLUniform3fv(GLint location, GLsizei count, const GLfloat *value)
{
	eastl::vector<float> values(value, value + count);
	if (ProgramPipeline.uniform3fv[ProgramPipeline.program].find(location) == ProgramPipeline.uniform3fv[ProgramPipeline.program].end() ||
		ProgramPipeline.uniform3fv[ProgramPipeline.program][location] != values) {
		ProgramPipeline.uniform3fv[ProgramPipeline.program][location] = values;
		glUniform3fv(location, count, value);
	}
}

void GLUniform4fv(GLint location, GLsizei count, const GLfloat *value)
{
	eastl::vector<float> values(value, value + count);
	if (ProgramPipeline.uniform4fv[ProgramPipeline.program].find(location) == ProgramPipeline.uniform4fv[ProgramPipeline.program].end() ||
		ProgramPipeline.uniform4fv[ProgramPipeline.program][location] != values) {
		ProgramPipeline.uniform4fv[ProgramPipeline.program][location] = values;
		glUniform4fv(location, count, value);
	}
}

void GLUniformMatrix2fv(GLint location, GLsizei count, const GLfloat *value)
{
	eastl::vector<float> values(value, value + count);
	if (ProgramPipeline.uniformMatrix2fv[ProgramPipeline.program].find(location) == ProgramPipeline.uniformMatrix2fv[ProgramPipeline.program].end() ||
		ProgramPipeline.uniformMatrix2fv[ProgramPipeline.program][location] != values) {
		ProgramPipeline.uniformMatrix2fv[ProgramPipeline.program][location] = values;
		glUniformMatrix2fv(location, count, GL_FALSE, value);
	}
}

void GLUniformMatrix3fv(GLint location, GLsizei count, const GLfloat *value)
{
	eastl::vector<float> values(value, value + count);
	if (ProgramPipeline.uniformMatrix3fv[ProgramPipeline.program].find(location) == ProgramPipeline.uniformMatrix3fv[ProgramPipeline.program].end() ||
		ProgramPipeline.uniformMatrix3fv[ProgramPipeline.program][location] != values) {
		ProgramPipeline.uniformMatrix3fv[ProgramPipeline.program][location] = values;
		glUniformMatrix3fv(location, count, GL_FALSE, value);
	}
}

void GLUniformMatrix4fv(GLint location, GLsizei count, const GLfloat *value)
{
	eastl::vector<float> values(value, value + count);
	if (ProgramPipeline.uniformMatrix4fv[ProgramPipeline.program].find(location) == ProgramPipeline.uniformMatrix4fv[ProgramPipeline.program].end() ||
		ProgramPipeline.uniformMatrix4fv[ProgramPipeline.program][location] != values) {
		ProgramPipeline.uniformMatrix4fv[ProgramPipeline.program][location] = values;
		glUniformMatrix4fv(location, count, GL_FALSE, value);
	}
}

void GLBindProgramPipeline(GLuint pipeline)
{
	if (ProgramPipeline.pipeline != pipeline) {
		ProgramPipeline.pipeline = pipeline;
		ProgramPipeline.uniform1i.clear();
		ProgramPipeline.uniform2i.clear();
		ProgramPipeline.uniform3i.clear();
		ProgramPipeline.uniform4i.clear();
		ProgramPipeline.uniform1f.clear();
		ProgramPipeline.uniform2f.clear();
		ProgramPipeline.uniform3f.clear();
		ProgramPipeline.uniform4f.clear();
		ProgramPipeline.uniform1iv.clear();
		ProgramPipeline.uniform2iv.clear();
		ProgramPipeline.uniform3iv.clear();
		ProgramPipeline.uniform4iv.clear();
		ProgramPipeline.uniform1fv.clear();
		ProgramPipeline.uniform2fv.clear();
		ProgramPipeline.uniform3fv.clear();
		ProgramPipeline.uniform4fv.clear();
		ProgramPipeline.uniformMatrix2fv.clear();
		ProgramPipeline.uniformMatrix3fv.clear();
		ProgramPipeline.uniformMatrix4fv.clear();
		glBindProgramPipeline(pipeline);
	}
}

void GLProgramUniform1i(GLuint program, GLint location, GLint v0)
{
	if (ProgramPipeline.uniform1i[program].find(location) == ProgramPipeline.uniform1i[program].end() ||
		ProgramPipeline.uniform1i[program][location] != v0) {
		ProgramPipeline.uniform1i[program][location] = v0;
		glProgramUniform1i(program, location, v0);
	}
}

void GLProgramUniform2i(GLuint program, GLint location, GLint v0, GLint v1)
{
	if (ProgramPipeline.uniform2i[program].find(location) == ProgramPipeline.uniform2i[program].end() ||
		ProgramPipeline.uniform2i[program][location][0] != v0 ||
		ProgramPipeline.uniform2i[program][location][1] != v1) {
		ProgramPipeline.uniform2i[program][location][0] = v0;
		ProgramPipeline.uniform2i[program][location][1] = v1;
		glProgramUniform2i(program, location, v0, v1);
	}
}

void GLProgramUniform3i(GLuint program, GLint location, GLint v0, GLint v1, GLint v2)
{
	if (ProgramPipeline.uniform3i[program].find(location) == ProgramPipeline.uniform3i[program].end() ||
		ProgramPipeline.uniform3i[program][location][0] != v0 ||
		ProgramPipeline.uniform3i[program][location][1] != v1 ||
		ProgramPipeline.uniform3i[program][location][2] != v2) {
		ProgramPipeline.uniform3i[program][location][0] = v0;
		ProgramPipeline.uniform3i[program][location][1] = v1;
		ProgramPipeline.uniform3i[program][location][2] = v2;
		glProgramUniform3i(program, location, v0, v1, v2);
	}
}

void GLProgramUniform4i(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{
	if (ProgramPipeline.uniform4i[program].find(location) == ProgramPipeline.uniform4i[program].end() ||
		ProgramPipeline.uniform4i[program][location][0] != v0 ||
		ProgramPipeline.uniform4i[program][location][1] != v1 ||
		ProgramPipeline.uniform4i[program][location][2] != v2 ||
		ProgramPipeline.uniform4i[program][location][3] != v3) {
		ProgramPipeline.uniform4i[program][location][0] = v0;
		ProgramPipeline.uniform4i[program][location][1] = v1;
		ProgramPipeline.uniform4i[program][location][2] = v2;
		ProgramPipeline.uniform4i[program][location][3] = v3;
		glProgramUniform4i(program, location, v0, v1, v2, v3);
	}
}

void GLProgramUniform1f(GLuint program, GLint location, GLfloat v0)
{
	if (ProgramPipeline.uniform1f[program].find(location) == ProgramPipeline.uniform1f[program].end() ||
		ProgramPipeline.uniform1f[program][location] != v0) {
		ProgramPipeline.uniform1f[program][location] = v0;
		glProgramUniform1f(program, location, v0);
	}
}

void GLProgramUniform2f(GLuint program, GLint location, GLfloat v0, GLfloat v1)
{
	if (ProgramPipeline.uniform2f[program].find(location) == ProgramPipeline.uniform2f[program].end() ||
		ProgramPipeline.uniform2f[program][location][0] != v0 ||
		ProgramPipeline.uniform2f[program][location][1] != v1) {
		ProgramPipeline.uniform2f[program][location][0] = v0;
		ProgramPipeline.uniform2f[program][location][1] = v1;
		glProgramUniform2f(program, location, v0, v1);
	}
}

void GLProgramUniform3f(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
	if (ProgramPipeline.uniform3f[program].find(location) == ProgramPipeline.uniform3f[program].end() ||
		ProgramPipeline.uniform3f[program][location][0] != v0 ||
		ProgramPipeline.uniform3f[program][location][1] != v1 ||
		ProgramPipeline.uniform3f[program][location][2] != v2) {
		ProgramPipeline.uniform3f[program][location][0] = v0;
		ProgramPipeline.uniform3f[program][location][1] = v1;
		ProgramPipeline.uniform3f[program][location][2] = v2;
		glProgramUniform3f(program, location, v0, v1, v2);
	}
}

void GLProgramUniform4f(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	if (ProgramPipeline.uniform4f[program].find(location) == ProgramPipeline.uniform4f[program].end() ||
		ProgramPipeline.uniform4f[program][location][0] != v0 ||
		ProgramPipeline.uniform4f[program][location][1] != v1 ||
		ProgramPipeline.uniform4f[program][location][2] != v2 ||
		ProgramPipeline.uniform4f[program][location][3] != v3) {
		ProgramPipeline.uniform4f[program][location][0] = v0;
		ProgramPipeline.uniform4f[program][location][1] = v1;
		ProgramPipeline.uniform4f[program][location][2] = v2;
		ProgramPipeline.uniform4f[program][location][3] = v3;
		glProgramUniform4f(program, location, v0, v1, v2, v3);
	}
}

void GLProgramUniform1iv(GLuint program, GLint location, GLsizei count, const GLint *value)
{
	eastl::vector<int> values(value, value + count);
	if (ProgramPipeline.uniform1iv[program].find(location) == ProgramPipeline.uniform1iv[program].end() ||
		ProgramPipeline.uniform1iv[program][location] != values) {
		ProgramPipeline.uniform1iv[program][location] = values;
		glProgramUniform1iv(program, location, count, value);
	}
}

void GLProgramUniform2iv(GLuint program, GLint location, GLsizei count, const GLint *value)
{
	eastl::vector<int> values(value, value + count);
	if (ProgramPipeline.uniform2iv[program].find(location) == ProgramPipeline.uniform2iv[program].end() ||
		ProgramPipeline.uniform2iv[program][location] != values) {
		ProgramPipeline.uniform2iv[program][location] = values;
		glProgramUniform2iv(program, location, count, value);
	}
}

void GLProgramUniform3iv(GLuint program, GLint location, GLsizei count, const GLint *value)
{
	eastl::vector<int> values(value, value + count);
	if (ProgramPipeline.uniform3iv[program].find(location) == ProgramPipeline.uniform3iv[program].end() ||
		ProgramPipeline.uniform3iv[program][location] != values) {
		ProgramPipeline.uniform3iv[program][location] = values;
		glProgramUniform3iv(program, location, count, value);
	}
}

void GLProgramUniform4iv(GLuint program, GLint location, GLsizei count, const GLint *value)
{
	eastl::vector<int> values(value, value + count);
	if (ProgramPipeline.uniform4iv[program].find(location) == ProgramPipeline.uniform4iv[program].end() ||
		ProgramPipeline.uniform4iv[program][location] != values) {
		ProgramPipeline.uniform4iv[program][location] = values;
		glProgramUniform4iv(program, location, count, value);
	}
}

void GLProgramUniform1fv(GLuint program, GLint location, GLsizei count, const GLfloat *value)
{
	eastl::vector<float> values(value, value + count);
	if (ProgramPipeline.uniform1fv[program].find(location) == ProgramPipeline.uniform1fv[program].end() ||
		ProgramPipeline.uniform1fv[program][location] != values) {
		ProgramPipeline.uniform1fv[program][location] = values;
		glProgramUniform1fv(program, location, count, value);
	}
}

void GLProgramUniform2fv(GLuint program, GLint location, GLsizei count, const GLfloat *value)
{
	eastl::vector<float> values(value, value + count);
	if (ProgramPipeline.uniform2fv[program].find(location) == ProgramPipeline.uniform2fv[program].end() ||
		ProgramPipeline.uniform2fv[program][location] != values) {
		ProgramPipeline.uniform2fv[program][location] = values;
		glProgramUniform2fv(program, location, count, value);
	}
}

void GLProgramUniform3fv(GLuint program, GLint location, GLsizei count, const GLfloat *value)
{
	eastl::vector<float> values(value, value + count);
	if (ProgramPipeline.uniform3fv[program].find(location) == ProgramPipeline.uniform3fv[program].end() ||
		ProgramPipeline.uniform3fv[program][location] != values) {
		ProgramPipeline.uniform3fv[program][location] = values;
		glProgramUniform3fv(program, location, count, value);
	}
}

void GLProgramUniform4fv(GLuint program, GLint location, GLsizei count, const GLfloat *value)
{
	eastl::vector<float> values(value, value + count);
	if (ProgramPipeline.uniform4fv[program].find(location) == ProgramPipeline.uniform4fv[program].end() ||
		ProgramPipeline.uniform4fv[program][location] != values) {
		ProgramPipeline.uniform4fv[program][location] = values;
		glProgramUniform4fv(program, location, count, value);
	}
}

void GLProgramUniformMatrix2fv(GLuint program, GLint location, GLsizei count, const GLfloat *value)
{
	eastl::vector<float> values(value, value + count);
	if (ProgramPipeline.uniformMatrix2fv[program].find(location) == ProgramPipeline.uniformMatrix2fv[program].end() ||
		ProgramPipeline.uniformMatrix2fv[program][location] != values) {
		ProgramPipeline.uniformMatrix2fv[program][location] = values;
		glProgramUniformMatrix2fv(program, location, count, GL_FALSE, value);
	}
}

void GLProgramUniformMatrix3fv(GLuint program, GLint location, GLsizei count, const GLfloat *value)
{
	eastl::vector<float> values(value, value + count);
	if (ProgramPipeline.uniformMatrix3fv[program].find(location) == ProgramPipeline.uniformMatrix3fv[program].end() ||
		ProgramPipeline.uniformMatrix3fv[program][location] != values) {
		ProgramPipeline.uniformMatrix3fv[program][location] = values;
		glProgramUniformMatrix3fv(program, location, count, GL_FALSE, value);
	}
}

void GLProgramUniformMatrix4fv(GLuint program, GLint location, GLsizei count, const GLfloat *value)
{
	eastl::vector<float> values(value, value + count);
	if (ProgramPipeline.uniformMatrix4fv[program].find(location) == ProgramPipeline.uniformMatrix4fv[program].end() ||
		ProgramPipeline.uniformMatrix4fv[program][location] != values) {
		ProgramPipeline.uniformMatrix4fv[program][location] = values;
		glProgramUniformMatrix4fv(program, location, count, GL_FALSE, value);
	}
}
#pragma endregion
