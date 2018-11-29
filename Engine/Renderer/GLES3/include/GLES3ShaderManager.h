#pragma once
#include "GLES3Renderer.h"


class CGLES3ShaderManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;


private:
	CGLES3ShaderManager(void);
	virtual ~CGLES3ShaderManager(void);


private:
	CGLES3Shader* Create(const char *szFileName, shader_kind kind);


private:
	eastl::unordered_map<uint32_t, CGfxShader*> m_pShaders;
};
