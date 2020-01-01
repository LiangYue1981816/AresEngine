#pragma once
#include "PreHeader.h"


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
	virtual void SetMask(uint32_t mask) = 0;
	virtual uint32_t GetMask(void) const = 0;

public:
	virtual CGfxMeshPtr GetMesh(void) const = 0;
	virtual glm::aabb GetAABB(void) const = 0;

public:
	virtual uint32_t GetIndexType(void) const = 0;
	virtual uint32_t GetIndexCount(void) const = 0;
	virtual uint32_t GetIndexFirst(void) const = 0;
	virtual uint32_t GetIndexOffset(void) const = 0;

	virtual uint32_t GetVertexBinding(void) const = 0;
	virtual uint32_t GetVertexFormat(void) const = 0;
	virtual uint32_t GetVertexCount(void) const = 0;

	virtual uint32_t GetInstanceBinding(void) const = 0;
	virtual uint32_t GetInstanceFormat(void) const = 0;
	virtual uint32_t GetInstanceCount(void) const = 0;

public:
	virtual bool InstanceBufferData(size_t size, const void* data) = 0;

public:
	typedef void (*RenderCallback)(CGfxCommandBufferPtr, void*);
	virtual void SetRenderCallback(RenderCallback callback, void* param) = 0;
	virtual void OnRenderCallback(CGfxCommandBufferPtr ptrCommandBuffer) const = 0;


private:
	uint32_t m_name;
};
