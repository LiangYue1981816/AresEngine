#pragma once
#include "WorkModeBase.h"


class CGame : public CWorkModeBase
{
public:
	CGame(int width, int height);
	virtual ~CGame(void);


public:
	void OnLButtonDown(int x, int y);
	void OnLButtonRelease(int x, int y);
	void OnMouseMove(int x, int y, int ppi = 100);

	void OnKeyDown(int key);
	void OnKeyRelease(int key);

public:
	void Update(float deltaTime);
	void UpdateCamera(float deltaTime);


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
};
