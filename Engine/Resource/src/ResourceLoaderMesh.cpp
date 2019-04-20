#include "ResourceLoader.h"

#include "Stream.h"
#include "FileManager.h"


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
	float minx = FLT_MAX;
	float miny = FLT_MAX;
	float minz = FLT_MAX;
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

static bool InternalLoadDraws(CGfxMesh* pMesh, DrawHeader* drawHeaders, int numDraws)
{
	for (int indexDraw = 0; indexDraw < numDraws; indexDraw++) {
		const glm::aabb aabb(
			glm::vec3(drawHeaders[indexDraw].minx, drawHeaders[indexDraw].miny, drawHeaders[indexDraw].minz),
			glm::vec3(drawHeaders[indexDraw].maxx, drawHeaders[indexDraw].maxy, drawHeaders[indexDraw].maxz));

		if (pMesh->CreateDraw(indexDraw, aabb, drawHeaders[indexDraw].baseVertex, drawHeaders[indexDraw].firstIndex, drawHeaders[indexDraw].indexCount) == false) {
			return false;
		}
	}

	return true;
}


bool CResourceLoader::LoadMesh(const char* szFileName, CGfxMesh* pMesh, uint32_t vertexBinding)
{
	int err = 0;

	pMesh->Destroy();
	LogOutput(LOG_TAG_RENDERER, "LoadMesh(%s) ... ", szFileName);
	{
		CStream stream;
		if (FileManager()->LoadStream(szFileName, &stream) == false) { err = -1; goto ERR; }

		MeshHeader meshHeader;
		stream.Read(&meshHeader, sizeof(meshHeader), 1);

		DrawHeader* drawHeaders = (DrawHeader*)stream.GetCurrentAddress();

		stream.Seek(meshHeader.indexBufferOffset, SEEK_SET);
		void* pIndexBuffer = stream.GetCurrentAddress();

		stream.Seek(meshHeader.vertexBufferOffset, SEEK_SET);
		void* pVertexBuffer = stream.GetCurrentAddress();

		if (pMesh->CreateIndexBuffer(GFX_INDEX_UNSIGNED_INT, meshHeader.indexBufferSize, false, pIndexBuffer) == false) { err = -2; goto ERR; }
		if (pMesh->CreateVertexBuffer(meshHeader.format, vertexBinding, meshHeader.vertexBufferSize, false, pVertexBuffer) == false) { err = -3; goto ERR; }
		if (InternalLoadDraws(pMesh, drawHeaders, meshHeader.numDraws) == false) { err = -4; goto ERR; }
	}
	LogOutput(nullptr, "OK\n");
	return true;
ERR:
	LogOutput(nullptr, "Fail(%d)\n", err);
	pMesh->Destroy();
	return false;
}
