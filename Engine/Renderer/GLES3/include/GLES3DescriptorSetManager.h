#pragma once
#include "GLES3Renderer.h"


class CALL_API CGLES3DescriptorSetManager : public CGfxResourceManager
{
	friend class CGLES3Renderer;
	friend class CGLES3DescriptorSet;


private:
	CGLES3DescriptorSetManager(void);
	virtual ~CGLES3DescriptorSetManager(void);


private:
	CGLES3DescriptorSet* Create(const CGfxDescriptorLayoutPtr ptrDescriptorLayout);
	void Destroy(CGLES3DescriptorSet *pDescriptorSet);


private:
	eastl::unordered_map<CGLES3DescriptorSet*, CGLES3DescriptorSet*> m_pDescriptorSets;
};
