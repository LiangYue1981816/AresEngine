#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxExtension.h"


void glInitState(GLstate *state)
{
	state->bEnableCullFace = GL_TRUE;
	state->bEnableDepthTest = GL_TRUE;
	state->bEnableDepthWrite = GL_TRUE;
	state->bEnableColorWrite[0] = GL_TRUE;
	state->bEnableColorWrite[1] = GL_TRUE;
	state->bEnableColorWrite[2] = GL_TRUE;
	state->bEnableColorWrite[3] = GL_TRUE;
	state->bEnableBlend = GL_FALSE;
	state->bEnablePolygonOffset = GL_FALSE;
	state->cullFace = GL_BACK;
	state->frontFace = GL_CCW;
	state->depthFunc = GL_LESS;
	state->srcBlendFactor = GL_SRC_ALPHA;
	state->dstBlendFactor = GL_ONE_MINUS_SRC_ALPHA;
	state->polygonOffsetFactor = 0.0f;
	state->polygonOffsetUnits = 0.0f;
}

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
