#pragma once
#include "GfxRenderer.h"


class CGfxInstanceBuffer
{
	friend class CGfxMesh;
	friend class CGfxVertexArrayObject;


private:
	CGfxInstanceBuffer(uint32_t format);
	virtual ~CGfxInstanceBuffer(void);


private:
	void SetInstance(const uint8_t *pBuffer, uint32_t size);
	void UpdateInstance(void);

private:
	uint32_t GetInstanceFormat(void) const;
	uint32_t GetInstanceBuffer(void) const;
	uint32_t GetInstanceBufferSize(void) const;
	uint32_t GetInstanceCount(void) const;
	uint32_t GetInstanceCount(uint32_t size) const;

private:
	void Bind(void) const;
	void SetupFormat(void) const;


private:
	uint32_t m_instanceFormat;
	uint32_t m_instanceBuffer;
	uint32_t m_size;

private:
	bool m_bDirty;
	uint32_t m_hash;
	eastl::vector<uint8_t> m_buffer;
};
