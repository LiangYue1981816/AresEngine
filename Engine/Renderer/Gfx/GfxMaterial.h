#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxMaterial : public CGfxResource
{
public:
	CGfxMaterial(uint32_t name)
		: m_name(name)
	{

	}
	virtual ~CGfxMaterial(void)
	{

	}

public:
	uint32_t GetName(void) const
	{
		return m_name;
	}

	
public:
	virtual bool Load(const char *szFileName) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual CGfxMaterialPass* GetPass(uint32_t name) const = 0;


private:
	uint32_t m_name;
};
