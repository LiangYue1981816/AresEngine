#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxMeshDraw : public CGfxResource
{
public:
	CGfxMeshDraw(void)
	{

	}
	virtual ~CGfxMeshDraw(void)
	{

	}


public:
	virtual bool Create(const CGfxMeshPtr &ptrMesh, int indexDraw, uint32_t instanceBinding, uint32_t instanceFormat) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual bool InstanceBufferData(size_t size, const void *pBuffer) = 0;

public:
	virtual GfxIndexType GetIndexType(void) const = 0;
	virtual uint32_t GetIndexCount(void) const = 0;
	virtual uint32_t GetIndexOffset(void) const = 0;

	virtual uint32_t GetVertexFormat(void) const = 0;
	virtual uint32_t GetVertexCount(void) const = 0;

	virtual uint32_t GetInstanceFormat(void) const = 0;
	virtual uint32_t GetInstanceCount(void) const = 0;

public:
	virtual void Bind(void *pParam) = 0;
};
