#pragma once
#include "GfxRenderer.h"


class CGfxMesh : public CGfxResource
{
	friend class CGfxRenderer;
	friend class CGfxMeshManager;


private:
	CGfxMesh(GLuint name);
	virtual ~CGfxMesh(void);
	virtual void Release(void);


public:
	bool Load(const char *szFileName);
	void Free(void);

public:
	bool CreateIndexBuffer(size_t size, const void *pBuffer, bool bDynamic, GLenum type);
	bool CreateVertexBuffer(size_t size, const void *pBuffer, bool bDynamic, GLuint format);
	bool CreateInstanceBuffer(GLuint format);
	bool CreateVertexArrayObject(void);

public:
	void AddInstance(const glm::mat4 &mtxTransform);
	void SetInstance(const eastl::vector<glm::mat4> &mtxTransforms);
	void ClearInstance(void);
	void UpdateInstance(void);

public:
	GLuint GetName(void) const;

	GLenum GetIndexType(void) const;
	GLuint GetIndexCount(void) const;

	GLuint GetVertexFormat(void) const;
	GLuint GetVertexCount(void) const;
	GLuint GetInstanceCount(void) const;

	const glm::aabb& GetLocalAABB(void) const;

public:
	void Bind(void) const;


private:
	GLuint m_name;

private:
	glm::aabb m_aabb;

private:
	CGfxIndexBuffer *m_pIndexBuffer;
	CGfxVertexBuffer *m_pVertexBuffer;
	CGfxInstanceBuffer *m_pInstanceBuffer;
	CGfxVertexArrayObject *m_pVertexArrayObject;
};
