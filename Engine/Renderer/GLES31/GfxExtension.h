#pragma once
#include "GfxRenderer.h"


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


extern void glInitState(GLstate *state);
extern void glReadBuffers(int n, const uint32_t *bufs);
extern uint32_t glGetShaderType(shaderc_shader_kind kind);
extern uint32_t glGetProgramStage(shaderc_shader_kind kind);
