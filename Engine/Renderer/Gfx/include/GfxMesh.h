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
	{

	}
	virtual ~CGfxMesh(void)
	{

	}


public:
	virtual uint32_t GetName(void) const = 0;

public:
	virtual CGfxIndexBuffer* GetIndexBuffer(void) = 0;
	virtual CGfxVertexBuffer* GetVertexBuffer(void) = 0;
	virtual Draw* GetDraw(int indexDraw) = 0;

	virtual GfxIndexType GetIndexType(void) const = 0;
	virtual uint32_t GetVertexFormat(void) const = 0;

public:
	virtual bool CreateIndexBuffer(GfxIndexType type, size_t size, bool bDynamic, const void *pBuffer) = 0;
	virtual bool CreateVertexBuffer(uint32_t vertexFormat, uint32_t vertexBinding, size_t size, bool bDynamic, const void *pBuffer) = 0;
	virtual bool CreateDraw(int indexDraw, const glm::aabb &aabb, int baseVertex, int firstIndex, int indexCount) = 0;
	virtual void Destroy(void) = 0;
};
