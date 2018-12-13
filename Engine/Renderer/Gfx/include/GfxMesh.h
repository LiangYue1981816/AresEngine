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
	virtual bool CreateIndexBuffer(GfxIndexType type, size_t size, bool bDynamic, const void *pBuffer) = 0;
	virtual bool CreateVertexBuffer(uint32_t vertexBinding, uint32_t vertexFormat, size_t size, bool bDynamic, const void *pBuffer) = 0;
	virtual bool CreateDraw(int indexDraw, const glm::aabb &aabb, int baseVertex, int firstIndex, int indexCount) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual const CGfxIndexBuffer* GetIndexBuffer(void) const = 0;
	virtual const CGfxVertexBuffer* GetVertexBuffer(void) const = 0;
	virtual const Draw* GetDraw(int indexDraw) const = 0;


private:
	uint32_t m_name;
};
