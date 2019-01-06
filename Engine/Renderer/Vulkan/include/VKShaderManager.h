#pragma once
#include "VKRenderer.h"


class CVKShaderManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKDevice;


private:
	CVKShaderManager(CVKDevice *pDevice);
	virtual ~CVKShaderManager(void);


private:
	CVKShader* Create(const char *szFileName, shader_kind kind);


private:
	eastl::unordered_map<uint32_t, CGfxShader*> m_pShaders;

private:
	CVKDevice *m_pDevice;
};
