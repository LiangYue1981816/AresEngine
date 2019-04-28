#pragma once
#include "GLES3Renderer.h"


class CGLES3ShaderManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3Shader;


private:
	CGLES3ShaderManager(void);
	virtual ~CGLES3ShaderManager(void);


private:
	CGLES3Shader* Create(const char* szFileName, shader_kind kind);


private:
	eastl::unordered_map<uint32_t, CGLES3Shader*> m_pShaders;
};
