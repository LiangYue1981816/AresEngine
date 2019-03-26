#pragma once
#include "GLES3Renderer.h"


class CGLES3Material : public CGfxMaterial
{
	friend class CGLES3Renderer;
	friend class CGLES3MaterialManager;


private:
	CGLES3Material(CGLES3MaterialManager *pManager, uint32_t name);
	virtual ~CGLES3Material(void);
	virtual void Release(void);

	
public:
	CGfxMaterialPass* GetPass(uint32_t name);
	CGfxMaterialPass* CreatePass(uint32_t name);
	void Destroy(void);


private:
	eastl::unordered_map<uint32_t, CGLES3MaterialPass*> m_pPasses;

private:
	CGLES3MaterialManager *m_pManager;
};
