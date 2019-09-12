#include "SceneHeader.h"


CScene::CScene(uint32_t name, CSceneManager* pSceneManager)
	: m_name(name)

	, m_pRootNode(nullptr)
	, m_pSceneManager(pSceneManager)
{
	m_pRootNode = new CSceneNode(HashValue("Root"), m_pSceneManager);
}

CScene::~CScene(void)
{
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

CSceneManager* CScene::GetSceneManager(void) const
{
	return m_pSceneManager;
}
