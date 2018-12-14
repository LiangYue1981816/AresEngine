#pragma once
#include "GLES3Renderer.h"


class CGLES3MeshDraw : public CGfxMeshDraw
{
	friend class CGLES3MeshManager;


private:
	CGLES3MeshDraw(CGLES3MeshManager *pManager);
	virtual ~CGLES3MeshDraw(void);
	virtual void Release(void);


public:
	bool Create(const CGfxMeshPtr &ptrMesh, int indexDraw, uint32_t instanceBinding, uint32_t instanceFormat);
	void Destroy(void);

public:
	bool InstanceBufferData(size_t size, const void *pBuffer);

public:
	GfxIndexType GetIndexType(void) const;
	uint32_t GetIndexCount(void) const;
	uint32_t GetIndexOffset(void) const;

	uint32_t GetVertexFormat(void) const;
	uint32_t GetVertexCount(void) const;

	uint32_t GetInstanceFormat(void) const;
	uint32_t GetInstanceCount(void) const;

public:
	void Bind(void *pParam);


private:
	CGfxMeshPtr m_ptrMesh;
	CGfxMesh::Draw *m_pMeshDraw;
	CGLES3InstanceBuffer *m_pInstanceBuffer;
	CGLES3VertexArrayObject *m_pVertexArrayObject;
	CGLES3DrawIndirectBuffer *m_pDrawIndirectBuffer;

private:
	CGLES3MeshManager *m_pManager;
};
/*
class CGLES3Mesh : public CGfxMesh
{
	friend class CGLES3MeshManager;


private:
	typedef struct Draw {
		glm::aabb aabb;

		int baseVertex = 0;
		int firstIndex = 0;
		int indexCount = 0;

		CGLES3InstanceBuffer *pInstanceBuffer = nullptr;
		CGLES3VertexArrayObject *pVertexArrayObject = nullptr;
	} Draw;


private:
	CGLES3Mesh(CGLES3MeshManager *pManager, uint32_t name);
	virtual ~CGLES3Mesh(void);
	virtual void Release(void);


public:
	bool CreateIndexBuffer(GfxIndexType type, size_t size, bool bDynamic, const void *pBuffer);
	bool CreateVertexBuffer(uint32_t binding, uint32_t vertexFormat, size_t size, bool bDynamic, const void *pBuffer);
	bool CreateVertexArrayObject(uint32_t drawCount, uint32_t binding, uint32_t instanceFormat);
	bool CreateDrawIndirectBuffer(uint32_t drawCount);
	void Destroy(void);

public:
	bool InstanceBufferData(int indexDraw, size_t size, const void *pBuffer);
	bool DrawIndirectBufferData(int indexDraw, int instanceCount);
	bool DrawIndirectBufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount);

public:
	bool SetLocalAABB(int indexDraw, glm::aabb aabb);
	const glm::aabb GetLocalAABB(int indexDraw) const;

public:
	GfxIndexType GetIndexType(void) const;
	uint32_t GetIndexCount(int indexDraw) const;
	uint32_t GetIndexOffset(int indexDraw) const;

	uint32_t GetVertexFormat(void) const;
	uint32_t GetVertexCount(int indexDraw) const;

	uint32_t GetInstanceFormat(void) const;
	uint32_t GetInstanceCount(int indexDraw) const;

	uint32_t GetDrawCommandCount(void) const;
	uint32_t GetDrawCommandOffset(int indexDraw) const;

public:
	void Bind(int indexDraw, void *pParam);


private:
	uint32_t m_instanceFormat;
	eastl::vector<Draw> m_draws;

private:
	CGLES3IndexBuffer *m_pIndexBuffer;
	CGLES3VertexBuffer *m_pVertexBuffer;
	CGLES3DrawIndirectBuffer *m_pDrawIndirectBuffer;

private:
	CGLES3MeshManager *m_pManager;
};
*/