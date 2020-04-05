#include "GfxHeader.h"
#include "SceneHeader.h"
#include "ResourceLoader.h"


typedef enum RawVertexAttribute {
	RAW_VERTEX_ATTRIBUTE_POSITION = 1 << 0,
	RAW_VERTEX_ATTRIBUTE_NORMAL = 1 << 1,
	RAW_VERTEX_ATTRIBUTE_BINORMAL = 1 << 3,
	RAW_VERTEX_ATTRIBUTE_COLOR = 1 << 4,
	RAW_VERTEX_ATTRIBUTE_UV0 = 1 << 5,
	RAW_VERTEX_ATTRIBUTE_UV1 = 1 << 6,
} RawVertexAttribute;

typedef struct DrawHeader {
	char szName[260];

	float minx;
	float miny;
	float minz;
	float maxx;
	float maxy;
	float maxz;

	unsigned int baseVertex;
	unsigned int firstIndex;
	unsigned int indexCount;
} DrawHeader;

typedef struct MeshHeader {
	unsigned int format;
	unsigned int numDraws;

	unsigned int indexBufferSize;
	unsigned int indexBufferOffset;

	unsigned int vertexBufferSize;
	unsigned int vertexBufferOffset;
} MeshHeader;

static bool InternalLoadDraws(CGfxMesh* pMesh, DrawHeader* drawHeaders, int numDraws)
{
	for (int indexDraw = 0; indexDraw < numDraws; indexDraw++) {
		LogOutput(LOG_TAG_RENDERER, "\tLoadMeshDraw %s ", drawHeaders[indexDraw].szName);
		{
			const glm::aabb aabb(
				glm::vec3(drawHeaders[indexDraw].minx, drawHeaders[indexDraw].miny, drawHeaders[indexDraw].minz),
				glm::vec3(drawHeaders[indexDraw].maxx, drawHeaders[indexDraw].maxy, drawHeaders[indexDraw].maxz));

			if (pMesh->CreateDraw(indexDraw, aabb, drawHeaders[indexDraw].baseVertex, drawHeaders[indexDraw].firstIndex, drawHeaders[indexDraw].indexCount) == false) {
				goto ERR;
			}
		}
		LogOutput(nullptr, "\n");
	}

	return true;
ERR:
	LogOutput(LOG_TAG_RENDERER, "Fail\n");
	return false;
}


bool CResourceLoader::LoadMesh(const char* szFileName, CGfxMesh* pMesh, int vertexBinding)
{
	int err = 0;

	LogOutput(LOG_TAG_RENDERER, "LoadMesh %s\n", szFileName);
	{
		pMesh->Destroy();

		CMemoryStream stream;
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

	return true;
ERR:
	pMesh->Destroy();
	LogOutput(LOG_TAG_RENDERER, "Fail(%d)\n", err);
	return false;
}
