#pragma once
#include "EngineHeader.h"


class CWorkModeBase
{
public:
	CWorkModeBase(void)
	{

	}
	virtual ~CWorkModeBase(void)
	{

	}


public:
	virtual void OnLButtonDown(int x, int y) = 0;
	virtual void OnLButtonRelease(int x, int y) = 0;
	virtual void OnMouseMove(int x, int y, int ppi = 100) = 0;

	virtual void OnKeyDown(int key) = 0;
	virtual void OnKeyRelease(int key) = 0;

public:
	virtual void Update(float deltaTime) = 0;
};
