#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxExtension.h"


void glReadBuffers(int n, const uint32_t *bufs)
{
	for (int index = 0; index < n; index++) {
		glReadBuffer(bufs[index]);
	}
}

uint32_t glGetShaderKind(shaderc_shader_kind kind)
{
	switch (kind) {
	case shaderc_shader_kind::shaderc_vertex_shader:
		return GL_VERTEX_SHADER;
	case shaderc_shader_kind::shaderc_fragment_shader:
		return GL_FRAGMENT_SHADER;
	case shaderc_shader_kind::shaderc_compute_shader:
		return GL_COMPUTE_SHADER;
	default:
		return GL_INVALID_ENUM;
	}
}
