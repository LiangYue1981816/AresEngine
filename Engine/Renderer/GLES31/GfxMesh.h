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


private:
	bool Load(const char *szFileName);
	void Free(void);

	void CreateIndexBuffer(size_t size, const void *pBuffer, bool bDynamic, GLenum type);
	void CreateVertexBuffer(size_t size, const void *pBuffer, bool bDynamic, GLuint format);
	void CreateInstanceBuffer(GLuint format);

public:
	void AddInstance(const glm::mat4 &mtxTransform);
	void SetInstance(const eastl::vector<glm::mat4> &mtxTransforms);
	void ClearInstance(void);

public:
	GLuint GetName(void) const;

	GLenum GetIndexType(void) const;
	GLuint GetIndexCount(void) const;

	GLuint GetVertexFormat(void) const;
	GLuint GetVertexCount(void) const;
	GLuint GetInstanceCount(void) const;

	const glm::aabb& GetLocalAABB(void) const;

private:
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
