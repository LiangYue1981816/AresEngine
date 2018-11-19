#include "GfxHeader.h"


CGLES3Mesh::CGLES3Mesh(CGLES3MeshManager *pManager, uint32_t name)
	: CGfxMesh(name)
	, m_pManager(pManager)

	, m_pIndexBuffer(nullptr)
	, m_pVertexBuffer(nullptr)
	, m_pInstanceBuffer(nullptr)
	, m_pDrawIndirectBuffer(nullptr)
	, m_pVertexArrayObject(nullptr)
{

}

CGLES3Mesh::CGLES3Mesh(CGLES3MeshManager *pManager, uint32_t name, uint32_t instanceFormat)
	: CGfxMesh(name)
	, m_pManager(pManager)

	, m_pIndexBuffer(nullptr)
	, m_pVertexBuffer(nullptr)
	, m_pInstanceBuffer(nullptr)
	, m_pDrawIndirectBuffer(nullptr)
	, m_pVertexArrayObject(nullptr)
{
	CreateInstanceBuffer(1, instanceFormat);
}

CGLES3Mesh::~CGLES3Mesh(void)
{
	Destroy();
}

void CGLES3Mesh::Release(void)
{
	m_pManager->DestroyMesh(this);
}

bool CGLES3Mesh::Load(const char *szFileName, uint32_t instanceFormat)
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

	Destroy();

	CStream stream;

	if (FileManager()->LoadStream(szFileName, &stream) == false) {
		return false;
	}

	MeshHeader header;
	stream.Read(&header, sizeof(header), 1);

	unsigned int vertexFormat = 0;
	stream.Read(&vertexFormat, sizeof(vertexFormat), 1);

	stream.Read(&m_aabb.minVertex.x, sizeof(m_aabb.minVertex.x), 1);
	stream.Read(&m_aabb.minVertex.y, sizeof(m_aabb.minVertex.y), 1);
	stream.Read(&m_aabb.minVertex.z, sizeof(m_aabb.minVertex.z), 1);
	stream.Read(&m_aabb.maxVertex.x, sizeof(m_aabb.maxVertex.x), 1);
	stream.Read(&m_aabb.maxVertex.y, sizeof(m_aabb.maxVertex.y), 1);
	stream.Read(&m_aabb.maxVertex.z, sizeof(m_aabb.maxVertex.z), 1);
	m_aabb.normalize();

	stream.Seek(header.indexBufferOffset, SEEK_SET);
	void *pIndexBuffer = stream.GetCurrentAddress();

	stream.Seek(header.vertexBufferOffset, SEEK_SET);
	void *pVertexBuffer = stream.GetCurrentAddress();

	CreateIndexBuffer(GL_UNSIGNED_INT, header.indexBufferSize, false, pIndexBuffer);
	CreateVertexBuffer(0, vertexFormat, header.vertexBufferSize, false, pVertexBuffer);
	CreateInstanceBuffer(1, instanceFormat);
	CreateVertexArrayObject();

	return true;
}

bool CGLES3Mesh::CreateIndexBuffer(uint32_t type, size_t size, bool bDynamic, const void *pBuffer)
{
	if (m_pIndexBuffer == nullptr) {
		m_pIndexBuffer = new CGLES3IndexBuffer(type, size, bDynamic);
		m_pIndexBuffer->BufferData(0, size, pBuffer);
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3Mesh::CreateVertexBuffer(uint32_t binding, uint32_t format, size_t size, bool bDynamic, const void *pBuffer)
{
	if (m_pVertexBuffer == nullptr) {
		m_pVertexBuffer = new CGLES3VertexBuffer(binding, format, size, bDynamic);
		m_pVertexBuffer->BufferData(0, size, pBuffer);
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3Mesh::CreateInstanceBuffer(uint32_t binding, uint32_t format)
{
	if (m_pInstanceBuffer == nullptr) {
		m_pInstanceBuffer = new CGLES3InstanceBuffer(binding, format);
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3Mesh::CreateDrawIndirectBuffer(uint32_t count)
{
	if (m_pDrawIndirectBuffer == nullptr) {
		m_pDrawIndirectBuffer = new CGLES3DrawIndirectBuffer(count);
		return true;
	}
	else {
		return false;
	}
}

bool CGLES3Mesh::CreateVertexArrayObject(void)
{
	if (m_pVertexArrayObject == nullptr) {
		m_pVertexArrayObject = new CGLES3VertexArrayObject;
		m_pVertexArrayObject->Buffer(m_pIndexBuffer, m_pVertexBuffer, m_pInstanceBuffer);
		return true;
	}
	else {
		return false;
	}
}

void CGLES3Mesh::Destroy(void)
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

	if (m_pDrawIndirectBuffer) {
		delete m_pDrawIndirectBuffer;
	}

	if (m_pVertexArrayObject) {
		delete m_pVertexArrayObject;
	}

	m_aabb.zero();
	m_pIndexBuffer = nullptr;
	m_pVertexBuffer = nullptr;
	m_pInstanceBuffer = nullptr;
	m_pDrawIndirectBuffer = nullptr;
	m_pVertexArrayObject = nullptr;
}

bool CGLES3Mesh::InstanceBufferData(size_t size, const void *pBuffer)
{
	return m_pInstanceBuffer ? m_pInstanceBuffer->BufferData(size, pBuffer) : false;
}

bool CGLES3Mesh::DrawIndirectBufferData(int indexDraw, int instanceCount)
{
	return m_pDrawIndirectBuffer ? m_pDrawIndirectBuffer->BufferData(indexDraw, instanceCount) : false;
}

bool CGLES3Mesh::DrawIndirectBufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount)
{
	return m_pDrawIndirectBuffer ? m_pDrawIndirectBuffer->BufferData(indexDraw, baseVertex, firstIndex, indexCount, instanceCount) : false;
}

const glm::aabb& CGLES3Mesh::GetLocalAABB(void) const
{
	return m_aabb;
}

uint32_t CGLES3Mesh::GetIndexType(void) const
{
	return m_pIndexBuffer ? m_pIndexBuffer->GetIndexType() : 0;
}

uint32_t CGLES3Mesh::GetIndexCount(void) const
{
	return m_pIndexBuffer ? m_pIndexBuffer->GetIndexCount() : 0;
}

uint32_t CGLES3Mesh::GetVertexFormat(void) const
{
	return m_pVertexBuffer ? m_pVertexBuffer->GetVertexFormat() : 0;
}

uint32_t CGLES3Mesh::GetVertexCount(void) const
{
	return m_pVertexBuffer ? m_pVertexBuffer->GetVertexCount() : 0;
}

uint32_t CGLES3Mesh::GetInstanceFormat(void) const
{
	return m_pInstanceBuffer ? m_pInstanceBuffer->GetInstanceFormat() : 0;
}

uint32_t CGLES3Mesh::GetInstanceCount(void) const
{
	return m_pInstanceBuffer ? m_pInstanceBuffer->GetInstanceCount() : 0;
}

uint32_t CGLES3Mesh::GetDrawCommandCount(void) const
{
	return m_pDrawIndirectBuffer ? m_pDrawIndirectBuffer->GetDrawCommandCount() : 0;
}

void CGLES3Mesh::Bind(void *pParam)
{
	if (m_pVertexArrayObject) {
		m_pVertexArrayObject->Bind(pParam);
	}

	if (m_pDrawIndirectBuffer) {
		m_pDrawIndirectBuffer->Bind(pParam);
	}
}
