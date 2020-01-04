#pragma once
#include "GLES3Renderer.h"


class CGLES3VertexArrayObject
{
public:
	CGLES3VertexArrayObject(void);
	CGLES3VertexArrayObject(const CGLES3IndexBuffer* pIndexBuffer, const CGLES3VertexBuffer* pVertexBuffer, const CGLES3InstanceBuffer* pInstanceBuffer);
	virtual ~CGLES3VertexArrayObject(void);
	virtual void Release(void);


public:
	bool Create(const CGLES3IndexBuffer* pIndexBuffer, const CGLES3VertexBuffer* pVertexBuffer, const CGLES3InstanceBuffer* pInstanceBuffer);
	void Destroy(void);

public:
	void Bind(void) const;


private:
	uint32_t m_vao;
};


class CGLES3MultiVertexArrayObject
{	
public:
	CGLES3MultiVertexArrayObject(int count);
	virtual ~CGLES3MultiVertexArrayObject(void);
	virtual void Release(void);


public:
	void SetBufferIndex(int index);

public:
	bool Create(const CGLES3IndexBuffer* pIndexBuffer, const CGLES3VertexBuffer* pVertexBuffer, const CGLES3InstanceBuffer* pInstanceBuffer);
	void Destroy(void);

public:
	void Bind(void) const;


private:
	int m_index;
	eastl::vector<CGLES3VertexArrayObject*> m_pVertexArrayObjects;
};
