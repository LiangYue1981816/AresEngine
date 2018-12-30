#pragma once
#include "VKRenderer.h"


class CVKTexture : public CGfxResource
{
	friend class CVKDevice;


private:
	CVKTexture(CVKDevice *pDevice);
	virtual ~CVKTexture(void);
	virtual void Release(void);


private:
	bool CreateTexture2D(VkFormat format, int width, int height, int levels, VkSampleCountFlagBits samples);
	bool CreateTexture2DArray(VkFormat format, int width, int height, int levels, int layers, VkSampleCountFlagBits samples);
	bool CreateTextureCubeMap(VkFormat format, int width, int height, int levels);
	void Destroy(void);


private:
	VkImage m_vkImage;
	VkImageView m_vkImageView;
	CVKMemory *m_pMemory;

private:
	CVKDevice *m_pDevice;
};

typedef CGfxResourcePtr<CVKTexture> CVKImagePtr;
