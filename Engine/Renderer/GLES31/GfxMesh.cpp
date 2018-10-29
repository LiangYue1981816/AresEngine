#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxMesh.h"
#include "GfxInstanceBuffer.h"
#include "GfxIndexBuffer.h"
#include "GfxVertexBuffer.h"
#include "GfxVertexArrayObject.h"
#include "GfxVertexAttribute.h"


CGfxMesh::CGfxMesh(uint32_t name)
	: m_name(name)

	, m_pInstanceBuffer(nullptr)
	, m_pIndexBuffer(nullptr)
	, m_pVertexBuffer(nullptr)
	, m_pVertexArrayObject(nullptr)
{

}

CGfxMesh::~CGfxMesh(void)
{
	Free();
}

void CGfxMesh::Release(void)
{
	Renderer()->DestroyMesh(this);
}

uint32_t CGfxMesh::GetName(void) const
{
	return m_name;
}

bool CGfxMesh::Load(const char *szFileName, uint32_t instanceFormat)
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

	FILE *pFile = nullptr;
	void *pIndexBuffer = nullptr;
	void *pVertexBuffer = nullptr;

	try {
		Free();

		pFile = fopen(Renderer()->GetResourceFullName(szFileName), "rb");
		if (pFile == nullptr) throw 0;

		MeshHeader header;
		fread(&header, sizeof(header), 1, pFile);

		unsigned int vertexFormat = 0;
		fread(&vertexFormat, sizeof(vertexFormat), 1, pFile);

		fread(&m_aabb.minVertex.x, sizeof(m_aabb.minVertex.x), 1, pFile);
		fread(&m_aabb.minVertex.y, sizeof(m_aabb.minVertex.y), 1, pFile);
		fread(&m_aabb.minVertex.z, sizeof(m_aabb.minVertex.z), 1, pFile);
		fread(&m_aabb.maxVertex.x, sizeof(m_aabb.maxVertex.x), 1, pFile);
		fread(&m_aabb.maxVertex.y, sizeof(m_aabb.maxVertex.y), 1, pFile);
		fread(&m_aabb.maxVertex.z, sizeof(m_aabb.maxVertex.z), 1, pFile);
		m_aabb.normalize();

		pIndexBuffer = AllocMemory(header.indexBufferSize);
		pVertexBuffer = AllocMemory(header.vertexBufferSize);

		fseek(pFile, header.indexBufferOffset, SEEK_SET);
		fread(pIndexBuffer, header.indexBufferSize, 1, pFile);

		fseek(pFile, header.vertexBufferOffset, SEEK_SET);
		fread(pVertexBuffer, header.vertexBufferSize, 1, pFile);

		CreateIndexBuffer(header.indexBufferSize, pIndexBuffer, false, GL_UNSIGNED_INT);
		CreateVertexBuffer(header.vertexBufferSize, pVertexBuffer, false, vertexFormat);
		CreateInstanceBuffer(instanceFormat);
		CreateVertexArrayObject();

		FreeMemory(pVertexBuffer);
		FreeMemory(pIndexBuffer);
		fclose(pFile);

		return true;
	}
	catch (int) {
		Free();

		if (pVertexBuffer) FreeMemory(pVertexBuffer);
		if (pIndexBuffer) FreeMemory(pIndexBuffer);
		if (pFile) fclose(pFile);

		return false;
	}
}

void CGfxMesh::Free(void)
{
	if (m_pInstanceBuffer) {
		delete m_pInstanceBuffer;
	}

	if (m_pIndexBuffer) {
		delete m_pIndexBuffer;
	}

	if (m_pVertexBuffer) {
		delete m_pVertexBuffer;
	}

	if (m_pVertexArrayObject) {
		delete m_pVertexArrayObject;
	}

	m_aabb.zero();
	m_pInstanceBuffer = nullptr;
	m_pIndexBuffer = nullptr;
	m_pVertexBuffer = nullptr;
	m_pVertexArrayObject = nullptr;
}

