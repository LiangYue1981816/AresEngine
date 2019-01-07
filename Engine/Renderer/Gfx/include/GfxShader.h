#pragma once
#include "GfxRenderer.h"


typedef enum shader_kind {
	// Forced shader kinds. These shader kinds force the compiler to compile the
	// source code as the specified kind of shader.
	vertex_shader,
	fragment_shader,
	compute_shader,
	geometry_shader,
	tess_control_shader,
	tess_evaluation_shader,

	glsl_vertex_shader = vertex_shader,
	glsl_fragment_shader = fragment_shader,
	glsl_compute_shader = compute_shader,
	glsl_geometry_shader = geometry_shader,
	glsl_tess_control_shader = tess_control_shader,
	glsl_tess_evaluation_shader = tess_evaluation_shader,
	// Deduce the shader kind from #pragma annotation in the source code. Compiler
	// will emit error if #pragma annotation is not found.
	glsl_infer_from_source,
	// Default shader kinds. Compiler will fall back to compile the source code as
	// the specified kind of shader when #pragma annotation is not found in the
	// source code.
	glsl_default_vertex_shader,
	glsl_default_fragment_shader,
	glsl_default_compute_shader,
	glsl_default_geometry_shader,
	glsl_default_tess_control_shader,
	glsl_default_tess_evaluation_shader,
	spirv_assembly,
} shader_kind;


class CALL_API CGfxShader
{
public:
	CGfxShader(uint32_t name)
	{

	}
	virtual ~CGfxShader(void)
	{

	}


public:
	virtual uint32_t GetName(void) const = 0;

public:
	virtual uint32_t GetKind(void) const = 0;

public:
	virtual bool Create(const uint32_t *words, size_t numWords, shader_kind kind) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual bool IsValid(void) const = 0;
};
