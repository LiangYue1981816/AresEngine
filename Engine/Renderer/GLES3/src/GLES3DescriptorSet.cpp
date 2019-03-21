#include "GLES3Renderer.h"


CGLES3DescriptorSet::CGLES3DescriptorSet(CGLES3DescriptorSetManager *pManager, const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
	: CGfxDescriptorSet(ptrDescriptorLayout)
	, m_pManager(pManager)
	, m_ptrDescriptorLayout(ptrDescriptorLayout)
{

}

CGLES3DescriptorSet::~CGLES3DescriptorSet(void)
{

}

void CGLES3DescriptorSet::Release(void)
{
	m_pManager->Destroy(this);
}


HANDLE CGLES3DescriptorSet::GetDescriptorSet(void) const
{
	return nullptr;
}

CGfxDescriptorLayoutPtr CGLES3DescriptorSet::GetDescriptorLayout(void) const
{
	return m_ptrDescriptorLayout;
}

bool CGLES3DescriptorSet::SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture, const CGfxSampler *pSampler)
{
	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].pSampler = (CGfxSampler *)pSampler;
		m_imageDescriptorInfos[name].ptrTexture2D = ptrTexture;
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3DescriptorSet::SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture, const CGfxSampler *pSampler)
{
	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].pSampler = (CGfxSampler *)pSampler;
		m_imageDescriptorInfos[name].ptrTexture2DArray = ptrTexture;
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3DescriptorSet::SetTextureCubeMap(uint32_t name, const CGfxTextureCubeMapPtr ptrTexture, const CGfxSampler *pSampler)
{
	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].pSampler = (CGfxSampler *)pSampler;
		m_imageDescriptorInfos[name].ptrTextureCubeMap = ptrTexture;
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3DescriptorSet::SetRenderTexture(uint32_t name, const CGfxRenderTexturePtr ptrTexture, const CGfxSampler *pSampler)
{
	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].pSampler = (CGfxSampler *)pSampler;
		m_imageDescriptorInfos[name].ptrRenderTexture = ptrTexture;
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3DescriptorSet::SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrUniformBuffer, uint32_t offset, uint32_t range)
{
	if (m_ptrDescriptorLayout->IsUniformBlockValid(name)) {
		m_bufferDescriptorInfos[name].offset = offset;
		m_bufferDescriptorInfos[name].range = range;
		m_bufferDescriptorInfos[name].ptrUniformBuffer = ptrUniformBuffer;
		return true;
	}
	else {
		return false;
	}
}

const CGfxDescriptorSet::DescriptorImageInfo* CGLES3DescriptorSet::GetDescriptorImageInfo(uint32_t name) const
{
	const auto &itDescriptorInfo = m_imageDescriptorInfos.find(name);

	if (itDescriptorInfo != m_imageDescriptorInfos.end()) {
		return &itDescriptorInfo->second;
	}
	else {
		return nullptr;
	}
}

const CGfxDescriptorSet::DescriptorBufferInfo* CGLES3DescriptorSet::GetDescriptorBufferInfo(uint32_t name) const
{
	const auto &itDescriptorInfo = m_bufferDescriptorInfos.find(name);

	if (itDescriptorInfo != m_bufferDescriptorInfos.end()) {
		return &itDescriptorInfo->second;
	}
	else {
		return nullptr;
	}
}
