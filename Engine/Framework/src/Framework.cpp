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
	, m_workMode(WORK_MODE_EDITOR)
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

void CFramework::SetWorkMode(CFramework::WorkMode mode)
{
	m_workMode = mode;
}

CWorkModeBase* CFramework::GetWorkMode(void) const
{
	switch (m_workMode) {
	case WORK_MODE_GAME:
		return m_pGame;

	case WORK_MODE_EDITOR:
		return m_pEditor;
	}

	ASSERT(0);
	return nullptr;
}

void CFramework::OnLButtonDown(int x, int y)
{
	GetWorkMode()->OnLButtonDown(x, y);
}

void CFramework::OnLButtonRelease(int x, int y)
{
	GetWorkMode()->OnLButtonRelease(x, y);
}

void CFramework::OnMouseMove(int x, int y, int ppi)
{
	GetWorkMode()->OnMouseMove(x, y, ppi);
}

void CFramework::OnKeyDown(int key)
{
	GetWorkMode()->OnKeyDown(key);
}

void CFramework::OnKeyRelease(int key)
{
	GetWorkMode()->OnKeyRelease(key);
}

void CFramework::Update(float deltaTime)
{
	GetWorkMode()->Update(deltaTime);
}
