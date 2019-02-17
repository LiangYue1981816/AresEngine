#pragma once
#include "VKRenderer.h"


class CVKShader : public CGfxShader
{
	friend class CVKShaderManager;


private:
	CVKShader(CVKDevice *pDevice, uint32_t name);
	virtual ~CVKShader(void);


public:
	uint32_t GetName(void) const;

public:
	HANDLE GetShader(void) const;

public:
	uint32_t GetKind(void) const;
	const CGfxSprivCross& GetSprivCross(void) const;

public:
	bool Create(const uint32_t *words, size_t numWords, shader_kind kind);
	void Destroy(void);

public:
	bool IsValid(void) const;


private:
	uint32_t m_name;

private:
	uint32_t m_kind;
	VkShaderModule m_vkShader;

private:
	CGfxSprivCross m_spriv;

private:
	CVKDevice *m_pDevice;
};
