#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxMeshDraw : public CGfxResource
{
public:
	CGfxMeshDraw(uint32_t name, const CGfxMeshPtr ptrMesh, uint32_t nameDraw, uint32_t instanceFormat, int instanceBinding)
		: m_name(name)
	{

	}
	virtual ~CGfxMeshDraw(void)
	{

	}

	uint32_t GetName(void) const
	{
		return m_name;
	}


public:
	virtual glm::aabb GetLocalAABB(void) const = 0;

	virtual GfxIndexType GetIndexType(void) const = 0;
	virtual uint32_t GetIndexCount(void) const = 0;
	virtual uint32_t GetIndexOffset(void) const = 0;

	virtual uint32_t GetVertexFormat(void) const = 0;
	virtual uint32_t GetVertexCount(void) const = 0;

	virtual uint32_t GetInstanceFormat(void) const = 0;
	virtual uint32_t GetInstanceCount(void) const = 0;

public:
	virtual bool InstanceBufferData(size_t size, const void* pBuffer) = 0;


private:
	uint32_t m_name;
};
