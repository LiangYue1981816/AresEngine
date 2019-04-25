#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxMesh : public CGfxResource
{
public:
	typedef struct Draw {
		glm::aabb aabb;
		int baseVertex = 0;
		int firstIndex = 0;
		int indexCount = 0;
	} Draw;


public:
	CGfxMesh(uint32_t name)
		: m_name(name)
	{

	}
	virtual ~CGfxMesh(void)
	{

	}

	uint32_t GetName(void) const
	{
		return m_name;
	}


public:
	virtual CGfxMesh::Draw* GetDraw(uint32_t name) = 0;
	virtual CGfxIndexBuffer* GetIndexBuffer(void) = 0;
	virtual CGfxVertexBuffer* GetVertexBuffer(void) = 0;

public:
	virtual bool CreateDraw(uint32_t name, const glm::aabb& aabb, int baseVertex, int firstIndex, int indexCount) = 0;
	virtual bool CreateIndexBuffer(GfxIndexType type, size_t size, bool bDynamic, const void* data) = 0;
	virtual bool CreateVertexBuffer(uint32_t vertexFormat, int vertexBinding, size_t size, bool bDynamic, const void* data) = 0;
	virtual void Destroy(void) = 0;


private:
	uint32_t m_name;
};
