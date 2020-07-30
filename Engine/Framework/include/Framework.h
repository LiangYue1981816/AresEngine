#pragma once
#include "EngineHeader.h"


#define CreateFramework(width, height) CFramework::Create((width), (height))
#define DestroyFramework() CFramework::Destroy()
#define Framework() CFramework::GetInstance()


class CGame;
class CEditor;
class CWorkModeBase;

class CFramework
{
public:
	enum WorkMode
	{
		WORK_MODE_GAME,
		WORK_MODE_EDITOR,
	};


public:
	static CFramework* GetInstance(void);
	static void Create(int width, int height);
	static void Destroy(void);


private:
	CFramework(int width, int height);
	virtual ~CFramework(void);


public:
	void SetWorkMode(WorkMode workMode);
	CWorkModeBase* GetWorkMode(void) const;

public:
	void OnLButtonDown(int x, int y);
	void OnLButtonRelease(int x, int y);
	void OnMouseMove(int x, int y, int ppi = 100);

	void OnKeyDown(int key);
	void OnKeyRelease(int key);

public:
	void Update(float deltaTime);
	void Render(CGfxCommandBufferPtr ptrComputeCommandBuffer, CGfxCommandBufferPtr ptrGraphicCommandBuffer, const CGfxSemaphore* pWaitSemaphore);


private:
	CGame* m_pGame;
	CEditor* m_pEditor;
	WorkMode m_workMode;

private:
	CImGUI_Console* m_pImGUI_Console;

private:
	static CFramework* pInstance;
};
