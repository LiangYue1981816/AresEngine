#include "SceneHeader.h"


CScene::CScene(uint32_t name, CSceneManager *pSceneManager)
	: m_name(name)

	, m_pRootNode(nullptr)
	, m_pSceneManager(pSceneManager)
{
	m_pRootNode = new CSceneNode(HashValue("Root"), this);
}

CScene::~CScene(void)
{
	Free();
	delete m_pRootNode;
}

uint32_t CScene::GetName(void) const
{
	return m_name;
}

CSceneNode* CScene::GetRootNode(void) const
{
	return m_pRootNode;
}

CSceneNode* CScene::GetNode(uint32_t name) const
{
	const auto &itNode = m_pNodes.find(name);
	return itNode != m_pNodes.end() ? itNode->second : nullptr;
}

bool CScene::AttachNode(CSceneNode *pNode)
{
	if (m_pNodes.find(pNode->GetName()) == m_pNodes.end()) {
		m_pNodes.insert(eastl::pair<uint32_t, CSceneNode*>(pNode->GetName(), pNode));
		return true;
	}
	else {
		return false;
	}
}

bool CScene::DetachNode(CSceneNode *pNode)
{
	if (m_pNodes.find(pNode->GetName()) != m_pNodes.end()) {
		m_pNodes.erase(pNode->GetName());
		return true;
	}
	else {
		return false;
	}
}

bool CScene::LoadMesh(const char *szFileName)
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
	do {
		CStream stream;
		if (FileManager()->LoadStream(szFileName, &stream) == false) break;

		TiXmlDocument xmlDoc;
		if (xmlDoc.LoadFile((char *)stream.GetAddress(), stream.GetFullSize()) == false) break;

		TiXmlNode *pMeshNode = xmlDoc.FirstChild("Mesh");
		if (pMeshNode == nullptr) break;
		if (LoadMesh(pMeshNode) == false) break;

		return true;
	} while (false);

	return false;
}

bool CScene::LoadMesh(TiXmlNode *pMeshNode)
{
	bool rcode = true;
	CSceneNode *pCurrentSceneNode = nullptr;

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

			pCurrentSceneNode = m_pSceneManager->CreateNode(m_pSceneManager->GetNextNodeName());
			if (m_pRootNode->AttachNode(pCurrentSceneNode) == false) break;

			do {
				if (LoadNode(ptrMesh, pNode, pCurrentSceneNode) == false) {
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

	if (rcode == false) {
		FreeNode(pCurrentSceneNode);
	}

	return rcode;
}

bool CScene::LoadNode(const CGfxMeshPtr &ptrMesh, TiXmlNode *pNode, CSceneNode *pParentSceneNode)
{
	bool rcode = true;
	CSceneNode *pCurrentSceneNode = nullptr;

	do {
		pCurrentSceneNode = m_pSceneManager->CreateNode(m_pSceneManager->GetNextNodeName());
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

		if (pParentSceneNode->AttachNode(pCurrentSceneNode) == false) {
			rcode = false;
			break;
		}

		if (LoadDraw(ptrMesh, pNode, pCurrentSceneNode) == false) {
			rcode = false;
			break;
		}

		if (TiXmlNode *pChildNode = pNode->FirstChild("Node")) {
			do {
				if (LoadNode(ptrMesh, pChildNode, pCurrentSceneNode) == false) {
					rcode = false;
					break;
				}
			} while ((pChildNode = pNode->IterateChildren("Node", pChildNode)) != nullptr);
		}
	} while (false);

	if (rcode == false) {
		FreeNode(pCurrentSceneNode);
	}

	return rcode;
}

bool CScene::LoadDraw(const CGfxMeshPtr &ptrMesh, TiXmlNode *pNode, CSceneNode *pCurrentSceneNode)
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

				CComponentMeshPtr ptrComponentMesh = m_pSceneManager->CreateComponentMesh(m_pSceneManager->GetNextComponentMeshName());
				ptrComponentMesh->SetMaterial(ptrMaterial);
				ptrComponentMesh->SetMesh(ptrMesh, indexDraw);
				pCurrentSceneNode->AttachComponentMesh(ptrComponentMesh);
			} while ((pDrawNode = pNode->IterateChildren("Draw", pDrawNode)) != nullptr);
		}
	} while (false);

	return rcode;
}

void CScene::FreeNode(CSceneNode *pNode)
{
	if (pNode) {
		m_pSceneManager->DestroyNode(pNode);
	}
}

void CScene::Free(void)
{
	const eastl::unordered_map<uint32_t, CSceneNode*> pNodes = m_pNodes;

	for (const auto &itNode : pNodes) {
		m_pSceneManager->DestroyNode(itNode.second);
	}

	m_pNodes.clear();

}
