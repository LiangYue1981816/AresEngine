#pragma once
#include "GfxRenderer.h"


class CGfxMesh : public CGfxResource
{
	friend class CGfxRenderer;
	friend class CGfxMeshManager;


private:
	CGfxMesh(uint32_t name);
	virtual ~CGfxMesh(void);
	virtual void Release(void);

public:
	uint32_t GetName(void) const;


public:
	bool Load(const char *szFileName, uint32_t instanceFormat);
	void Free(void);

public:
	bool CreateInstanceBuffer(uint32_t format);
	bool CreateIndexBuffer(size_t size, const void *pBuffer, bool bDynamic, uint32_t type);
	bool CreateVertexBuffer(size_t size, const void *pBuffer, bool bDynamic, uint32_t format);
	bool CreateVertexArrayObject(void);

public:
	void SetInstance(const uint8_t *pBuffer, uint32_t size);
	void UpdateInstance(void);

public:
	uint32_t GetIndexType(void) const;
	uint32_t GetIndexCount(void) const;

	uint32_t GetVertexFormat(void) const;
	uint32_t GetVertexCount(void) const;

	uint32_t GetInstanceCount(void) const;
	uint32_t GetInstanceCount(uint32_t size) const;

public:
	const glm::aabb& GetLocalAABB(void) const;

public:
	void Bind(void) const;


private:
	uint32_t m_name;

private:
	glm::aabb m_aabb;

private:
	CGfxInstanceBuffer *m_pInstanceBuffer;
	CGfxIndexBuffer *m_pIndexBuffer;
	CGfxVertexBuffer *m_pVertexBuffer;
	CGfxVertexArrayObject *m_pVertexArrayObject;
};
