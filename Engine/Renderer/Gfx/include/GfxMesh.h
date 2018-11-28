#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxMesh : public CGfxResource
{
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
	virtual bool CreateIndexBuffer(uint32_t type, size_t size, bool bDynamic, const void *pBuffer) = 0;
	virtual bool CreateVertexBuffer(uint32_t binding, uint32_t format, size_t size, bool bDynamic, const void *pBuffer) = 0;
	virtual bool CreateVertexArrayObject(uint32_t drawCount, uint32_t binding, uint32_t format) = 0;
	virtual bool CreateDrawIndirectBuffer(uint32_t drawCount) = 0;
	virtual void Destroy(void) = 0;

public:
	virtual bool InstanceBufferData(int indexDraw, size_t size, const void *pBuffer) = 0;
	virtual bool DrawIndirectBufferData(int indexDraw, int instanceCount) = 0;
	virtual bool DrawIndirectBufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount) = 0;

public:
	virtual bool SetLocalAABB(int indexDraw, glm::aabb aabb) = 0;
	virtual const glm::aabb GetLocalAABB(int indexDraw) const = 0;

public:
	virtual uint32_t GetIndexType(void) const = 0;
	virtual uint32_t GetIndexCount(int indexDraw) const = 0;
	virtual uint32_t GetIndexOffset(int indexDraw) const = 0;

	virtual uint32_t GetVertexFormat(void) const = 0;
	virtual uint32_t GetVertexCount(int indexDraw) const = 0;

	virtual uint32_t GetInstanceFormat(void) const = 0;
	virtual uint32_t GetInstanceCount(int indexDraw) const = 0;

	virtual uint32_t GetDrawCommandCount(void) const = 0;
	virtual uint32_t GetDrawCommandOffset(int indexDraw) const = 0;

public:
	virtual void Bind(int indexDraw, void *pParam) = 0;


private:
	uint32_t m_name;
};
