#pragma once
#include "GLES3Renderer.h"


class CGLES3InstanceBuffer : public CGfxInstanceBuffer
{
	friend class CGLES3Mesh;
	friend class CGLES3MeshDraw;


private:
	CGLES3InstanceBuffer(uint32_t instanceFormat, uint32_t instanceBinding);
	virtual ~CGLES3InstanceBuffer(void);


public:
	bool BufferData(size_t size, const void *pBuffer);
	void Bind(void *pParam);


private:
	uint32_t m_hash;
	uint32_t m_buffer;
};
