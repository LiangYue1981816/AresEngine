#pragma once
#include "GfxRenderer.h"


typedef struct GLstate {
	GLboolean bEnableCullFace;
	GLboolean bEnableDepthTest;
	GLboolean bEnableDepthWrite;
	GLboolean bEnableColorWrite[4];
	GLboolean bEnableBlend;
	GLboolean bEnablePolygonOffset;
	GLenum cullFace;
	GLenum frontFace;
	GLenum depthFunc;
	GLenum srcBlendFactor;
	GLenum dstBlendFactor;
	GLfloat polygonOffsetFactor;
	GLfloat polygonOffsetUnits;
} GLstate;


extern GLenum glGetShaderKind(shaderc_shader_kind kind);
