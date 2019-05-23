#include "VKRenderer.h"


CVKDescriptorLayoutManager::CVKDescriptorLayoutManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
{

}

CVKDescriptorLayoutManager::~CVKDescriptorLayoutManager(void)
{
	for (const auto& itDescriptorLayout : m_pDescriptorLayouts) {
		delete itDescriptorLayout.second;
	}
}

CVKDescriptorLayout* CVKDescriptorLayoutManager::Create(uint32_t set)
{
	mutex_autolock autolock(&lock);
	{
		CVKDescriptorLayout* pDescriptorLayout = new CVKDescriptorLayout(m_pDevice, this, set);
		m_pDescriptorLayouts[pDescriptorLayout] = pDescriptorLayout;
		return pDescriptorLayout;
	}
}

void CVKDescriptorLayoutManager::Destroy(CVKDescriptorLayout* pDescriptorLayout)
{
	mutex_autolock autolock(&lock);
	{
		ASSERT(pDescriptorLayout);

		if (m_pDescriptorLayouts.find(pDescriptorLayout) != m_pDescriptorLayouts.end()) {
			m_pDescriptorLayouts.erase(pDescriptorLayout);
			delete pDescriptorLayout;
		}
	}
}
