#pragma once
#include "GLES3Renderer.h"


class CALL_API CGLES3DescriptorLayoutManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3DescriptorLayout;


private:
	CGLES3DescriptorLayoutManager(void);
	virtual ~CGLES3DescriptorLayoutManager(void);


private:
	CGLES3DescriptorLayout* Create(uint32_t set);
	void Destroy(CGLES3DescriptorLayout *pDescriptorLayout);


private:
	eastl::unordered_map<CGLES3DescriptorLayout*, CGLES3DescriptorLayout*> m_pDescriptorLayouts;
};
