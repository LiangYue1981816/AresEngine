#include "imgui.h"
#include "imgui_impl_win32.h"

#include "Application.h"
#include "EngineHeader.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


CApplication::CApplication(void)
	: m_hDC(nullptr)
	, m_width(0)
	, m_height(0)
{

}

CApplication::~CApplication(void)
{

}

void CApplication::Update(void)
{
	/*
	static uint32_t lastTick = 0;
	float deltaTime = (Tick() - lastTick) / 1000000.0f;

	if (deltaTime > 1.0f / 60.0f) {
		static bool bFirstFrame = true;
		{
			if (bFirstFrame == false) {
				UpdateInternal(deltaTime);
			}
		}
		bFirstFrame = false;

		lastTick = Tick();
	}
	/*/
	uint32_t currTick = Tick();
	{
		static uint32_t lastTick = 0;
		UpdateInternal((currTick - lastTick) / 1000000.0f);
		lastTick = currTick;
	}
	//*/
}

int64_t CApplication::WndProc(void* hWnd, uint32_t msg, uint64_t wParam, int64_t lParam)
{
	return ImGui_ImplWin32_WndProcHandler((HWND)hWnd, msg, wParam, lParam);
}
