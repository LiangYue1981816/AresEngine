#include "GLES3Renderer.h"


#pragma region OpenGL extension
uint32_t glGetShaderType(shader_kind kind)
{
	switch ((int)kind) {
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
	switch ((int)kind) {
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

void glReadBuffers(int n, const uint32_t* bufs)
{
	for (int indexBuffer = 0; indexBuffer < n; indexBuffer++) {
		glReadBuffer(bufs[indexBuffer]);
	}
}

void glMapBufferRangeAddress(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access, void** addr)
{
	*addr = glMapBufferRange(target, offset, length, access);
}

void glBufferSubDataSync(GLenum target, GLintptr offset, GLsizeiptr size, const void *data, bool sync)
{
	//*
	void* addr = nullptr;
	glMapBufferRangeAddress(target, offset, size, sync ? GL_MAP_WRITE_BIT : GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT, &addr);
	memcpy(addr, data, size);
	glUnmapBuffer(target);
	/*/
	glBufferSubData(target, offset, size, data);
	//*/
}
#pragma endregion

#pragma region OpenGL state cache
typedef struct ScissorParam {
	void Reset(void)
	{
		x = GL_INVALID_VALUE;
		y = GL_INVALID_VALUE;
		width = GL_INVALID_VALUE;
		height = GL_INVALID_VALUE;
	}

	int x;
	int y;
	int width;
	int height;
} ScissorParam;

typedef struct ViewportParam {
	void Reset(void)
	{
		x = GL_INVALID_VALUE;
		y = GL_INVALID_VALUE;
		width = GL_INVALID_VALUE;
		height = GL_INVALID_VALUE;
		znear = 0.0f;
		zfar = 1.0f;
	}

	int x;
	int y;
	int width;
	int height;
	float znear;
	float zfar;
} ViewportParam;

typedef struct ClearColorParam {
	void Reset(void)
	{
		red = GL_INVALID_VALUE;
		green = GL_INVALID_VALUE;
		blue = GL_INVALID_VALUE;
		alpha = GL_INVALID_VALUE;
	}

	float red;
	float green;
	float blue;
	float alpha;
} ClearColorParam;

typedef struct ClearDepthParam {
	void Reset(void)
	{
		depth = GL_INVALID_VALUE;
	}

	float depth;
} ClearDepthParam;

typedef struct ClearStencilParam {
	void Reset(void)
	{
		stencil = GL_INVALID_VALUE;
	}

	int stencil;
} ClearStencilParam;

typedef struct PolygonModeParam {
	void Reset(void)
	{
		mode = GL_INVALID_VALUE;
	}

	uint32_t mode;
} PolygonModeParam;

typedef struct CullFaceParam {
	void Reset(void)
	{
		mode = GL_INVALID_VALUE;
	}

	uint32_t mode;
} CullFaceParam;

typedef struct FrontFaceParam {
	void Reset(void)
	{
		mode = GL_INVALID_VALUE;
	}

	uint32_t mode;
} FrontFaceParam;

typedef struct LineWidthParam {
	void Reset(void)
	{
		width = GL_INVALID_VALUE;
	}

	float width;
} LineWidthParam;

typedef struct PolygonOffsetParam {
	void Reset(void)
	{
		factor = GL_INVALID_VALUE;
		units = GL_INVALID_VALUE;
	}

	float factor;
	float units;
} PolygonOffsetParam;

typedef struct DepthRangefParam {
	void Reset(void)
	{
		n = GL_INVALID_VALUE;
		f = GL_INVALID_VALUE;
	}

	float n;
	float f;
} DepthRangefParam;

typedef struct DepthFuncParam {
	void Reset(void)
	{
		func = GL_INVALID_VALUE;
	}

	uint32_t func;
} DepthFuncParam;

typedef struct DepthMaskParam {
	void Reset(void)
	{
		flag = GL_INVALID_VALUE;
	}

	uint32_t flag;
} DepthMaskParam;

typedef struct ColorMaskParam {
	void Reset(void)
	{
		red = GL_INVALID_VALUE;
		green = GL_INVALID_VALUE;
		blue = GL_INVALID_VALUE;
		alpha = GL_INVALID_VALUE;
	}

	uint32_t red;
	uint32_t green;
	uint32_t blue;
	uint32_t alpha;
} ColorMaskParam;

typedef struct StencilOpParam {
	void Reset(void)
	{
		sfail = GL_INVALID_VALUE;
		dpfail = GL_INVALID_VALUE;
		dppass = GL_INVALID_VALUE;
	}

	uint32_t sfail;
	uint32_t dpfail;
	uint32_t dppass;
} StencilOpParam;

typedef struct StencilFuncParam {
	void Reset(void)
	{
		func = GL_INVALID_VALUE;
		ref = GL_INVALID_VALUE;
		mask = GL_INVALID_VALUE;
	}

	uint32_t func;
	uint32_t ref;
	uint32_t mask;
} StencilFuncParam;

typedef struct StencilMaskParam {
	void Reset(void)
	{
		mask = GL_INVALID_VALUE;
	}

	uint32_t mask;
} StencilMaskParam;

typedef struct BlendFuncParam {
	void Reset(void)
	{
		srcRGB = GL_INVALID_VALUE;
		dstRGB = GL_INVALID_VALUE;
		srcAlpha = GL_INVALID_VALUE;
		dstAlpha = GL_INVALID_VALUE;
	}

	uint32_t srcRGB;
	uint32_t dstRGB;
	uint32_t srcAlpha;
	uint32_t dstAlpha;
} BlendFuncParam;

typedef struct BlendEquationParam {
	void Reset(void)
	{
		modeRGB = GL_INVALID_VALUE;
		modeAlpha = GL_INVALID_VALUE;
	}

	uint32_t modeRGB;
	uint32_t modeAlpha;
} BlendEquationParam;

typedef struct BlendColorParam {
	void Reset(void)
	{
		red = GL_INVALID_VALUE;
		green = GL_INVALID_VALUE;
		blue = GL_INVALID_VALUE;
		alpha = GL_INVALID_VALUE;
	}

	float red;
	float green;
	float blue;
	float alpha;
} BlendColorParam;

typedef struct FrameBufferAttachmentParam {
	void Reset(void)
	{
		textarget = GL_INVALID_VALUE;
		texture = GL_INVALID_VALUE;
		level = GL_INVALID_VALUE;
	}

	uint32_t textarget;
	uint32_t texture;
	int level;
} FrameBufferAttachmentParam;

typedef struct FrameBufferParam {
	void Reset(void)
	{
		framebuffer = GL_INVALID_VALUE;
		attachments.clear();
		readbuffers.clear();
		drawbuffers.clear();
	}

	uint32_t framebuffer;
	eastl::unordered_map<uint32_t, eastl::unordered_map<uint32_t, FrameBufferAttachmentParam>> attachments;
	eastl::unordered_map<uint32_t, eastl::vector<uint32_t>> readbuffers;
	eastl::unordered_map<uint32_t, eastl::vector<uint32_t>> drawbuffers;
} FrameBufferParam;

typedef struct ProgramParam {
	void Reset(void)
	{
		program = GL_INVALID_VALUE;
		uniform1i.clear();
		uniform2i.clear();
		uniform3i.clear();
		uniform4i.clear();
		uniform1f.clear();
		uniform2f.clear();
		uniform3f.clear();
		uniform4f.clear();
		uniform1iv.clear();
		uniform2iv.clear();
		uniform3iv.clear();
		uniform4iv.clear();
		uniform1fv.clear();
		uniform2fv.clear();
		uniform3fv.clear();
		uniform4fv.clear();
		uniformMatrix2fv.clear();
		uniformMatrix3fv.clear();
		uniformMatrix4fv.clear();
	}

	uint32_t program;
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
} ProgramParam;

typedef struct VertexArrayParam {
	void Reset(void)
	{
		array = GL_INVALID_VALUE;
	}

	uint32_t array;
} VertexArrayParam;

typedef struct BufferBaseParam {
	void Reset(void)
	{
		index = GL_INVALID_VALUE;
		buffer = GL_INVALID_VALUE;
	}

	uint32_t index;
	uint32_t buffer;
} BufferBaseParam;

typedef struct BufferRangeParam {
	void Reset(void)
	{
		index = GL_INVALID_VALUE;
		buffer = GL_INVALID_VALUE;
		offset = GL_INVALID_VALUE;
		size = GL_INVALID_VALUE;
	}

	uint32_t index;
	uint32_t buffer;
	int offset;
	int size;
} BufferRangeParam;

typedef struct TextureParam {
	void Reset(void)
	{
		target = GL_INVALID_VALUE;
		texture = GL_INVALID_VALUE;
	}

	uint32_t target;
	uint32_t texture;
} TextureParam;

typedef struct ImageParam {
	void Reset(void)
	{
		texture = GL_INVALID_VALUE;
		level = GL_INVALID_VALUE;
		layer = GL_INVALID_VALUE;
		access = GL_INVALID_VALUE;
		format = GL_INVALID_VALUE;
	}

	uint32_t texture;
	uint32_t level;
	uint32_t layer;
	uint32_t access;
	uint32_t format;
} ImageParam;

static ScissorParam Scissor;
static ViewportParam Viewport;
static ClearColorParam ClearColor;
static ClearDepthParam ClearDepth;
static ClearStencilParam ClearStencil;
static PolygonModeParam PolygonMode;
static CullFaceParam CullFace;
static FrontFaceParam FrontFace;
static LineWidthParam LineWidth;
static PolygonOffsetParam PolygonOffset;
static DepthRangefParam DepthRangef;
static DepthFuncParam DepthFunc;
static DepthMaskParam DepthMask;
static ColorMaskParam ColorMask;
static StencilOpParam StencilFrontOp;
static StencilFuncParam StencilFrontFunc;
static StencilMaskParam StencilFrontMask;
static StencilOpParam StencilBackOp;
static StencilFuncParam StencilBackFunc;
static StencilMaskParam StencilBackMask;
static BlendFuncParam BlendFunc;
static BlendEquationParam BlendEquation;
static BlendColorParam BlendColor;
static VertexArrayParam VertexArray;
static ProgramParam Program;

static eastl::unordered_map<GLenum, GLboolean> Caps;
static eastl::unordered_map<GLenum, GLuint> Buffers;
static eastl::unordered_map<GLenum, BufferBaseParam> BufferBases;
static eastl::unordered_map<GLenum, BufferRangeParam> BufferRanges;
static eastl::unordered_map<GLenum, FrameBufferParam> FrameBuffers;
static eastl::unordered_map<GLuint, GLuint> Samplers;
static eastl::unordered_map<GLuint, TextureParam> Textures;
static eastl::unordered_map<GLuint, ImageParam> Images;

void GLResetContext(void)
{
	Scissor.Reset();
	Viewport.Reset();
	ClearColor.Reset();
	ClearDepth.Reset();
	ClearStencil.Reset();
	PolygonMode.Reset();
	CullFace.Reset();
	FrontFace.Reset();
	LineWidth.Reset();
	PolygonOffset.Reset();
	DepthRangef.Reset();
	DepthFunc.Reset();
	DepthMask.Reset();
	ColorMask.Reset();
	StencilFrontOp.Reset();
	StencilFrontFunc.Reset();
	StencilFrontMask.Reset();
	StencilBackOp.Reset();
	StencilBackFunc.Reset();
	StencilBackMask.Reset();
	BlendFunc.Reset();
	BlendEquation.Reset();
	BlendColor.Reset();
	VertexArray.Reset();
	Program.Reset();

	Caps.clear();
	Buffers.clear();
	BufferBases.clear();
	BufferRanges.clear();
	FrameBuffers.clear();
	Samplers.clear();
	Textures.clear();
	Images.clear();
}

void GLEnable(GLenum cap)
{
	switch ((int)cap) {
	case GL_BLEND:
	case GL_CULL_FACE:
	case GL_DEPTH_TEST:
	case GL_DITHER:
	case GL_POLYGON_OFFSET_FILL:
	case GL_PRIMITIVE_RESTART_FIXED_INDEX:
	case GL_RASTERIZER_DISCARD:
	case GL_SAMPLE_ALPHA_TO_COVERAGE:
	case GL_SAMPLE_COVERAGE:
	case GL_SCISSOR_TEST:
	case GL_STENCIL_TEST:
	case GL_SAMPLE_MASK:
		if (Caps.find(cap) == Caps.end() || Caps[cap] != GL_TRUE) {
			Caps[cap] = GL_TRUE;
			glEnable(cap);
		}
		break;
	}
}

void GLDisable(GLenum cap)
{
	switch ((int)cap) {
	case GL_BLEND:
	case GL_CULL_FACE:
	case GL_DEPTH_TEST:
	case GL_DITHER:
	case GL_POLYGON_OFFSET_FILL:
	case GL_PRIMITIVE_RESTART_FIXED_INDEX:
	case GL_RASTERIZER_DISCARD:
	case GL_SAMPLE_ALPHA_TO_COVERAGE:
	case GL_SAMPLE_COVERAGE:
	case GL_SCISSOR_TEST:
	case GL_STENCIL_TEST:
	case GL_SAMPLE_MASK:
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

void GLDepthRange(GLfloat znear, GLfloat zfar)
{
	if (Viewport.znear != znear || Viewport.zfar != zfar) {
		Viewport.znear = znear;
		Viewport.zfar = zfar;
		glDepthRangef(znear, zfar);
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

void GLPolygonMode(GLenum mode)
{
	if (PolygonMode.mode != mode) {
		PolygonMode.mode = mode;
		glPolygonMode(GL_FRONT_AND_BACK, mode);
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

void GLStencilFrontOp(GLenum sfail, GLenum dpfail, GLenum dppass)
{
	if (StencilFrontOp.sfail != sfail || StencilFrontOp.dpfail != dpfail || StencilFrontOp.dppass != dppass) {
		StencilFrontOp.sfail = sfail;
		StencilFrontOp.dpfail = dpfail;
		StencilFrontOp.dppass = dppass;
		glStencilOpSeparate(GL_FRONT, sfail, dpfail, dppass);
	}
}

void GLStencilFrontFunc(GLenum func, GLint ref, GLuint mask)
{
	if (StencilFrontFunc.func != func || StencilFrontFunc.ref != ref || StencilFrontFunc.mask != mask) {
		StencilFrontFunc.func = func;
		StencilFrontFunc.ref = ref;
		StencilFrontFunc.mask = mask;
		glStencilFuncSeparate(GL_FRONT, func, ref, mask);
	}
}

void GLStencilFrontMask(GLuint mask)
{
	if (StencilFrontMask.mask != mask) {
		StencilFrontMask.mask = mask;
		glStencilMaskSeparate(GL_FRONT, mask);
	}
}

void GLStencilBackOp(GLenum sfail, GLenum dpfail, GLenum dppass)
{
	if (StencilBackOp.sfail != sfail || StencilBackOp.dpfail != dpfail || StencilBackOp.dppass != dppass) {
		StencilBackOp.sfail = sfail;
		StencilBackOp.dpfail = dpfail;
		StencilBackOp.dppass = dppass;
		glStencilOpSeparate(GL_BACK, sfail, dpfail, dppass);
	}
}

void GLStencilBackFunc(GLenum func, GLint ref, GLuint mask)
{
	if (StencilBackFunc.func != func || StencilBackFunc.ref != ref || StencilBackFunc.mask != mask) {
		StencilBackFunc.func = func;
		StencilBackFunc.ref = ref;
		StencilBackFunc.mask = mask;
		glStencilFuncSeparate(GL_BACK, func, ref, mask);
	}
}

void GLStencilBackMask(GLuint mask)
{
	if (StencilBackMask.mask != mask) {
		StencilBackMask.mask = mask;
		glStencilMaskSeparate(GL_BACK, mask);
	}
}

void GLBlendFunc(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
{
	if (BlendFunc.srcRGB != srcRGB || BlendFunc.dstRGB != dstRGB || BlendFunc.srcAlpha != srcAlpha || BlendFunc.dstAlpha != dstAlpha) {
		BlendFunc.srcRGB = srcRGB;
		BlendFunc.dstRGB = dstRGB;
		BlendFunc.srcAlpha = srcAlpha;
		BlendFunc.dstAlpha = dstAlpha;
		glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
	}
}

void GLBlendEquation(GLenum modeRGB, GLenum modeAlpha)
{
	if (BlendEquation.modeRGB != modeRGB || BlendEquation.modeAlpha != modeAlpha) {
		BlendEquation.modeRGB = modeRGB;
		BlendEquation.modeAlpha = modeAlpha;
		glBlendEquationSeparate(modeRGB, modeAlpha);
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

void GLBindState(const PipelineState* state)
{
	// Input Assembly State
	if (state->bEnablePrimitiveRestart) {
		GLEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	}
	else {
		GLDisable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	}

	// Rasterization State
	if (state->bEnableRasterizerDiscard) {
		GLEnable(GL_RASTERIZER_DISCARD);
	}
	else {
		GLDisable(GL_RASTERIZER_DISCARD);
	}

	if (state->bEnableCullFace) {
		GLEnable(GL_CULL_FACE);
		GLCullFace(CGLES3Helper::TranslateCullFace(state->cullFace));
		GLFrontFace(CGLES3Helper::TranslateFrontFace(state->frontFace));
	}
	else {
		GLDisable(GL_CULL_FACE);
	}

	if (state->bEnableDepthBias) {
		GLEnable(GL_POLYGON_OFFSET_FILL);
		GLPolygonOffset(state->depthBiasSlopeFactor, state->depthBiasConstantFactor);
	}
	else {
		GLDisable(GL_POLYGON_OFFSET_FILL);
	}

	GLPolygonMode(CGLES3Helper::TranslatePolytonMode(state->polygonMode));

	// Multisample State
	if (state->bEnableAlphaToCoverage) {
		GLEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	}
	else {
		GLDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	}

	// Depth Stencil State
	if (state->bEnableDepthTest) {
		GLEnable(GL_DEPTH_TEST);
		GLDepthFunc(CGLES3Helper::TranslateCompareOp(state->depthCompareOp));
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

	if (state->bEnableStencilTest) {
		GLEnable(GL_STENCIL_TEST);
		GLStencilFrontOp(
			CGLES3Helper::TranslateStencilOp(state->stencilFrontOpSFail),
			CGLES3Helper::TranslateStencilOp(state->stencilFrontOpDFail),
			CGLES3Helper::TranslateStencilOp(state->stencilFrontOpDPass));
		GLStencilFrontFunc(
			CGLES3Helper::TranslateCompareOp(state->stencilFrontCompareOp),
			state->stencilFrontCompareRef,
			state->stencilFrontCompareMask);
		GLStencilFrontMask(state->stencilFrontWriteMask);
		GLStencilBackOp(
			CGLES3Helper::TranslateStencilOp(state->stencilBackOpSFail),
			CGLES3Helper::TranslateStencilOp(state->stencilBackOpDFail),
			CGLES3Helper::TranslateStencilOp(state->stencilBackOpDPass));
		GLStencilBackFunc(
			CGLES3Helper::TranslateCompareOp(state->stencilBackCompareOp),
			state->stencilBackCompareRef,
			state->stencilBackCompareMask);
		GLStencilBackMask(state->stencilBackWriteMask);
	}
	else {
		GLDisable(GL_STENCIL_TEST);
	}

	// Color Blend State
	if (state->bEnableBlend) {
		GLEnable(GL_BLEND);
		GLBlendFunc(
			CGLES3Helper::TranslateBlendFactor(state->blendSrcRGB),
			CGLES3Helper::TranslateBlendFactor(state->blendDstRGB),
			CGLES3Helper::TranslateBlendFactor(state->blendSrcAlpha),
			CGLES3Helper::TranslateBlendFactor(state->blendDstAlpha));
		GLBlendEquation(
			CGLES3Helper::TranslateBlendOp(state->blendOpRGB),
			CGLES3Helper::TranslateBlendOp(state->blendOpAlpha));
		GLBlendColor(
			state->blendColorRed,
			state->blendColorGreen,
			state->blendColorBlue,
			state->blendColorAlpha);
	}
	else {
		GLDisable(GL_BLEND);
	}

	GLColorMask(state->bEnableColorRedWrite, state->bEnableColorGreenWrite, state->bEnableColorBlueWrite, state->bEnableColorAlphaWrite);
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
	switch ((int)target) {
	case GL_ARRAY_BUFFER:
	case GL_COPY_READ_BUFFER:
	case GL_COPY_WRITE_BUFFER:
	case GL_ELEMENT_ARRAY_BUFFER:
	case GL_PIXEL_PACK_BUFFER:
	case GL_PIXEL_UNPACK_BUFFER:
	case GL_TRANSFORM_FEEDBACK_BUFFER:
	case GL_UNIFORM_BUFFER:
	case GL_ATOMIC_COUNTER_BUFFER:
	case GL_DRAW_INDIRECT_BUFFER:
	case GL_DISPATCH_INDIRECT_BUFFER:
	case GL_SHADER_STORAGE_BUFFER:
		if (Buffers.find(target) == Buffers.end() || Buffers[target] != buffer) {
			Buffers[target] = buffer;
			glBindBuffer(target, buffer);
		}
		break;
	}
}

void GLBindBufferBase(GLenum target, GLuint index, GLuint buffer)
{
	switch ((int)target) {
	case GL_TRANSFORM_FEEDBACK_BUFFER:
	case GL_UNIFORM_BUFFER:
	case GL_ATOMIC_COUNTER_BUFFER:
	case GL_SHADER_STORAGE_BUFFER:
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
	switch ((int)target) {
	case GL_TRANSFORM_FEEDBACK_BUFFER:
	case GL_UNIFORM_BUFFER:
	case GL_ATOMIC_COUNTER_BUFFER:
	case GL_SHADER_STORAGE_BUFFER:
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
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(target, texture);
	}
}

void GLBindImageTexture(GLuint unit, GLuint texture, GLint level, GLint layer, GLenum access, GLenum format)
{
	if (Images.find(unit) == Images.end() || Images[unit].texture != texture || Images[unit].level != level || Images[unit].layer != layer || Images[unit].access != access || Images[unit].format != format) {
		Images[unit].texture = texture;
		Images[unit].level = level;
		Images[unit].layer = layer;
		Images[unit].access = access;
		Images[unit].format = format;
		glBindImageTexture(unit, texture, level, layer >= 0 ? GL_FALSE : GL_TRUE, layer, access, format);
	}
}

void GLInitFramebuffer(GLuint framebuffer)
{
	const uint32_t targets[3] = { GL_DRAW_FRAMEBUFFER, GL_READ_FRAMEBUFFER, GL_FRAMEBUFFER };

	for (int indexTarget = 0; indexTarget < 3; indexTarget++) {
		const uint32_t target = targets[indexTarget];
		const auto &itFrameBufferAttachment = FrameBuffers[target].attachments.find(framebuffer);
		const auto &itFrameBufferDraw = FrameBuffers[target].drawbuffers.find(framebuffer);
		const auto &itFrameBufferRead = FrameBuffers[target].readbuffers.find(framebuffer);

		if (FrameBuffers[target].framebuffer == framebuffer) {
			FrameBuffers[target].framebuffer = 0;
		}

		if (itFrameBufferAttachment != FrameBuffers[target].attachments.end()) {
			FrameBuffers[target].attachments.erase(itFrameBufferAttachment);
		}

		if (itFrameBufferDraw != FrameBuffers[target].drawbuffers.end()) {
			FrameBuffers[target].drawbuffers.erase(itFrameBufferDraw);
		}

		if (itFrameBufferRead != FrameBuffers[target].readbuffers.end()) {
			FrameBuffers[target].readbuffers.erase(itFrameBufferRead);
		}
	}
}

void GLBindFramebuffer(GLenum target, GLuint framebuffer)
{
	switch ((int)target) {
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
		if (FrameBuffers[target].attachments[framebuffer].find(attachment) == FrameBuffers[target].attachments[framebuffer].end() || 
			FrameBuffers[target].attachments[framebuffer][attachment].textarget != textarget || 
			FrameBuffers[target].attachments[framebuffer][attachment].texture != texture || 
			FrameBuffers[target].attachments[framebuffer][attachment].level != level) {
			FrameBuffers[target].attachments[framebuffer][attachment].textarget = textarget;
			FrameBuffers[target].attachments[framebuffer][attachment].texture = texture;
			FrameBuffers[target].attachments[framebuffer][attachment].level = level;
			glFramebufferTexture2D(target, attachment, textarget, texture, level);
		}
	}
}

void GLReadBuffers(GLenum target, GLsizei n, const GLenum* bufs)
{
	if (FrameBuffers.find(target) != FrameBuffers.end()) {
		bool bReset = false;
		uint32_t framebuffer = FrameBuffers[target].framebuffer;

		if (n != FrameBuffers[target].readbuffers[framebuffer].size()) {
			bReset = true;
		}
		else {
			for (int indexBuffer = 0; indexBuffer < n; indexBuffer++) {
				if (bufs[indexBuffer] != FrameBuffers[target].readbuffers[framebuffer][indexBuffer]) {
					bReset = true;
					break;
				}
			}
		}

		if (bReset) {
			FrameBuffers[target].readbuffers[framebuffer].clear();

			for (int indexBuffer = 0; indexBuffer < n; indexBuffer++) {
				FrameBuffers[target].readbuffers[framebuffer].emplace_back(bufs[indexBuffer]);
			}

			glReadBuffers(n, bufs);
		}
	}
	else {
		glReadBuffers(n, bufs);
	}
}

void GLDrawBuffers(GLenum target, GLsizei n, const GLenum* bufs)
{
	if (FrameBuffers.find(target) != FrameBuffers.end()) {
		bool bReset = false;
		uint32_t framebuffer = FrameBuffers[target].framebuffer;

		if (n != FrameBuffers[target].drawbuffers[framebuffer].size()) {
			bReset = true;
		}
		else {
			for (int indexBuffer = 0; indexBuffer < n; indexBuffer++) {
				if (bufs[indexBuffer] != FrameBuffers[target].drawbuffers[framebuffer][indexBuffer]) {
					bReset = true;
					break;
				}
			}
		}

		if (bReset) {
			FrameBuffers[target].drawbuffers[framebuffer].clear();

			for (int indexBuffer = 0; indexBuffer < n; indexBuffer++) {
				FrameBuffers[target].drawbuffers[framebuffer].emplace_back(bufs[indexBuffer]);
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
	if (Program.program != program) {
		Program.program = program;
		Program.uniform1i[program].clear();
		Program.uniform2i[program].clear();
		Program.uniform3i[program].clear();
		Program.uniform4i[program].clear();
		Program.uniform1f[program].clear();
		Program.uniform2f[program].clear();
		Program.uniform3f[program].clear();
		Program.uniform4f[program].clear();
		Program.uniform1iv[program].clear();
		Program.uniform2iv[program].clear();
		Program.uniform3iv[program].clear();
		Program.uniform4iv[program].clear();
		Program.uniform1fv[program].clear();
		Program.uniform2fv[program].clear();
		Program.uniform3fv[program].clear();
		Program.uniform4fv[program].clear();
		Program.uniformMatrix2fv[program].clear();
		Program.uniformMatrix3fv[program].clear();
		Program.uniformMatrix4fv[program].clear();
		glUseProgram(program);
	}
}

void GLUniform1i(GLint location, GLint v0)
{
	if (Program.uniform1i[Program.program].find(location) == Program.uniform1i[Program.program].end() ||
		Program.uniform1i[Program.program][location] != v0) {
		Program.uniform1i[Program.program][location] = v0;
		glUniform1i(location, v0);
	}
}

void GLUniform2i(GLint location, GLint v0, GLint v1)
{
	if (Program.uniform2i[Program.program].find(location) == Program.uniform2i[Program.program].end() ||
		Program.uniform2i[Program.program][location][0] != v0 ||
		Program.uniform2i[Program.program][location][1] != v1) {
		Program.uniform2i[Program.program][location][0] = v0;
		Program.uniform2i[Program.program][location][1] = v1;
		glUniform2i(location, v0, v1);
	}
}

void GLUniform3i(GLint location, GLint v0, GLint v1, GLint v2)
{
	if (Program.uniform3i[Program.program].find(location) == Program.uniform3i[Program.program].end() ||
		Program.uniform3i[Program.program][location][0] != v0 ||
		Program.uniform3i[Program.program][location][1] != v1 ||
		Program.uniform3i[Program.program][location][2] != v2) {
		Program.uniform3i[Program.program][location][0] = v0;
		Program.uniform3i[Program.program][location][1] = v1;
		Program.uniform3i[Program.program][location][2] = v2;
		glUniform3i(location, v0, v1, v2);
	}
}

void GLUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{
	if (Program.uniform4i[Program.program].find(location) == Program.uniform4i[Program.program].end() ||
		Program.uniform4i[Program.program][location][0] != v0 ||
		Program.uniform4i[Program.program][location][1] != v1 ||
		Program.uniform4i[Program.program][location][2] != v2 ||
		Program.uniform4i[Program.program][location][3] != v3) {
		Program.uniform4i[Program.program][location][0] = v0;
		Program.uniform4i[Program.program][location][1] = v1;
		Program.uniform4i[Program.program][location][2] = v2;
		Program.uniform4i[Program.program][location][3] = v3;
		glUniform4i(location, v0, v1, v2, v3);
	}
}

void GLUniform1f(GLint location, GLfloat v0)
{
	if (Program.uniform1f[Program.program].find(location) == Program.uniform1f[Program.program].end() ||
		Program.uniform1f[Program.program][location] != v0) {
		Program.uniform1f[Program.program][location] = v0;
		glUniform1f(location, v0);
	}
}

void GLUniform2f(GLint location, GLfloat v0, GLfloat v1)
{
	if (Program.uniform2f[Program.program].find(location) == Program.uniform2f[Program.program].end() ||
		Program.uniform2f[Program.program][location][0] != v0 ||
		Program.uniform2f[Program.program][location][1] != v1) {
		Program.uniform2f[Program.program][location][0] = v0;
		Program.uniform2f[Program.program][location][1] = v1;
		glUniform2f(location, v0, v1);
	}
}

void GLUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
	if (Program.uniform3f[Program.program].find(location) == Program.uniform3f[Program.program].end() ||
		Program.uniform3f[Program.program][location][0] != v0 ||
		Program.uniform3f[Program.program][location][1] != v1 ||
		Program.uniform3f[Program.program][location][2] != v2) {
		Program.uniform3f[Program.program][location][0] = v0;
		Program.uniform3f[Program.program][location][1] = v1;
		Program.uniform3f[Program.program][location][2] = v2;
		glUniform3f(location, v0, v1, v2);
	}
}

void GLUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	if (Program.uniform4f[Program.program].find(location) == Program.uniform4f[Program.program].end() ||
		Program.uniform4f[Program.program][location][0] != v0 ||
		Program.uniform4f[Program.program][location][1] != v1 ||
		Program.uniform4f[Program.program][location][2] != v2 ||
		Program.uniform4f[Program.program][location][3] != v3) {
		Program.uniform4f[Program.program][location][0] = v0;
		Program.uniform4f[Program.program][location][1] = v1;
		Program.uniform4f[Program.program][location][2] = v2;
		Program.uniform4f[Program.program][location][3] = v3;
		glUniform4f(location, v0, v1, v2, v3);
	}
}

void GLUniform1iv(GLint location, GLsizei count, const GLint* value)
{
	eastl::vector<int> values(value, value + count);
	if (Program.uniform1iv[Program.program].find(location) == Program.uniform1iv[Program.program].end() ||
		Program.uniform1iv[Program.program][location] != values) {
		Program.uniform1iv[Program.program][location] = values;
		glUniform1iv(location, count, value);
	}
}

void GLUniform2iv(GLint location, GLsizei count, const GLint* value)
{
	eastl::vector<int> values(value, value + count);
	if (Program.uniform2iv[Program.program].find(location) == Program.uniform2iv[Program.program].end() ||
		Program.uniform2iv[Program.program][location] != values) {
		Program.uniform2iv[Program.program][location] = values;
		glUniform2iv(location, count, value);
	}
}

void GLUniform3iv(GLint location, GLsizei count, const GLint* value)
{
	eastl::vector<int> values(value, value + count);
	if (Program.uniform3iv[Program.program].find(location) == Program.uniform3iv[Program.program].end() ||
		Program.uniform3iv[Program.program][location] != values) {
		Program.uniform3iv[Program.program][location] = values;
		glUniform3iv(location, count, value);
	}
}

void GLUniform4iv(GLint location, GLsizei count, const GLint* value)
{
	eastl::vector<int> values(value, value + count);
	if (Program.uniform4iv[Program.program].find(location) == Program.uniform4iv[Program.program].end() ||
		Program.uniform4iv[Program.program][location] != values) {
		Program.uniform4iv[Program.program][location] = values;
		glUniform4iv(location, count, value);
	}
}

void GLUniform1fv(GLint location, GLsizei count, const GLfloat* value)
{
	eastl::vector<float> values(value, value + count);
	if (Program.uniform1fv[Program.program].find(location) == Program.uniform1fv[Program.program].end() ||
		Program.uniform1fv[Program.program][location] != values) {
		Program.uniform1fv[Program.program][location] = values;
		glUniform1fv(location, count, value);
	}
}

void GLUniform2fv(GLint location, GLsizei count, const GLfloat* value)
{
	eastl::vector<float> values(value, value + count);
	if (Program.uniform2fv[Program.program].find(location) == Program.uniform2fv[Program.program].end() ||
		Program.uniform2fv[Program.program][location] != values) {
		Program.uniform2fv[Program.program][location] = values;
		glUniform2fv(location, count, value);
	}
}

void GLUniform3fv(GLint location, GLsizei count, const GLfloat* value)
{
	eastl::vector<float> values(value, value + count);
	if (Program.uniform3fv[Program.program].find(location) == Program.uniform3fv[Program.program].end() ||
		Program.uniform3fv[Program.program][location] != values) {
		Program.uniform3fv[Program.program][location] = values;
		glUniform3fv(location, count, value);
	}
}

void GLUniform4fv(GLint location, GLsizei count, const GLfloat* value)
{
	eastl::vector<float> values(value, value + count);
	if (Program.uniform4fv[Program.program].find(location) == Program.uniform4fv[Program.program].end() ||
		Program.uniform4fv[Program.program][location] != values) {
		Program.uniform4fv[Program.program][location] = values;
		glUniform4fv(location, count, value);
	}
}

void GLUniformMatrix2fv(GLint location, GLsizei count, const GLfloat* value)
{
	eastl::vector<float> values(value, value + 4 * count);
	if (Program.uniformMatrix2fv[Program.program].find(location) == Program.uniformMatrix2fv[Program.program].end() ||
		Program.uniformMatrix2fv[Program.program][location] != values) {
		Program.uniformMatrix2fv[Program.program][location] = values;
		glUniformMatrix2fv(location, count, GL_FALSE, value);
	}
}

void GLUniformMatrix3fv(GLint location, GLsizei count, const GLfloat* value)
{
	eastl::vector<float> values(value, value + 9 * count);
	if (Program.uniformMatrix3fv[Program.program].find(location) == Program.uniformMatrix3fv[Program.program].end() ||
		Program.uniformMatrix3fv[Program.program][location] != values) {
		Program.uniformMatrix3fv[Program.program][location] = values;
		glUniformMatrix3fv(location, count, GL_FALSE, value);
	}
}

void GLUniformMatrix4fv(GLint location, GLsizei count, const GLfloat* value)
{
	eastl::vector<float> values(value, value + 16 * count);
	if (Program.uniformMatrix4fv[Program.program].find(location) == Program.uniformMatrix4fv[Program.program].end() ||
		Program.uniformMatrix4fv[Program.program][location] != values) {
		Program.uniformMatrix4fv[Program.program][location] = values;
		glUniformMatrix4fv(location, count, GL_FALSE, value);
	}
}
#pragma endregion
