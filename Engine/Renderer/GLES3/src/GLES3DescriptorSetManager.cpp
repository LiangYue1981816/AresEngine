#include "GLES3Renderer.h"


CGLES3DescriptorSetManager::CGLES3DescriptorSetManager(void)
{

}

CGLES3DescriptorSetManager::~CGLES3DescriptorSetManager(void)
{
	for (const auto &itDescriptorSet : m_pDescriptorSets) {
		delete itDescriptorSet.second;
	}
}

CGLES3DescriptorSet* CGLES3DescriptorSetManager::Create(const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
{
	mutex_autolock autolock(&lock);
	{
		CGLES3DescriptorSet *pDescriptorSet = new CGLES3DescriptorSet(this, ptrDescriptorLayout);
		m_pDescriptorSets[pDescriptorSet] = pDescriptorSet;
		return pDescriptorSet;
	}
}

void CGLES3DescriptorSetManager::Destroy(CGLES3DescriptorSet *pDescriptorSet)
{
	mutex_autolock autolock(&lock);
	{
		if (pDescriptorSet) {
			m_pDescriptorSets.erase(pDescriptorSet);
			delete pDescriptorSet;
		}
	}
}
