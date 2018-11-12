#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxShader
{
public:
	CGfxShader(uint32_t name)
		: m_name(name)
	{

	}
	virtual ~CGfxShader(void)
	{

	}

public:
	uint32_t GetName(void) const
	{
		return m_name;
	}


public:
	virtual bool Load(const char *szFileName, shaderc_shader_kind kind) = 0;
	virtual bool Create(const uint32_t *words, size_t numWords, shaderc_shader_kind kind) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual bool IsValid(void) const = 0;

public:
	virtual uint32_t GetKind(void) const = 0;


private:
	uint32_t m_name;
};
