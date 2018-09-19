#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxMesh.h"
#include "GfxIndexBuffer.h"
#include "GfxVertexBuffer.h"
#include "GfxInstanceBuffer.h"
#include "GfxVertexAttribute.h"
#include "GfxVertexArrayObject.h"


CGfxMesh::CGfxMesh(GLuint name)
	: m_name(name)

	, m_pIndexBuffer(NULL)
	, m_pVertexBuffer(NULL)
	, m_pInstanceBuffer(NULL)
	, m_pVertexArrayObject(NULL)
{
	m_aabb.zero();
}

CGfxMesh::~CGfxMesh(void)
{
	Free();
}

void CGfxMesh::Release(void)
{
	Renderer()->FreeMesh(this);
}

bool CGfxMesh::Load(const char *szFileName)
{
	enum RawVertexAttribute
	{
		RAW_VERTEX_ATTRIBUTE_POSITION = 1 << 0,
		RAW_VERTEX_ATTRIBUTE_NORMAL = 1 << 1,
		RAW_VERTEX_ATTRIBUTE_BINORMAL = 1 << 3,
		RAW_VERTEX_ATTRIBUTE_COLOR = 1 << 4,
		RAW_VERTEX_ATTRIBUTE_UV0 = 1 << 5,
		RAW_VERTEX_ATTRIBUTE_UV1 = 1 << 6,
	};

	typedef struct MeshHeader
	{
		unsigned int indexBufferSize;
		unsigned int indexBufferOffset;

		unsigned int vertexBufferSize;
		unsigned int vertexBufferOffset;

	} MeshHeader;

	FILE *pFile = NULL;
	void *pIndexBuffer = NULL;
	void *pVertexBuffer = NULL;

	try {
		Free();

		char szFullPath[260];
		Renderer()->GetMeshFullPath(szFileName, szFullPath);

		pFile = fopen(szFullPath, "rb");
		if (pFile == NULL) throw 0;

		MeshHeader header;
		fread(&header, sizeof(header), 1, pFile);

		unsigned int format = 0;
		fread(&format, sizeof(format), 1, pFile);

		fread(&m_aabb.minVertex.x, sizeof(m_aabb.minVertex.x), 1, pFile);
		fread(&m_aabb.minVertex.y, sizeof(m_aabb.minVertex.y), 1, pFile);
		fread(&m_aabb.minVertex.z, sizeof(m_aabb.minVertex.z), 1, pFile);
		fread(&m_aabb.maxVertex.x, sizeof(m_aabb.maxVertex.x), 1, pFile);
		fread(&m_aabb.maxVertex.y, sizeof(m_aabb.maxVertex.y), 1, pFile);
		fread(&m_aabb.maxVertex.z, sizeof(m_aabb.maxVertex.z), 1, pFile);
		m_aabb.normalize();

		pIndexBuffer = malloc(header.indexBufferSize);
		pVertexBuffer = malloc(header.vertexBufferSize);

		fseek(pFile, header.indexBufferOffset, SEEK_SET);
		fread(pIndexBuffer, header.indexBufferSize, 1, pFile);

		fseek(pFile, header.vertexBufferOffset, SEEK_SET);
		fread(pVertexBuffer, header.vertexBufferSize, 1, pFile);

		m_pIndexBuffer = new CGfxIndexBuffer(GL_UNSIGNED_INT);
		m_pVertexBuffer = new CGfxVertexBuffer(format);
		m_pInstanceBuffer = new CGfxInstanceBuffer(INSTANCE_ATTRIBUTE_TRANSFORM);
		m_pVertexArrayObject = new CGfxVertexArrayObject(m_pIndexBuffer, m_pVertexBuffer, m_pInstanceBuffer);

		m_pIndexBuffer->BufferData(header.indexBufferSize, pIndexBuffer, false);
		m_pVertexBuffer->BufferData(header.vertexBufferSize, pVertexBuffer, false);

		free(pVertexBuffer);
		free(pIndexBuffer);
		fclose(pFile);

		return true;
	}
	catch (int) {
		Free();

		if (pVertexBuffer) free(pVertexBuffer);
		if (pIndexBuffer) free(pIndexBuffer);
		if (pFile) fclose(pFile);

		return false;
	}
}

