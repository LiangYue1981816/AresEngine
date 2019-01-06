#pragma once
#include "VKRenderer.h"


class CVKShader : public CGfxShader
{
	friend class CVKShaderManager;


private:
	CVKShader(CVKDevice *pDevice, uint32_t name);
	virtual ~CVKShader(void);


public:
	bool Create(const uint32_t *words, size_t numWords, shader_kind kind);
	void Destroy(void);

public:
	bool IsValid(void) const;

public:
	uint32_t GetKind(void) const;
	VkShaderModule GetShader(void) const;
	const spirv_cross::CompilerGLSL* GetShaderCompiler(void) const;


private:
	uint32_t m_kind;
	VkShaderModule m_vkShader;

private:
	spirv_cross::CompilerGLSL *m_pShaderCompiler;

private:
	CVKDevice *m_pDevice;
};
