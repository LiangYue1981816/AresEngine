#pragma once
#include "GLES3Renderer.h"


class CGLES3ShaderManager
{
	friend class CGLES3Renderer;


private:
	CGLES3ShaderManager(void);
	virtual ~CGLES3ShaderManager(void);


private:
	CGLES3Shader* LoadShader(const char *szFileName, shaderc_shader_kind kind);


private:
	eastl::unordered_map<uint32_t, CGLES3Shader*> m_pShaders;
};
