#include "VKRenderer.h"


CVKTexture::CVKTexture(CVKDevice *pDevice, CVKTextureManager *pManager)
	: m_pDevice(pDevice)
	, m_pManager(pManager)

	, m_bExtern(false)
	, m_vkImageView(VK_NULL_HANDLE)

	, m_transferLevels(1)
	, m_transferLayers(1)
{

}

CVKTexture::~CVKTexture(void)
{

}

bool CVKTexture::Create(VkImageView vkImageView)
{
	Destroy();

	m_bExtern = true;
	m_vkImageView = vkImageView;

	return true;
}

void CVKTexture::Destroy(void)
{
	if (m_bExtern == false) {
		if (m_vkImageView) {
			vkDestroyImageView(m_pDevice->GetDevice(), m_vkImageView, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
		}
	}

	m_bExtern = false;
	m_vkImageView = VK_NULL_HANDLE;
	m_ptrImage.Release();

	m_transferLevels = 1;
	m_transferLayers = 1;
	m_transferBuffers.clear();
}

void CVKTexture::Bind(VkCommandBuffer vkCommandBuffer, CVKBufferPtr &ptrBufferTransfer)
{
	if (m_transferBuffers.size()) {
		eastl::vector<uint8_t> buffers;
		eastl::vector<VkBufferImageCopy> regions;

		for (auto &itTransferBuffer : m_transferBuffers) {
			itTransferBuffer.second.region.bufferOffset = buffers.size();
			regions.emplace_back(itTransferBuffer.second.region);
			buffers.insert(buffers.end(), itTransferBuffer.second.buffer.data(), itTransferBuffer.second.buffer.size());
		}

		ptrBufferTransfer = CVKBufferPtr(new CVKBuffer(m_pDevice, buffers.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));
		ptrBufferTransfer->BufferData(0, buffers.size(), buffers.data());
		vkCmdTransferImage(vkCommandBuffer, ptrBufferTransfer->GetBuffer(), m_ptrImage->GetImage(), m_transferLevels, m_transferLayers, regions.data(), regions.size());

		m_transferLevels = 1;
		m_transferLayers = 1;
		m_transferBuffers.clear();
	}
}

VkImageView CVKTexture::GetImageView(void) const
{
	return m_vkImageView;
}