bool CGfxMesh::CreateInstanceBuffer(uint32_t format)
{
	if (m_pInstanceBuffer) {
		delete m_pInstanceBuffer;
	}

	if (m_pVertexArrayObject) {
		delete m_pVertexArrayObject;
	}

	m_pInstanceBuffer = nullptr;
	m_pVertexArrayObject = nullptr;

	m_pInstanceBuffer = new CGfxInstanceBuffer(format);
	return true;
}

bool CGfxMesh::CreateIndexBuffer(size_t size, const void *pBuffer, bool bDynamic, uint32_t type)
{
	if (m_pIndexBuffer) {
		delete m_pIndexBuffer;
	}

	if (m_pVertexArrayObject) {
		delete m_pVertexArrayObject;
	}

	m_pIndexBuffer = nullptr;
	m_pVertexArrayObject = nullptr;

	m_pIndexBuffer = new CGfxIndexBuffer(type, size, bDynamic);
	return m_pIndexBuffer->BufferData(0, size, pBuffer);
}

bool CGfxMesh::CreateVertexBuffer(size_t size, const void *pBuffer, bool bDynamic, uint32_t format)
{
	if (m_pVertexBuffer) {
		delete m_pVertexBuffer;
	}

	if (m_pVertexArrayObject) {
		delete m_pVertexArrayObject;
	}

	m_pVertexBuffer = nullptr;
	m_pVertexArrayObject = nullptr;

	m_pVertexBuffer = new CGfxVertexBuffer(format, size, bDynamic);
	return m_pVertexBuffer->BufferData(0, size, pBuffer);
}

bool CGfxMesh::CreateVertexArrayObject(void)
{
	if (m_pVertexArrayObject) {
		delete m_pVertexArrayObject;
	}

	m_pVertexArrayObject = new CGfxVertexArrayObject;
	return m_pVertexArrayObject->Buffer(m_pIndexBuffer, m_pVertexBuffer, m_pInstanceBuffer);
}

void CGfxMesh::SetInstance(const uint8_t *pBuffer, uint32_t size)
{
	if (m_pInstanceBuffer) {
		m_pInstanceBuffer->SetInstance(pBuffer, size);
	}
}

void CGfxMesh::UpdateInstance(void)
{
	if (m_pInstanceBuffer) {
		m_pInstanceBuffer->UpdateInstance();
	}
}

uint32_t CGfxMesh::GetIndexType(void) const
{
	return m_pIndexBuffer ? m_pIndexBuffer->GetIndexType() : GL_INVALID_ENUM;
}

uint32_t CGfxMesh::GetIndexCount(void) const
{
	return m_pIndexBuffer ? m_pIndexBuffer->GetIndexCount() : 0;
}

uint32_t CGfxMesh::GetVertexFormat(void) const
{
	return m_pVertexBuffer ? m_pVertexBuffer->GetVertexFormat() : 0;
}

uint32_t CGfxMesh::GetVertexCount(void) const
{
	return m_pVertexBuffer ? m_pVertexBuffer->GetVertexCount() : 0;
}

uint32_t CGfxMesh::GetInstanceCount(void) const
{
	return m_pInstanceBuffer ? m_pInstanceBuffer->GetInstanceCount() : 0;
}

uint32_t CGfxMesh::GetInstanceCount(uint32_t size) const
{
	return m_pInstanceBuffer ? m_pInstanceBuffer->GetInstanceCount(size) : 0;
}

const glm::aabb& CGfxMesh::GetLocalAABB(void) const
{
	return m_aabb;
}

void CGfxMesh::Bind(void) const
{
	if (m_pVertexArrayObject) {
		m_pVertexArrayObject->Bind();
	}
	else {
		if (m_pIndexBuffer) {
			m_pIndexBuffer->Bind();
		}

		if (m_pVertexBuffer) {
			m_pVertexBuffer->Bind();
			m_pVertexBuffer->SetupFormat();
		}

		if (m_pInstanceBuffer) {
			m_pInstanceBuffer->Bind();
			m_pInstanceBuffer->SetupFormat();
		}
	}
}
