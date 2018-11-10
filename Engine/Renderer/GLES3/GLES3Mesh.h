#pragma once
#include "GLES3Renderer.h"


class CGLES3Mesh : public CGfxMesh
{
	friend class CGLES3MeshManager;


private:
	CGLES3Mesh(CGLES3MeshManager *pManager, uint32_t name);
	CGLES3Mesh(CGLES3MeshManager *pManager, uint32_t name, uint32_t instanceFormat);
	virtual ~CGLES3Mesh(void);
	virtual void Release(void);


public:
	bool Load(const char *szFileName, uint32_t instanceFormat);
	bool CreateIndexBuffer(uint32_t type, size_t size, bool bDynamic, const void *pBuffer);
	bool CreateVertexBuffer(uint32_t binding, uint32_t format, size_t size, bool bDynamic, const void *pBuffer);
	bool CreateInstanceBuffer(uint32_t binding, uint32_t format);
	bool CreateDrawIndirectBuffer(size_t size);
	bool CreateVertexArrayObject(void);
	void Destroy(void);

public:
	bool InstanceBufferData(size_t size, const void *pBuffer);
	bool DrawIndirectBufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount);

public:
	uint32_t GetIndexType(void) const;
	uint32_t GetIndexCount(void) const;

	uint32_t GetVertexFormat(void) const;
	uint32_t GetVertexCount(void) const;

	uint32_t GetInstanceFormat(void) const;
	uint32_t GetInstanceCount(void) const;

	const glm::aabb& GetLocalAABB(void) const;

public:
	void Bind(void *pParam);


private:
	glm::aabb m_aabb;

private:
	CGLES3IndexBuffer *m_pIndexBuffer;
	CGLES3VertexBuffer *m_pVertexBuffer;
	CGLES3InstanceBuffer *m_pInstanceBuffer;
	CGLES3DrawIndirectBuffer *m_pDrawIndirectBuffer;
	CGLES3VertexArrayObject *m_pVertexArrayObject;

private:
	CGLES3MeshManager *m_pManager;
};
