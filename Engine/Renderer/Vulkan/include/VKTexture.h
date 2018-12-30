#pragma once
#include "VKRenderer.h"


class CVKTexture
{
	friend class CVKDevice;
	friend class CVKTextureManager;


private:
	CVKTexture(CVKDevice *pDevice, CVKTextureManager *pManager);
	virtual ~CVKTexture(void);


protected:
	bool Create(VkImageType imageType, VkImageViewType viewType, VkFormat format, int width, int height, int levels, int layers, VkSampleCountFlagBits samples, VkImageUsageFlags usage);
	void Destroy(void);


private:
	VkImage m_vkImage;
	VkImageView m_vkImageView;
	CVKMemory *m_pMemory;

private:
	CVKTextureManager *m_pManager;

private:
	CVKDevice *m_pDevice;
};
