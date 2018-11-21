#include "GfxHeader.h"


CGLES3Mesh::CGLES3Mesh(CGLES3MeshManager *pManager, uint32_t name)
	: CGfxMesh(name)
	, m_pManager(pManager)

	, m_instanceFormat(0)
	, m_pIndexBuffer(nullptr)
	, m_pVertexBuffer(nullptr)
	, m_pDrawIndirectBuffer(nullptr)
{

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

	typedef struct DrawHeader
	{
		float minx =  FLT_MAX;
		float miny =  FLT_MAX;
		float minz =  FLT_MAX;
		float maxx = -FLT_MAX;
		float maxy = -FLT_MAX;
		float maxz = -FLT_MAX;

		unsigned int baseVertex = 0;
		unsigned int firstIndex = 0;
		unsigned int indexCount = 0;

	} DrawHeader;

	typedef struct MeshHeader
	{
		unsigned int format = 0;
		unsigned int numDraws = 0;

		unsigned int indexBufferSize = 0;
		unsigned int indexBufferOffset = 0;

		unsigned int vertexBufferSize = 0;
		unsigned int vertexBufferOffset = 0;

	} MeshHeader;

	Destroy();

	CStream stream;
	if (FileManager()->LoadStream(szFileName, &stream) == false) {
		return false;
	}

	MeshHeader meshHeader;
	stream.Read(&meshHeader, sizeof(meshHeader), 1);

	DrawHeader *drawHeaders = (DrawHeader *)stream.GetCurrentAddress();

	stream.Seek(meshHeader.indexBufferOffset, SEEK_SET);
	void *pIndexBuffer = stream.GetCurrentAddress();

	stream.Seek(meshHeader.vertexBufferOffset, SEEK_SET);
	void *pVertexBuffer = stream.GetCurrentAddress();

	CreateIndexBuffer(GL_UNSIGNED_INT, meshHeader.indexBufferSize, false, pIndexBuffer);
	CreateVertexBuffer(0, meshHeader.format, meshHeader.vertexBufferSize, false, pVertexBuffer);
	CreateVertexArrayObject(meshHeader.numDraws, 1, instanceFormat);
//	CreateDrawIndirectBuffer(meshHeader.numDraws);

	for (int indexDraw = 0; indexDraw < (int)meshHeader.numDraws; indexDraw++) {
		DrawIndirectBufferData(indexDraw, drawHeaders[indexDraw].baseVertex, drawHeaders[indexDraw].firstIndex, drawHeaders[indexDraw].indexCount, 0);
		SetLocalAABB(indexDraw, glm::aabb(glm::vec3(drawHeaders[indexDraw].minx, drawHeaders[indexDraw].miny, drawHeaders[indexDraw].minz), glm::vec3(drawHeaders[indexDraw].maxx, drawHeaders[indexDraw].maxy, drawHeaders[indexDraw].maxz)));
	}

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

bool CGLES3Mesh::CreateVertexArrayObject(uint32_t drawCount, uint32_t binding, uint32_t format)
{
	if (m_draws.empty() && m_pIndexBuffer && m_pVertexBuffer) {
		m_draws.resize(drawCount);
		m_instanceFormat = format;

		for (int indexDraw = 0; indexDraw < (int)drawCount; indexDraw++) {
			m_draws[indexDraw].pInstanceBuffer = new CGLES3InstanceBuffer(binding, format);
			m_draws[indexDraw].pVertexArrayObject = new CGLES3VertexArrayObject;
			m_draws[indexDraw].pVertexArrayObject->Buffer(m_pIndexBuffer, m_pVertexBuffer, m_draws[indexDraw].pInstanceBuffer);
		}

		return true;
	}
	else {
		return false;
	}
}

bool CGLES3Mesh::CreateDrawIndirectBuffer(uint32_t drawCount)
{
	if (m_pDrawIndirectBuffer == nullptr) {
		m_pDrawIndirectBuffer = new CGLES3DrawIndirectBuffer(drawCount);
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

	if (m_pDrawIndirectBuffer) {
		delete m_pDrawIndirectBuffer;
	}

	for (int indexDraw = 0; indexDraw < (int)m_draws.size(); indexDraw++) {
		delete m_draws[indexDraw].pInstanceBuffer;
		delete m_draws[indexDraw].pVertexArrayObject;
	}

	m_draws.clear();
	m_instanceFormat = 0;
	m_pIndexBuffer = nullptr;
	m_pVertexBuffer = nullptr;
	m_pDrawIndirectBuffer = nullptr;
}

bool CGLES3Mesh::InstanceBufferData(int indexDraw, size_t size, const void *pBuffer)
{
	if (indexDraw >= 0 && indexDraw < (int)m_draws.size()) {
		return m_draws[indexDraw].pInstanceBuffer->BufferData(size, pBuffer);
	}
	else {
		return false;
	}
}

bool CGLES3Mesh::DrawIndirectBufferData(int indexDraw, int instanceCount)
{
	if (indexDraw >= 0 && indexDraw < (int)m_draws.size() && m_pDrawIndirectBuffer) {
		return m_pDrawIndirectBuffer->BufferData(indexDraw, instanceCount);
	}
	else {
		return false;
	}
}

bool CGLES3Mesh::DrawIndirectBufferData(int indexDraw, int baseVertex, int firstIndex, int indexCount, int instanceCount)
{
	if (indexDraw >= 0 && indexDraw < (int)m_draws.size()) {
		m_draws[indexDraw].baseVertex = baseVertex;
		m_draws[indexDraw].firstIndex = firstIndex;
		m_draws[indexDraw].indexCount = indexCount;

		if (m_pDrawIndirectBuffer) {
			m_pDrawIndirectBuffer->BufferData(indexDraw, baseVertex, firstIndex, indexCount, instanceCount);
		}

		return true;
	}
	else {
		return false;
	}
}

bool CGLES3Mesh::SetLocalAABB(int indexDraw, glm::aabb aabb)
{
	if (indexDraw >= 0 && indexDraw < (int)m_draws.size()) {
		m_draws[indexDraw].aabb = aabb;
		return true;
	}
	else {
		return false;
	}
}

const glm::aabb CGLES3Mesh::GetLocalAABB(int indexDraw) const
{
	if (indexDraw >= 0 && indexDraw < (int)m_draws.size()) {
		return m_draws[indexDraw].aabb;
	}
	else {
		return glm::aabb();
	}
}

uint32_t CGLES3Mesh::GetIndexType(void) const
{
	if (m_pIndexBuffer) {
		return m_pIndexBuffer->GetIndexType();
	}
	else {
		return 0;
	}
}

uint32_t CGLES3Mesh::GetIndexCount(int indexDraw) const
{
	if (indexDraw >= 0 && indexDraw < (int)m_draws.size() && m_pIndexBuffer) {
		return m_draws[indexDraw].indexCount;
	}
	else {
		return 0;
	}
}

uint32_t CGLES3Mesh::GetIndexOffset(int indexDraw) const
{
	if (indexDraw >= 0 && indexDraw < (int)m_draws.size() && m_pIndexBuffer) {
		switch (m_pIndexBuffer->GetIndexType()) {
		case GL_UNSIGNED_BYTE:  return m_draws[indexDraw].firstIndex;
		case GL_UNSIGNED_SHORT: return m_draws[indexDraw].firstIndex * 2;
		case GL_UNSIGNED_INT:   return m_draws[indexDraw].firstIndex * 4;
		default:                return 0;
		}
	}
	else {
		return 0;
	}
}

uint32_t CGLES3Mesh::GetVertexFormat(void) const
{
	if (m_pVertexBuffer) {
		return m_pVertexBuffer->GetVertexFormat();
	}
	else {
		return 0;
	}
}

uint32_t CGLES3Mesh::GetVertexCount(int indexDraw) const
{
	if (m_pVertexBuffer) {
		return m_pVertexBuffer->GetVertexCount();
	}
	else {
		return 0;
	}
}

uint32_t CGLES3Mesh::GetInstanceFormat(void) const
{
	return m_instanceFormat;
}

uint32_t CGLES3Mesh::GetInstanceCount(int indexDraw) const
{
	if (indexDraw >= 0 && indexDraw < (int)m_draws.size()) {
		return m_draws[indexDraw].pInstanceBuffer->GetInstanceCount();
	}
	else {
		return 0;
	}
}

uint32_t CGLES3Mesh::GetDrawCommandCount(void) const
{
	if (m_pDrawIndirectBuffer) {
		return m_pDrawIndirectBuffer->GetDrawCommandCount();
	}
	else {
		return 0;
	}
}

uint32_t CGLES3Mesh::GetDrawCommandOffset(int indexDraw) const
{
	if (m_pDrawIndirectBuffer) {
		return m_pDrawIndirectBuffer->GetDrawCommandOffset(indexDraw);
	}
	else {
		return 0;
	}
}

void CGLES3Mesh::Bind(void *pParam)
{
	int indexDraw = (int)pParam;

	if (indexDraw >= 0 && indexDraw < (int)m_draws.size()) {
		m_draws[indexDraw].pVertexArrayObject->Bind(nullptr);
	}

	if (m_pDrawIndirectBuffer) {
		m_pDrawIndirectBuffer->Bind(nullptr);
	}
}
