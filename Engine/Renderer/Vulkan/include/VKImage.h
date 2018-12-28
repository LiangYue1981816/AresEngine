#pragma once
#include "VKRenderer.h"


class CVKImage : public CGfxResource
{
	friend class CVKDevice;


private:
	CVKImage(CVKDevice *pDevice);
	virtual ~CVKImage(void);
	virtual void Release(void);


private:
	bool CreateImage2D(VkFormat format, int width, int height, int levels, VkSampleCountFlagBits samples);
	bool CreateImage2DArray(VkFormat format, int width, int height, int levels, int layers, VkSampleCountFlagBits samples);
	bool CreateImageCubeMap(VkFormat format, int width, int height, int levels);
	void Destroy(void);


private:
	VkImage m_vkImage;
	VkImageView m_vkImageView;
	CVKMemory *m_pMemory;

private:
	CVKDevice *m_pDevice;
};

typedef CGfxResourcePtr<CVKImage> CVKImagePtr;
