#include "GLES3Renderer.h"


CGLES3DescriptorLayoutManager::CGLES3DescriptorLayoutManager(void)
{

}

CGLES3DescriptorLayoutManager::~CGLES3DescriptorLayoutManager(void)
{
	for (const auto& itDescriptorLayout : m_pDescriptorLayouts) {
		delete itDescriptorLayout.second;
	}
}

CGLES3DescriptorLayout* CGLES3DescriptorLayoutManager::Create(uint32_t set)
{
	mutex_autolock autolock(&lock);
	{
		if (CGLES3DescriptorLayout* pDescriptorLayout = new CGLES3DescriptorLayout(this, set)) {
			m_pDescriptorLayouts[pDescriptorLayout] = pDescriptorLayout;
			return pDescriptorLayout;
		}
		else {
			return nullptr;
		}
	}
}

void CGLES3DescriptorLayoutManager::Destroy(CGLES3DescriptorLayout* pDescriptorLayout)
{
	ASSERT(pDescriptorLayout);
	{
		mutex_autolock autolock(&lock);
		{
			if (m_pDescriptorLayouts.find(pDescriptorLayout) != m_pDescriptorLayouts.end()) {
				m_pDescriptorLayouts.erase(pDescriptorLayout);
			}
		}
	}
	delete pDescriptorLayout;
}
