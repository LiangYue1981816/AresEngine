#include "imgui.h"
#include "imgui_impl_win32.h"

#include "Application.h"
#include "EngineHeader.h"


// Forward declare message handler from imgui_impl_win32.cpp
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

	Sleep(1);
}

LRESULT CApplication::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}
