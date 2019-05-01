#pragma once
#include "VKRenderer.h"


class CVKDescriptorLayoutManager : public CGfxResourceManager
{
	friend class CVKRenderer;
	friend class CVKDescriptorLayout;


private:
	CVKDescriptorLayoutManager(CVKDevice* pDevice);
	virtual ~CVKDescriptorLayoutManager(void);


private:
	CVKDescriptorLayout* Create(uint32_t set);
	void Destroy(CVKDescriptorLayout* pDescriptorLayout);


private:
	eastl::unordered_map<CVKDescriptorLayout*, CVKDescriptorLayout*> m_pDescriptorLayouts;

private:
	CVKDevice* m_pDevice;
};
