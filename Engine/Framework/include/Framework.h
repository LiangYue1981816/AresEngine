#pragma once
#include "EngineHeader.h"


class CGame;
class CEditor;

class CFramework
{
public:
	static CFramework* GetInstance(void);
	static void Create(int width, int height);
	static void Destroy(void);


private:
	CFramework(int width, int height);
	virtual ~CFramework(void);


	// Input
public:
	void OnLButtonDown(int x, int y);
	void OnLButtonRelease(int x, int y);
	void OnMouseMove(int x, int y, int ppi = 100);

	void OnKeyDown(int key);
	void OnKeyRelease(int key);

	// Update
public:
	void UpdatePlay(float deltaTime);
	void UpdateEditor(float deltaTime);


private:
	CGame* m_pGame;
	CEditor* m_pEditor;

private:
	static CFramework* pInstance;
};
