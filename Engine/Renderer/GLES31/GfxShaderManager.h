#pragma once
#include "GfxRenderer.h"


class CGfxShaderManager
{
	friend class CGfxRenderer;


private:
	CGfxShaderManager(void);
	virtual ~CGfxShaderManager(void);


private:
	CGfxShader* LoadShader(const char *szFileName, shaderc_shader_kind kind);


private:
	eastl::unordered_map<uint32_t, CGfxShader*> m_pShaders;
};
