#include "Game.h"
#include "Editor.h"
#include "Framework.h"


CFramework* CFramework::pInstance = nullptr;
CFramework* CFramework::GetInstance(void)
{
	return pInstance;
}

void CFramework::Create(int width, int height)
{
	if (pInstance == nullptr) {
		pInstance = new CFramework(width, height);
	}
}

void CFramework::Destroy(void)
{
	if (pInstance) {
		delete pInstance;
		pInstance = nullptr;
	}
}


CFramework::CFramework(int width, int height)
	: m_pGame(nullptr)
	, m_pEditor(nullptr)
{
	pInstance = this;

	m_pGame = new CGame(width, height);
	m_pEditor = new CEditor(width, height);
}

CFramework::~CFramework(void)
{
	delete m_pGame;
	delete m_pEditor;
}

void CFramework::OnLButtonDown(int x, int y)
{

}

void CFramework::OnLButtonRelease(int x, int y)
{

}

void CFramework::OnMouseMove(int x, int y, int ppi)
{

}

void CFramework::OnKeyDown(int key)
{

}

void CFramework::OnKeyRelease(int key)
{

}

void CFramework::UpdatePlay(float deltaTime)
{

}

void CFramework::UpdateEditor(float deltaTime)
{

}
