#pragma once
#include "GfxRenderer.h"


class CGfxShaderManager
{
	friend class CGfxRenderer;


private:
	CGfxShaderManager(void);
	virtual ~CGfxShaderManager(void);


private:
	CGfxShader* CreateShader(uint32_t name);


private:
	eastl::unordered_map<uint32_t, CGfxShader*> m_pShaders;
};
