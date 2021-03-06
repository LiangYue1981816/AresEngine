#include "GLES3Renderer.h"


CGLES3DescriptorSet::CGLES3DescriptorSet(CGLES3DescriptorSetManager* pManager, uint32_t name, const CGfxDescriptorLayoutPtr ptrDescriptorLayout)
	: CGfxDescriptorSet(name, ptrDescriptorLayout)
	, m_pManager(pManager)
	, m_ptrDescriptorLayout(ptrDescriptorLayout)
{

}

CGLES3DescriptorSet::CGLES3DescriptorSet(CGLES3DescriptorSetManager* pManager, uint32_t name, const CGfxDescriptorSetPtr ptrDescriptorSetCopyFrom)
	: CGfxDescriptorSet(name, ptrDescriptorSetCopyFrom->GetDescriptorLayout())
	, m_pManager(pManager)
	, m_ptrDescriptorLayout(ptrDescriptorSetCopyFrom->GetDescriptorLayout())
{
	for (const auto& itImageDescriptorInfo : ((CGLES3DescriptorSet *)ptrDescriptorSetCopyFrom.GetPointer())->m_imageDescriptorInfos) {
		if (itImageDescriptorInfo.second.ptrTexture2D) {
			SetTexture2D(itImageDescriptorInfo.first, itImageDescriptorInfo.second.ptrTexture2D, itImageDescriptorInfo.second.pSampler);
		}

		if (itImageDescriptorInfo.second.ptrTexture2DArray) {
			SetTexture2DArray(itImageDescriptorInfo.first, itImageDescriptorInfo.second.ptrTexture2DArray, itImageDescriptorInfo.second.pSampler);
		}

		if (itImageDescriptorInfo.second.ptrTextureCubemap) {
			SetTextureCubemap(itImageDescriptorInfo.first, itImageDescriptorInfo.second.ptrTextureCubemap, itImageDescriptorInfo.second.pSampler);
		}

		if (itImageDescriptorInfo.second.ptrRenderTexture) {
			SetRenderTexture(itImageDescriptorInfo.first, itImageDescriptorInfo.second.ptrRenderTexture, itImageDescriptorInfo.second.pSampler);
		}

		if (itImageDescriptorInfo.second.ptrInputAttachmentTexture) {
			SetInputAttachmentTexture(itImageDescriptorInfo.first, itImageDescriptorInfo.second.ptrInputAttachmentTexture, itImageDescriptorInfo.second.pSampler);
		}
	}

	for (const auto& itBufferDescriptorInfo : ((CGLES3DescriptorSet *)ptrDescriptorSetCopyFrom.GetPointer())->m_bufferDescriptorInfos) {
		if (itBufferDescriptorInfo.second.ptrUniformBuffer) {
			SetUniformBuffer(itBufferDescriptorInfo.first, itBufferDescriptorInfo.second.ptrUniformBuffer, itBufferDescriptorInfo.second.offset, itBufferDescriptorInfo.second.range);
		}

		if (itBufferDescriptorInfo.second.ptrStorageBuffer) {
			SetStorageBuffer(itBufferDescriptorInfo.first, itBufferDescriptorInfo.second.ptrStorageBuffer, itBufferDescriptorInfo.second.offset, itBufferDescriptorInfo.second.range);
		}
	}
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
		m_imageDescriptorInfos[name].SetTexture2D(m_ptrDescriptorLayout->GetSampledImageBinding(name), (CGfxSampler*)pSampler, ptrTexture);
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
		m_imageDescriptorInfos[name].SetTexture2DArray(m_ptrDescriptorLayout->GetSampledImageBinding(name), (CGfxSampler*)pSampler, ptrTexture);
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
		m_imageDescriptorInfos[name].SetTextureCubemap(m_ptrDescriptorLayout->GetSampledImageBinding(name), (CGfxSampler*)pSampler, ptrTexture);
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3DescriptorSet::SetRenderTexture(uint32_t name, const CGfxRenderTexturePtr ptrTexture, const CGfxSampler* pSampler)
{
	ASSERT(pSampler);
	ASSERT(ptrTexture);
	ASSERT(m_ptrDescriptorLayout);

	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].SetRenderTexture(m_ptrDescriptorLayout->GetSampledImageBinding(name), (CGfxSampler*)pSampler, ptrTexture);
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3DescriptorSet::SetInputAttachmentTexture(uint32_t name, const CGfxRenderTexturePtr ptrTexture, const CGfxSampler* pSampler)
{
	ASSERT(pSampler);
	ASSERT(ptrTexture);
	ASSERT(m_ptrDescriptorLayout);

	if (m_ptrDescriptorLayout->IsSampledImageValid(name)) {
		m_imageDescriptorInfos[name].SetInputAttachmentTexture(m_ptrDescriptorLayout->GetInputAttachmentBinding(name), (CGfxSampler*)pSampler, ptrTexture);
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3DescriptorSet::SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrUniformBuffer, uint32_t offset, uint32_t range)
{
	ASSERT(ptrUniformBuffer);
	ASSERT(ptrUniformBuffer->GetSize() >= offset + range);
	ASSERT(m_ptrDescriptorLayout);

	if (m_ptrDescriptorLayout->IsUniformBlockValid(name)) {
		m_bufferDescriptorInfos[name].SetUniformBuffer(m_ptrDescriptorLayout->GetUniformBlockBinding(name), offset, range, ptrUniformBuffer);
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3DescriptorSet::SetStorageBuffer(uint32_t name, const CGfxStorageBufferPtr ptrStorageBuffer, uint32_t offset, uint32_t range)
{
	ASSERT(ptrStorageBuffer);
	ASSERT(ptrStorageBuffer->GetSize() >= offset + range);
	ASSERT(m_ptrDescriptorLayout);

	if (m_ptrDescriptorLayout->IsStorageBlockValid(name)) {
		m_bufferDescriptorInfos[name].SetStorageBuffer(m_ptrDescriptorLayout->GetStorageBlockBinding(name), offset, range, ptrStorageBuffer);
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
