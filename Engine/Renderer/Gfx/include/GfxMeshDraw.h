#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxMeshDraw : public CGfxResource
{
public:
	CGfxMeshDraw(const CGfxMeshPtr &ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding)
	{

	}
	virtual ~CGfxMeshDraw(void)
	{

	}


public:
	virtual bool InstanceBufferData(size_t size, const void *pBuffer) = 0;

public:
	virtual glm::aabb GetLocalAABB(void) const = 0;

	virtual GfxIndexType GetIndexType(void) const = 0;
	virtual uint32_t GetVertexFormat(void) const = 0;

	virtual uint32_t GetIndexCount(void) const = 0;
	virtual uint32_t GetIndexOffset(void) const = 0;

	virtual uint32_t GetInstanceCount(void) const = 0;
	virtual uint32_t GetInstanceFormat(void) const = 0;

public:
	virtual void Bind(void *pParam) = 0;
};
