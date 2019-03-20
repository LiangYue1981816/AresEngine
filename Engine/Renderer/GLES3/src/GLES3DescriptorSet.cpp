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

bool CGLES3DescriptorSet::SetUniformBuffer(uint32_t name, const CGfxUniformBufferPtr ptrUniformBuffer)
{
	if (m_ptrDescriptorLayout->IsUniformBlockValid(name)) {
		m_bufferDescriptors[name].ptrUniformBuffer = ptrUniformBuffer;
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3DescriptorSet::SetTexture2D(uint32_t name, const CGfxTexture2DPtr ptrTexture, const CGfxSampler *pSampler)
{
	if (m_ptrDescriptorLayout->IsTextureValid(name)) {
		m_imageDescriptors[name].pSampler = (CGfxSampler *)pSampler;
		m_imageDescriptors[name].ptrTexture2D = ptrTexture;
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3DescriptorSet::SetTexture2DArray(uint32_t name, const CGfxTexture2DArrayPtr ptrTexture, const CGfxSampler *pSampler)
{
	if (m_ptrDescriptorLayout->IsTextureValid(name)) {
		m_imageDescriptors[name].pSampler = (CGfxSampler *)pSampler;
		m_imageDescriptors[name].ptrTexture2DArray = ptrTexture;
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3DescriptorSet::SetTextureCubeMap(uint32_t name, const CGfxTextureCubeMapPtr ptrTexture, const CGfxSampler *pSampler)
{
	if (m_ptrDescriptorLayout->IsTextureValid(name)) {
		m_imageDescriptors[name].pSampler = (CGfxSampler *)pSampler;
		m_imageDescriptors[name].ptrTextureCubeMap = ptrTexture;
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3DescriptorSet::SetRenderTexture(uint32_t name, const CGfxRenderTexturePtr ptrRenderTexture, const CGfxSampler *pSampler)
{
	if (m_ptrDescriptorLayout->IsTextureValid(name)) {
		m_imageDescriptors[name].pSampler = (CGfxSampler *)pSampler;
		m_imageDescriptors[name].ptrRenderTexture = ptrRenderTexture;
		return true;
	}
	else {
		return false;
	}
}

CGfxSampler* CGLES3DescriptorSet::GetSampler(uint32_t name) const
{
	const auto &itDescriptor = m_imageDescriptors.find(name);

	if (itDescriptor != m_imageDescriptors.end()) {
		return itDescriptor->second.pSampler;
	}
	else {
		return false;
	}
}

CGfxTexture2DPtr CGLES3DescriptorSet::GetTexture2D(uint32_t name) const
{
	const auto &itDescriptor = m_imageDescriptors.find(name);

	if (itDescriptor != m_imageDescriptors.end()) {
		return itDescriptor->second.ptrTexture2D;
	}
	else {
		return false;
	}
}

CGfxTexture2DArrayPtr CGLES3DescriptorSet::GetTexture2DArray(uint32_t name) const
{
	const auto &itDescriptor = m_imageDescriptors.find(name);

	if (itDescriptor != m_imageDescriptors.end()) {
		return itDescriptor->second.ptrTexture2DArray;
	}
	else {
		return false;
	}
}

CGfxTextureCubeMapPtr CGLES3DescriptorSet::GetTextureCubeMap(uint32_t name) const
{
	const auto &itDescriptor = m_imageDescriptors.find(name);

	if (itDescriptor != m_imageDescriptors.end()) {
		return itDescriptor->second.ptrTextureCubeMap;
	}
	else {
		return false;
	}
}

CGfxRenderTexturePtr CGLES3DescriptorSet::GetRenderTexture(uint32_t name) const
{
	const auto &itDescriptor = m_imageDescriptors.find(name);

	if (itDescriptor != m_imageDescriptors.end()) {
		return itDescriptor->second.ptrRenderTexture;
	}
	else {
		return false;
	}
}
