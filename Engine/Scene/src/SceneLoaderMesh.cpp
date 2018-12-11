#include "SceneHeader.h"
#include "EngineHeader.h"

#include "tinyxml.h"
#include "tinystr.h"


static bool LoadDraw(TiXmlNode *pNode, const CGfxMeshPtr &ptrMesh, CSceneNode *pCurrentSceneNode)
{
	int err = 0;

	if (TiXmlNode *pDrawNode = pNode->FirstChild("Draw")) {
		do {
			int indexDraw = pDrawNode->ToElement()->AttributeInt1("index");
			const char *szMaterialFileName = pDrawNode->ToElement()->AttributeString("material");
			if (szMaterialFileName == nullptr) { err = -1; goto ERR; }

			CGfxMaterialPtr ptrMaterial = GfxRenderer()->NewMaterial(szMaterialFileName);
			if (ptrMaterial.IsValid() == false) { err = -2; goto ERR; }

			CComponentMeshPtr ptrComponentMesh = pCurrentSceneNode->GetSceneManager()->CreateComponentMesh(pCurrentSceneNode->GetSceneManager()->GetNextComponentMeshName());
			ptrComponentMesh->SetMaterial(ptrMaterial);
			ptrComponentMesh->SetMesh(ptrMesh, indexDraw);
			pCurrentSceneNode->AttachComponentMesh(ptrComponentMesh);
		} while ((pDrawNode = pNode->IterateChildren("Draw", pDrawNode)) != nullptr);
	}
	return true;
ERR:
	return false;
}

static bool LoadNode(TiXmlNode *pNode, const CGfxMeshPtr &ptrMesh, CSceneNode *pParentSceneNode)
{
	int err = 0;
	CSceneNode *pCurrentSceneNode = nullptr;
	{
		pCurrentSceneNode = pParentSceneNode->GetSceneManager()->CreateNode(pParentSceneNode->GetSceneManager()->GetNextNodeName());
		{
			float scale[3];
			float rotation[4];
			float translation[3];
			pNode->ToElement()->AttributeFloat3("scale", scale);
			pNode->ToElement()->AttributeFloat4("rotation", rotation);
			pNode->ToElement()->AttributeFloat3("translation", translation);

			pCurrentSceneNode->SetLocalScale(scale[0], scale[1], scale[2]);
			pCurrentSceneNode->SetLocalOrientation(rotation[0], rotation[1], rotation[2], rotation[3]);
			pCurrentSceneNode->SetLocalPosition(translation[0], translation[1], translation[2]);
		}

		if (LoadDraw(pNode, ptrMesh, pCurrentSceneNode) == false) {
			err = -1; goto ERR;
		}

		if (pParentSceneNode->AttachNode(pCurrentSceneNode) == false) {
			err = -2; goto ERR;
		}

		if (TiXmlNode *pChildNode = pNode->FirstChild("Node")) {
			do {
				if (LoadNode(pChildNode, ptrMesh, pCurrentSceneNode) == false) {
					err = -3; goto ERR;
				}
			} while ((pChildNode = pNode->IterateChildren("Node", pChildNode)) != nullptr);
		}
	}
	return true;
ERR:
	pParentSceneNode->GetSceneManager()->DestroyNode(pCurrentSceneNode);
	return false;
}

static bool LoadMesh(TiXmlNode *pMeshNode, CSceneNode *pParentSceneNode)
{
	int err = 0;
	{
		if (TiXmlNode *pNode = pMeshNode->FirstChild("Node")) {
			const char *szMeshFileName = pMeshNode->ToElement()->AttributeString("mesh");
			if (szMeshFileName == nullptr) { err = -1; goto ERR; }

			CGfxMeshPtr ptrMesh = GfxRenderer()->NewMesh(szMeshFileName, INSTANCE_ATTRIBUTE_TRANSFORM);
			if (ptrMesh.IsValid() == false) { err = -2; goto ERR; }

			do {
				if (LoadNode(pNode, ptrMesh, pParentSceneNode) == false) { err = -3; goto ERR; }
			} while ((pNode = pMeshNode->IterateChildren("Node", pNode)) != nullptr);
		}
	}
	return true;
ERR:
	return false;
}

static CSceneNode* LoadMesh(const char *szFileName, CSceneNode *pParentSceneNode)
{
	/*
	<Mesh mesh="sponza.mesh">
		<Node translation="0.000000 0.000000 0.000000" rotation="0.000000 0.000000 0.000000 1.000000" scale="1.000000 1.000000 1.000000">
			<Node translation="0.000000 0.000000 0.000000" rotation="-0.707107 -0.000000 0.000000 0.707107" scale="1.000000 1.000000 1.000000">
				<Draw index="0" material="leaf.material" />
			</Node>
		</Node>
	</<Mesh>
	*/

	int err = 0;
	CSceneNode *pCurrentSceneNode = nullptr;
	{
		CStream stream;
		if (FileManager()->LoadStream(szFileName, &stream) == false) { err = -1; goto ERR; }

		TiXmlDocument xmlDoc;
		if (xmlDoc.LoadFile((char *)stream.GetAddress(), stream.GetFullSize()) == false) { err = -2; goto ERR; }

		TiXmlNode *pMeshNode = xmlDoc.FirstChild("Mesh");
		if (pMeshNode == nullptr) { err = -3; goto ERR; }

		pCurrentSceneNode = pParentSceneNode->GetSceneManager()->CreateNode(pParentSceneNode->GetSceneManager()->GetNextNodeName());
		if (LoadMesh(pMeshNode, pCurrentSceneNode) == false) { err = -4; goto ERR; }
		if (pParentSceneNode->AttachNode(pCurrentSceneNode) == false) { err = -5; goto ERR; }
	}
	return pCurrentSceneNode;
ERR:
	pParentSceneNode->GetSceneManager()->DestroyNode(pCurrentSceneNode);
	return nullptr;
}

CSceneNode* CSceneLoader::LoadMesh(const char *szFileName, CSceneNode *pParentSceneNode)
{
	return ::LoadMesh(szFileName, pParentSceneNode);
}
