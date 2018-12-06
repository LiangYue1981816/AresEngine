#include "SceneHeader.h"
#include "EngineHeader.h"

#include "tinyxml.h"
#include "tinystr.h"


static bool LoadDraw(CSceneManager *pSceneManager, CSceneNode *pCurrentSceneNode, const CGfxMeshPtr &ptrMesh, TiXmlNode *pNode)
{
	bool rcode = true;

	do {
		if (TiXmlNode *pDrawNode = pNode->FirstChild("Draw")) {
			do {
				int indexDraw = pDrawNode->ToElement()->AttributeInt1("index");
				const char *szMaterialFileName = pDrawNode->ToElement()->AttributeString("material");
				if (szMaterialFileName == nullptr) {
					rcode = false;
					break;
				}

				CGfxMaterialPtr ptrMaterial = Renderer()->NewMaterial(szMaterialFileName);
				if (ptrMaterial.IsValid() == false) {
					rcode = false;
					break;
				}

				CComponentMeshPtr ptrComponentMesh = pSceneManager->CreateComponentMesh(pSceneManager->GetNextComponentMeshName());
				ptrComponentMesh->SetMaterial(ptrMaterial);
				ptrComponentMesh->SetMesh(ptrMesh, indexDraw);
				pCurrentSceneNode->AttachComponentMesh(ptrComponentMesh);
			} while ((pDrawNode = pNode->IterateChildren("Draw", pDrawNode)) != nullptr);
		}
	} while (false);

	return rcode;
}

static bool LoadNode(CSceneManager *pSceneManager, CScene *pScene, CSceneNode *pParentSceneNode, const CGfxMeshPtr &ptrMesh, TiXmlNode *pNode)
{
	bool rcode = true;
	CSceneNode *pCurrentSceneNode = nullptr;

	do {
		pCurrentSceneNode = pSceneManager->CreateNode(pSceneManager->GetNextNodeName());
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

		if (LoadDraw(pSceneManager, pCurrentSceneNode, ptrMesh, pNode) == false) {
			rcode = false;
			break;
		}

		if (pParentSceneNode->AttachNode(pCurrentSceneNode) == false) {
			rcode = false;
			break;
		}

		if (TiXmlNode *pChildNode = pNode->FirstChild("Node")) {
			do {
				if (LoadNode(pSceneManager, pScene, pCurrentSceneNode, ptrMesh, pChildNode) == false) {
					rcode = false;
					break;
				}
			} while ((pChildNode = pNode->IterateChildren("Node", pChildNode)) != nullptr);
		}
	} while (false);

	if (rcode == false) {
		pScene->FreeNode(pCurrentSceneNode);
	}

	return rcode;
}

static bool LoadMesh(CSceneManager *pSceneManager, CScene *pScene, CSceneNode *pParentSceneNode, TiXmlNode *pMeshNode)
{
	bool rcode = true;

	do {
		if (TiXmlNode *pNode = pMeshNode->FirstChild("Node")) {
			const char *szMeshFileName = pMeshNode->ToElement()->AttributeString("mesh");
			if (szMeshFileName == nullptr) {
				rcode = false;
				break;
			}

			CGfxMeshPtr ptrMesh = Renderer()->NewMesh(szMeshFileName, INSTANCE_ATTRIBUTE_TRANSFORM);
			if (ptrMesh.IsValid() == false) {
				rcode = false;
				break;
			}

			do {
				if (LoadNode(pSceneManager, pScene, pParentSceneNode, ptrMesh, pNode) == false) {
					rcode = false;
					break;
				}
			} while ((pNode = pMeshNode->IterateChildren("Node", pNode)) != nullptr);
		}
		else {
			rcode = false;
			break;
		}
	} while (false);

	return rcode;
}

static CSceneNode* LoadMesh(const char *szFileName, CSceneManager *pSceneManager, CScene *pScene, CSceneNode *pParentSceneNode)
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

	CSceneNode *pCurrentSceneNode = nullptr;

	if (pParentSceneNode == nullptr) {
		pParentSceneNode = pScene->GetRootNode();
	}

	do {
		CStream stream;
		if (FileManager()->LoadStream(szFileName, &stream) == false) break;

		TiXmlDocument xmlDoc;
		if (xmlDoc.LoadFile((char *)stream.GetAddress(), stream.GetFullSize()) == false) break;

		TiXmlNode *pMeshNode = xmlDoc.FirstChild("Mesh");
		if (pMeshNode == nullptr) break;

		pCurrentSceneNode = pSceneManager->CreateNode(pSceneManager->GetNextNodeName());
		if (LoadMesh(pSceneManager, pScene, pCurrentSceneNode, pMeshNode) == false) break;
		if (pParentSceneNode->AttachNode(pCurrentSceneNode) == false) break;

		return pCurrentSceneNode;
	} while (false);

	pScene->FreeNode(pCurrentSceneNode);
	return nullptr;
}

CSceneNode* CScene::LoadMesh(const char *szFileName, CSceneNode *pParentSceneNode)
{
	return ::LoadMesh(szFileName, m_pSceneManager, this, pParentSceneNode);
}

void CScene::FreeNode(CSceneNode *pNode)
{
	m_pSceneManager->DestroyNode(pNode);
}
