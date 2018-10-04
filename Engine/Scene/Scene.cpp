#include "Engine.h"
#include "Scene.h"


CScene::CScene(uint32_t name)
	: m_name(name)
	, m_pRootNode(NULL)
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

CSceneNode* CScene::GetNode(uint32_t name) const
{
	const auto &itNode = m_pNodes.find(name);
	return itNode != m_pNodes.end() ? itNode->second : NULL;
}

bool CScene::Load(const char *szFileName)
{
	/*
	<Scene>
		<Node translation = "0.000000 0.000000 0.000000" rotation = "0.000000 0.000000 0.000000 1.000000" scale = "1.000000 1.000000 1.000000">
			<Node translation = "0.000000 0.000000 0.000000" rotation = "-0.707107 -0.000000 0.000000 0.707107" scale = "1.000000 1.000000 1.000000">
				<Mesh mesh = "./sponza_00.mesh" material = "./leaf.material" / >
			</Node>
		</Node>
	</Scene>
	*/
	try {
		Free();

		TiXmlDocument xmlDoc;
		if (xmlDoc.LoadFile(Renderer()->GetResourceFullName(szFileName)) == false) throw 0;

		TiXmlNode *pSceneNode = xmlDoc.FirstChild("Scene");
		if (pSceneNode == NULL) throw 1;
		if (LoadScene(pSceneNode) == false) throw 2;

		return true;
	}
	catch (int) {
		Free();
		return false;
	}
}

bool CScene::LoadScene(TiXmlNode *pSceneNode)
{
	try {
		if (TiXmlNode *pNode = pSceneNode->FirstChild("Node")) {
			do {
				if (LoadNode(pNode, m_pRootNode) == false) {
					throw 0;
				}
			} while (pNode = pSceneNode->IterateChildren("Node", pNode));
		}
		return true;
	}
	catch (int) {
		return false;
	}
}

bool CScene::LoadNode(TiXmlNode *pNode, CSceneNode *pParentNode)
{
	try {
		float scale[3];
		float rotation[4];
		float translation[3];
		pNode->ToElement()->AttributeFloat3("scale", scale);
		pNode->ToElement()->AttributeFloat4("rotation", rotation);
		pNode->ToElement()->AttributeFloat3("translation", translation);

		CSceneNode *pCurrentNode = SceneManager()->CreateNode(SceneManager()->GetNextNodeName());
		pCurrentNode->SetLocalPosition(translation[0], translation[1], translation[2]);
		pCurrentNode->SetLocalOrientation(rotation[0], rotation[1], rotation[2], rotation[3]);
		pCurrentNode->SetLocalScale(scale[0], scale[1], scale[2]);

		if (pParentNode->AttachNode(pCurrentNode) == false) {
			throw 0;
		}

		if (LoadMesh(pNode, pCurrentNode) == false) {
			throw 1;
		}

		if (TiXmlNode *pChildNode = pNode->FirstChild("Node")) {
			do {
				if (LoadNode(pChildNode, pCurrentNode) == false) {
					throw 2;
				}
			} while (pChildNode = pNode->IterateChildren("Node", pChildNode));
		}

		return true;
	}
	catch (int) {
		return false;
	}
}

bool CScene::LoadMesh(TiXmlNode *pNode, CSceneNode *pCurrentNode)
{
	try {
		if (TiXmlNode *pMeshNode = pNode->FirstChild("Mesh")) {
			const char *szMeshFileName = pMeshNode->ToElement()->AttributeString("mesh");
			const char *szMaterialFileName = pMeshNode->ToElement()->AttributeString("material");
			if (szMeshFileName == NULL || szMaterialFileName == NULL) throw 0;

			CGfxMeshPtr ptrMesh = Renderer()->LoadMesh(szMeshFileName);
			CGfxMaterialPtr ptrMaterial = Renderer()->LoadMaterial(szMaterialFileName);

			CComponentMeshPtr ptrComponentMesh = SceneManager()->CreateComponentMesh(SceneManager()->GetNextComponentMeshName());
			ptrComponentMesh->SetMesh(ptrMesh);
			ptrComponentMesh->SetMaterial(ptrMaterial);
			pCurrentNode->AttachComponentMesh(ptrComponentMesh);
		}
		return true;
	}
	catch (int) {
		return false;
	}
}

void CScene::Free(void)
{
	const eastl::unordered_map<uint32_t, CSceneNode*> pNodes = m_pNodes;

	for (const auto &itNode : pNodes) {
		SceneManager()->DestroyNode(itNode.second);
	}

	m_pNodes.clear();
}
