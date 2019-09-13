#pragma once
#include "PreHeader.h"


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

	uint32_t GetName(void) const
	{
		return m_name;
	}


public:
	virtual CGfxMaterialPass* GetPass(uint32_t name) = 0;
	virtual CGfxMaterialPass* CreatePass(uint32_t name) = 0;
	virtual void Destroy(void) = 0;


private:
	uint32_t m_name;
};
