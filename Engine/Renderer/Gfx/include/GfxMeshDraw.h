#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxMeshDraw : public CGfxResource
{
public:
	CGfxMeshDraw(uint32_t name, const CGfxMeshPtr &ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding)
		: m_name(name)
	{

	}
	CGfxMeshDraw(const CGfxMeshPtr &ptrMesh, int indexDraw, uint32_t instanceFormat, uint32_t instanceBinding)
		: m_name(INVALID_HASHNAME)
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
	virtual bool InstanceBufferData(size_t size, const void *pBuffer) = 0;

public:
	virtual GfxIndexType GetIndexType(void) const = 0;
	virtual uint32_t GetVertexFormat(void) const = 0;

	virtual uint32_t GetIndexCount(void) const = 0;
	virtual uint32_t GetIndexOffset(void) const = 0;

	virtual uint32_t GetInstanceCount(void) const = 0;
	virtual uint32_t GetInstanceFormat(void) const = 0;

	virtual glm::aabb GetLocalAABB(void) const = 0;

public:
	virtual void Bind(void *pParam) = 0;


private:
	uint32_t m_name;
};
