#include "GLES3Renderer.h"


CGLES3DescriptorSet::CGLES3DescriptorSet(CGLES3DescriptorSetManager* pManager, const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
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

bool CGLES3DescriptorSet::SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture, const CGfxSampler* pSampler)
{
	ASSERT(pSampler);
	ASSERT(ptrTexture);
	ASSERT(m_ptrDescriptorLayout);

	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].pSampler = (CGfxSampler*)pSampler;
		m_imageDescriptorInfos[name].ptrTexture2D = ptrTexture;
		m_imageDescriptorInfos[name].ptrTexture2DArray.Release();
		m_imageDescriptorInfos[name].ptrTextureCubemap.Release();
		m_imageDescriptorInfos[name].ptrTextureInputAttachment.Release();
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3DescriptorSet::SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture, const CGfxSampler* pSampler)
{
	ASSERT(pSampler);
	ASSERT(ptrTexture);
	ASSERT(m_ptrDescriptorLayout);

	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].pSampler = (CGfxSampler*)pSampler;
		m_imageDescriptorInfos[name].ptrTexture2D.Release();
		m_imageDescriptorInfos[name].ptrTexture2DArray = ptrTexture;
		m_imageDescriptorInfos[name].ptrTextureCubemap.Release();
		m_imageDescriptorInfos[name].ptrTextureInputAttachment.Release();
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3DescriptorSet::SetTextureCubemap(uint32_t name, const CGfxTextureCubemapPtr ptrTexture, const CGfxSampler* pSampler)
{
	ASSERT(pSampler);
	ASSERT(ptrTexture);
	ASSERT(m_ptrDescriptorLayout);

	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].pSampler = (CGfxSampler*)pSampler;
		m_imageDescriptorInfos[name].ptrTexture2D.Release();
		m_imageDescriptorInfos[name].ptrTexture2DArray.Release();
		m_imageDescriptorInfos[name].ptrTextureCubemap = ptrTexture;
		m_imageDescriptorInfos[name].ptrTextureInputAttachment.Release();
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3DescriptorSet::SetTextureInputAttachment(uint32_t name, const CGfxRenderTexturePtr ptrTexture, const CGfxSampler* pSampler)
{
	ASSERT(pSampler);
	ASSERT(ptrTexture);
	ASSERT(m_ptrDescriptorLayout);

	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].pSampler = (CGfxSampler*)pSampler;
		m_imageDescriptorInfos[name].ptrTexture2D.Release();
		m_imageDescriptorInfos[name].ptrTexture2DArray.Release();
		m_imageDescriptorInfos[name].ptrTextureCubemap.Release();
		m_imageDescriptorInfos[name].ptrTextureInputAttachment = ptrTexture;
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3DescriptorSet::SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrUniformBuffer, uint32_t offset, uint32_t range)
{
	ASSERT(ptrUniformBuffer);
	ASSERT(m_ptrDescriptorLayout);

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

const CGfxDescriptorLayoutPtr CGLES3DescriptorSet::GetDescriptorLayout(void) const
{
	ASSERT(m_ptrDescriptorLayout);
	return m_ptrDescriptorLayout;
}

const DescriptorImageInfo* CGLES3DescriptorSet::GetDescriptorImageInfo(uint32_t name) const
{
	const auto& itDescriptorInfo = m_imageDescriptorInfos.find(name);

	if (itDescriptorInfo != m_imageDescriptorInfos.end()) {
		return &itDescriptorInfo->second;
	}
	else {
		return nullptr;
	}
}

const DescriptorBufferInfo* CGLES3DescriptorSet::GetDescriptorBufferInfo(uint32_t name) const
{
	const auto& itDescriptorInfo = m_bufferDescriptorInfos.find(name);

	if (itDescriptorInfo != m_bufferDescriptorInfos.end()) {
		return &itDescriptorInfo->second;
	}
	else {
		return nullptr;
	}
}
