#pragma once
#include "VKRenderer.h"


class CVKShader : public CGfxShader
{
public:
	CVKShader(CVKDevice* pDevice, uint32_t name, const uint32_t* words, size_t numWords, shader_kind kind);
	virtual ~CVKShader(void);
	virtual void Release(void);


public:
	VkShaderModule GetShader(void) const;

public:
	bool IsValid(void) const;

public:
	shader_kind GetKind(void) const;
	const CGfxSprivCross& GetSprivCross(void) const;


private:
	shader_kind m_kind;
	CGfxSprivCross m_spriv;

private:
	VkShaderModule m_vkShader;

private:
	CVKDevice* m_pDevice;
};
