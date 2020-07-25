#pragma once
#include "EngineHeader.h"


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
	void UpdateGame(float deltaTime);
	void UpdateEditor(float deltaTime);

private:
	void UpdateEditorMainCamera(float deltaTime);


private:
	float m_cameraAngleX;
	float m_cameraAngleY;
	float m_cameraMoveSpeed;

private:
	POINT m_ptMousePosition;
	bool m_bLButtonDown;
	bool m_bMoveForward;
	bool m_bMoveBackward;
	bool m_bMoveLeft;
	bool m_bMoveRight;
	bool m_bMoveUp;
	bool m_bMoveDown;

private:
	CCamera* m_pMainCamera;

private:
	static CFramework* pInstance;
};
