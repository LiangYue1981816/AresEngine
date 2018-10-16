#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxExtension.h"


#pragma region OpenGL extension
void glReadBuffers(int n, const uint32_t *bufs)
{
	for (int index = 0; index < n; index++) {
		glReadBuffer(bufs[index]);
	}
}

uint32_t glGetShaderType(shaderc_shader_kind kind)
{
	switch (kind) {
	case shaderc_vertex_shader:
		return GL_VERTEX_SHADER;
	case shaderc_fragment_shader:
		return GL_FRAGMENT_SHADER;
	case shaderc_compute_shader:
		return GL_COMPUTE_SHADER;
	default:
		return GL_INVALID_ENUM;
	}
}

uint32_t glGetProgramStage(shaderc_shader_kind kind)
{
	switch (kind) {
	case shaderc_vertex_shader:
		return GL_VERTEX_SHADER_BIT;
	case shaderc_fragment_shader:
		return GL_FRAGMENT_SHADER_BIT;
	case shaderc_compute_shader:
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
	uint32_t target;
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
	uint32_t pipeline;
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

void GLInitState(GLstate *state)
{
	state->bEnableCullFace = GL_TRUE;
	state->cullFace = GL_BACK;
	state->frontFace = GL_CCW;

	state->bEnableStencilTest = GL_FALSE;
	state->stencilFunc = GL_ALWAYS;
	state->stencilRef = 0;
	state->stencilMask = 1;
	state->stencilOpSFail = GL_KEEP;
	state->stencilOpDFail = GL_KEEP;
	state->stencilOpDPass = GL_KEEP;

	state->bEnableDepthTest = GL_TRUE;
	state->bEnableDepthWrite = GL_TRUE;
	state->depthFunc = GL_LESS;
	state->depthRangeNear = 0.0f;
	state->depthRangeFar = 1.0f;

	state->bEnableColorWrite[0] = GL_TRUE;
	state->bEnableColorWrite[1] = GL_TRUE;
	state->bEnableColorWrite[2] = GL_TRUE;
	state->bEnableColorWrite[3] = GL_TRUE;

	state->bEnableAlphaToCoverage = GL_FALSE;
	state->bEnableBlend = GL_FALSE;
	state->blendSrcFactor = GL_ONE;
	state->blendDstFactor = GL_ZERO;
	state->blendEquation = GL_FUNC_ADD;
	state->blendColor[0] = 0.0f;
	state->blendColor[1] = 0.0f;
	state->blendColor[2] = 0.0f;
	state->blendColor[3] = 0.0f;

	state->bEnablePolygonOffset = GL_FALSE;
	state->polygonOffsetFactor = 0.0f;
	state->polygonOffsetUnits = 0.0f;
}

void GLBindState(const GLstate *state)
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

void GLBindProgramPipeline(GLuint pipeline)
{
	if (ProgramPipeline.pipeline != pipeline) {
		ProgramPipeline.pipeline = pipeline;
		glBindProgramPipeline(pipeline);
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
		if (FrameBuffers[target].attachments[framebuffer].find(attachment) == FrameBuffers[target].attachments[framebuffer].end() || FrameBuffers[target].attachments[framebuffer][attachment].target != textarget || FrameBuffers[target].attachments[framebuffer][attachment].texture != texture || FrameBuffers[target].attachments[framebuffer][attachment].level != level) {
			FrameBuffers[target].attachments[framebuffer][attachment].target = textarget;
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
#pragma endregion
