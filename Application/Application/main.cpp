#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "Application.h"


enum RendererType {
	RENDERER_TYPE_OPENGL,
	RENDERER_TYPE_VULKAN
};


// Global variable
RendererType type = RENDERER_TYPE_VULKAN;
CApplication* pApplication = NULL;


// Forward declarations of helper functions
void ParseCommandLine(int argc, char** argv);
LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


// Main code
int main(int argc, char** argv)
{
	//
	// 1. Paser command line
	//
	ParseCommandLine(argc, argv);

	//
	// 2. Create window
	//
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "Framework", NULL }; RegisterClassEx(&wc);
	HWND hWnd = CreateWindow(wc.lpszClassName, "Framework", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 100, 100, 1300, 700, NULL, NULL, wc.hInstance, NULL);

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	//
	// 3. Create application
	//
	RECT rcWindow;
	GetClientRect(hWnd, &rcWindow);

	switch (type) {
	case RENDERER_TYPE_OPENGL:
		pApplication = new CApplicationOpenGL;
		break;

	case RENDERER_TYPE_VULKAN:
		pApplication = new CApplicationVulkan;
		break;

	default:
		pApplication = new CApplicationVulkan;
		break;
	}

	pApplication->Create(wc.hInstance, hWnd, GetDC((HWND)hWnd), rcWindow.right - rcWindow.left + 1, rcWindow.bottom - rcWindow.top + 1);

	//
	// 4. Main loop
	//
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		pApplication->Update();
	}

	//
	// 5. Destroy application
	//
	pApplication->Destroy();
	delete pApplication;

	//
	// 6. Destroy window
	//
	DestroyWindow(hWnd);
	UnregisterClass(wc.lpszClassName, wc.hInstance);

	return 0;
}

// Paser command line
void ParseCommandLine(int argc, char** argv)
{
	type = RENDERER_TYPE_VULKAN;

	if (argc > 1) {
		if (!stricmp(argv[1], "-opengl")) {
			type = RENDERER_TYPE_OPENGL;
		}

		if (!stricmp(argv[1], "-vulkan")) {
			type = RENDERER_TYPE_VULKAN;
		}
	}
}

// Win32 message handler
LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (pApplication->WndProc(hWnd, msg, wParam, lParam)) {
		return true;
	}

	switch (msg)
	{
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
