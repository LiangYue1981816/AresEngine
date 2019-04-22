#pragma once
#include "VKRenderer.h"


class CVKTexture
{
protected:
	typedef struct TransferBuffer {
		VkBufferImageCopy region;
		eastl::vector<uint8_t> buffer;
	} TransferBuffer;


protected:
	CVKTexture(CVKDevice* pDevice);
	virtual ~CVKTexture(void);


protected:
	bool CreateView(VkImageView vkImageView);
	bool CreateView(VkImageViewType viewType, VkImageAspectFlags aspectMask, VkFormat format, int levels, int layers);
	bool CreateImage(VkImageType imageType, VkImageViewType viewType, VkFormat format, int width, int height, int levels, int layers, VkSampleCountFlagBits samples, VkImageTiling imageTiling, VkImageUsageFlags imageUsageFlags);
	void Destroy(void);

protected:
	CVKBufferPtr BufferTransfer(VkCommandBuffer vkCommandBuffer);


protected:
	uint32_t m_transferLevels;
	uint32_t m_transferLayers;
	eastl::unordered_map<int, TransferBuffer> m_transferBuffers;

protected:
	bool m_bExtern;
	CVKImagePtr m_ptrImage;
	VkImageView m_vkImageView;

protected:
	CVKDevice* m_pDevice;
};
