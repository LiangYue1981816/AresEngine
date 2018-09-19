#pragma once
#include "GfxRenderer.h"


class CGfxInstanceBuffer
{
	friend class CGfxMesh;
	friend class CGfxVertexArrayObject;


private:
	CGfxInstanceBuffer(GLuint format);
	virtual ~CGfxInstanceBuffer(void);


private:
	void AddInstance(const glm::mat4 &mtxTransform);
	void SetInstance(const eastl::vector<glm::mat4> &mtxTransforms);
	void ClearInstance(void);
	void UpdateInstance(void);

private:
	GLuint GetInstanceCount(void) const;
	GLuint GetInstanceFormat(void) const;
	GLuint GetInstanceBuffer(void) const;
	GLsizeiptr GetSize(void) const;

private:
	void Bind(void) const;


private:
	GLuint m_instanceFormat;
	GLuint m_instanceBuffer;
	GLsizeiptr m_size;

private:
	bool m_bDirty;
	eastl::vector<glm::mat4> m_instanceDatas;
};
