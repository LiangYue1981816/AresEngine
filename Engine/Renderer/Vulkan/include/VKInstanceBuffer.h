#pragma once
#include "VKRenderer.h"


class CVKInstanceBuffer : public CGfxInstanceBuffer
{
	friend class CVKDevice;
	friend class CVKMeshDraw;


private:
	CVKInstanceBuffer(CVKDevice *pDevice, uint32_t instanceFormat, uint32_t instanceBinding);
	virtual ~CVKInstanceBuffer(void);


public:
	bool BufferData(size_t size, const void *pBuffer);
	void Bind(VkCommandBuffer vkCommandBuffer, VkDeviceSize offset);

private:
	CVKBufferPtr GetBuffer(void) const;


private:
	uint32_t m_hash;
	uint32_t m_binding;

private:
	CVKBufferPtr m_ptrBuffer;

private:
	CVKDevice *m_pDevice;
};