void CGfxMesh::Free(void)
{
	if (m_pIndexBuffer) {
		delete m_pIndexBuffer;
	}

	if (m_pVertexBuffer) {
		delete m_pVertexBuffer;
	}

	if (m_pInstanceBuffer) {
		delete m_pInstanceBuffer;
	}

	if (m_pVertexArrayObject) {
		delete m_pVertexArrayObject;
	}

	m_pIndexBuffer = NULL;
	m_pVertexBuffer = NULL;
	m_pInstanceBuffer = NULL;
	m_pVertexArrayObject = NULL;
}

void CGfxMesh::CreateIndexBuffer(size_t size, const void *pBuffer, bool bDynamic, GLenum type)
{
	if (m_pIndexBuffer) {
		delete m_pIndexBuffer;
	}

	if (m_pVertexArrayObject) {
		delete m_pVertexArrayObject;
	}

	m_pIndexBuffer = NULL;
	m_pVertexArrayObject = NULL;

	m_pIndexBuffer = new CGfxIndexBuffer(type);
	m_pIndexBuffer->BufferData(size, pBuffer, bDynamic);
}

void CGfxMesh::CreateVertexBuffer(size_t size, const void *pBuffer, bool bDynamic, GLuint format)
{
	if (m_pVertexBuffer) {
		delete m_pVertexBuffer;
	}

	if (m_pVertexArrayObject) {
		delete m_pVertexArrayObject;
	}

	m_pVertexBuffer = NULL;
	m_pVertexArrayObject = NULL;

	m_pVertexBuffer = new CGfxVertexBuffer(format);
	m_pVertexBuffer->BufferData(size, pBuffer, bDynamic);
}

void CGfxMesh::CreateInstanceBuffer(GLuint format)
{
	if (m_pInstanceBuffer) {
		delete m_pInstanceBuffer;
	}

	if (m_pVertexArrayObject) {
		delete m_pVertexArrayObject;
	}

	m_pInstanceBuffer = NULL;
	m_pVertexArrayObject = NULL;

	m_pInstanceBuffer = new CGfxInstanceBuffer(format);
}

void CGfxMesh::AddInstance(const glm::mat4 &mtxTransform)
{
	if (m_pInstanceBuffer) {
		m_pInstanceBuffer->AddInstance(mtxTransform);
	}
}

void CGfxMesh::SetInstance(const eastl::vector<glm::mat4> &mtxTransforms)
{
	if (m_pInstanceBuffer) {
		m_pInstanceBuffer->SetInstance(mtxTransforms);
	}
}

void CGfxMesh::ClearInstance(void)
{
	if (m_pInstanceBuffer) {
		m_pInstanceBuffer->ClearInstance();
	}
}

GLuint CGfxMesh::GetName(void) const
{
	return m_name;
}

GLenum CGfxMesh::GetIndexType(void) const
{
	return m_pIndexBuffer ? m_pIndexBuffer->GetIndexType() : GL_INVALID_ENUM;
}

GLuint CGfxMesh::GetIndexCount(void) const
{
	return m_pIndexBuffer ? m_pIndexBuffer->GetIndexCount() : 0;
}

GLuint CGfxMesh::GetVertexFormat(void) const
{
	return m_pVertexBuffer ? m_pVertexBuffer->GetVertexFormat() : 0;
}

GLuint CGfxMesh::GetVertexCount(void) const
{
	return m_pVertexBuffer ? m_pVertexBuffer->GetVertexCount() : 0;
}

GLuint CGfxMesh::GetInstanceCount(void) const
{
	return m_pInstanceBuffer ? m_pInstanceBuffer->GetInstanceCount() : 0;
}

const glm::aabb& CGfxMesh::GetLocalAABB(void) const
{
	return m_aabb;
}

void CGfxMesh::Bind(void) const
{
	if (m_pInstanceBuffer) {
		m_pInstanceBuffer->UpdateInstance();
	}

	if (m_pVertexArrayObject) {
		m_pVertexArrayObject->Bind();
	}
	else {
		if (m_pIndexBuffer) {
			m_pIndexBuffer->Bind();
		}

		if (m_pVertexBuffer) {
			m_pVertexBuffer->Bind();
		}

		if (m_pInstanceBuffer) {
			m_pInstanceBuffer->Bind();
		}
	}
}
