#pragma once
#include <tinyxml.h>
#include <tinystr.h>
#include "GfxRenderer.h"


class CGfxMaterial : public CGfxResource
{
	friend class CGfxRenderer;
	friend class CGfxMaterialManager;


private:
	CGfxMaterial(uint32_t name);
	virtual ~CGfxMaterial(void);
	virtual void Release(void);

public:
	uint32_t GetName(void) const;

	
private:
	bool Load(const char *szFileName);
	bool LoadPass(TiXmlNode *pPassNode);
	void Destroy(void);

public:
	const CGfxMaterialPass* GetPass(uint32_t name) const;


private:
	uint32_t m_name;

private:
	eastl::unordered_map<GLuint, CGfxMaterialPass*> m_pPasses;
};
