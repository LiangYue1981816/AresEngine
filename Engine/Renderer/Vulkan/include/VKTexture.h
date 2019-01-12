#pragma once
#include "VKRenderer.h"


class CVKTexture
{
	friend class CVKDevice;
	friend class CVKTextureManager;


protected:
	typedef struct TransferBuffer {
		VkBufferImageCopy region;
		eastl::vector<uint8_t> buffer;
	} TransferBuffer;


protected:
	CVKTexture(CVKDevice *pDevice, CVKTextureManager *pManager);
	virtual ~CVKTexture(void);


public:
	VkImageView GetImageView(void) const;

protected:
	bool CreateView(VkImageView vkImageView);
	bool CreateView(VkImageViewType viewType, VkImageAspectFlags aspectMask, VkFormat format, int levels, int layers);
	bool CreateImage(VkImageType imageType, VkImageViewType viewType, VkFormat format, int width, int height, int levels, int layers, VkSampleCountFlagBits samples, VkImageTiling imageTiling, VkImageUsageFlags imageUsageFlags);
	void Destroy(void);

public:
	void Bind(VkCommandBuffer vkCommandBuffer, CVKBufferPtr &ptrBufferTransfer);


protected:
	bool m_bExtern;
	CVKImagePtr m_ptrImage;
	VkImageView m_vkImageView;

protected:
	uint32_t m_transferLevels;
	uint32_t m_transferLayers;
	eastl::unordered_map<int, TransferBuffer> m_transferBuffers;

protected:
	CVKDevice *m_pDevice;

protected:
	CVKTextureManager *m_pManager;
};
