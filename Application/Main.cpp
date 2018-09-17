#include "stdafx.h"
#include "Main.h"
#include "Engine.h"

#define MAIN_SCENE 1000

#define MAX_LOADSTRING 100

HINSTANCE hInst;
HWND hWnd;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

DWORD dwScreenWidth;
DWORD dwScreenHeight;

ATOM RegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

void OnLButtonDown(int x, int y);
void OnLButtonUp(int x, int y);
void OnMouseMove(int x, int y);
void OnKeyDown(WPARAM wParam);
void OnKeyUp(WPARAM wParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_ARESENGINE, szWindowClass, MAX_LOADSTRING);
	RegisterClass(hInstance);

	if (InitInstance(hInstance, nCmdShow) == FALSE) {
		return FALSE;
	}

	CreateEngine(GetDC(hWnd), "../Data/Shader", "../Data/Scene", "../Data/Scene", "../Data/Scene");
	{
		CGfxCamera *pMainCamera = Camera("MainCamera");
		pMainCamera->SetClearDepth(1.0f);
		pMainCamera->SetClearColor(0.25f, 0.25f, 0.5f, 1.0f);
		pMainCamera->SetScissor(0.0f, 0.0f, 1.0f * dwScreenWidth, 1.0f * dwScreenHeight);
		pMainCamera->SetViewport(0.0f, 0.0f, 1.0f * dwScreenWidth, 1.0f * dwScreenHeight);
		pMainCamera->SetPerspective(60.0f, 1.0f * dwScreenWidth / dwScreenHeight, 0.1f, 100.0f);
		pMainCamera->SetLookat(0.0f, 20.0f, 5.0f, 0.0f, 10.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		CScene *pScene = SceneManager()->CreateScene(HashValue("Sponza"));
		pScene->Load("../Data/Scene/scene.xml");
		{
			MSG msg;
			HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ARESENGINE));

			uint32_t currTick = 0;
			uint32_t lastLogicTick = 0;
			uint32_t lastRenderTick = 0;

			do {
				if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
					if (TranslateAccelerator(msg.hwnd, hAccelTable, &msg) == false) {
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
				else {
					currTick = tick();

					float deltaLogicTick = (currTick - lastLogicTick) / 1000000.0f;
					float deltaRenderTick = (currTick - lastRenderTick) / 1000000.0f;

					// Logic 30FPS
					if (deltaLogicTick > 1.0f / 30.0f) {
						Engine()->Update(deltaLogicTick);
						lastLogicTick = currTick;
					}

					// Render 60FPS
					if (deltaRenderTick > 1.0f / 60.0f) {
						Engine()->UpdateCamera(pMainCamera);
						Engine()->RenderCamera(pMainCamera);
						Engine()->RenderPresent();
						lastRenderTick = currTick;
					}
				}
			} while (msg.message != WM_QUIT);
		}
		SceneManager()->DestroyScene(pScene);
	}
	DestroyEngine();

	return 0;
}

ATOM RegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ARESENGINE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_ARESENGINE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	RECT rcView;
	rcView.left = 50; rcView.right = rcView.left + 1280;
	rcView.top = 50; rcView.bottom = rcView.top + 720;
	AdjustWindowRectEx(&rcView, WS_TILED | WS_SYSMENU, TRUE, NULL);

	hInst = hInstance;
	hWnd = CreateWindowW(szWindowClass, szTitle, WS_TILED | WS_SYSMENU, rcView.left, rcView.top, rcView.right - rcView.left, rcView.bottom - rcView.top, nullptr, nullptr, hInstance, nullptr);

	if (hWnd == NULL) {
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	GetClientRect(hWnd, &rcView);
	dwScreenWidth = rcView.right - rcView.left + 1;
	dwScreenHeight = rcView.bottom - rcView.top + 1;

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_LBUTTONDOWN:
	{
		OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	}
	break;

	case WM_LBUTTONUP:
	{
		OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	}
	break;

	case WM_MOUSEMOVE:
	{
		OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	}
	break;

	case WM_KEYDOWN:
	{
		OnKeyDown(wParam);
	}
	break;

	case WM_KEYUP:
	{
		OnKeyUp(wParam);
	}
	break;

	case WM_PAINT:
	{
		ValidateRect(hWnd, NULL);
	}
	break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void OnLButtonDown(int x, int y)
{

}

void OnLButtonUp(int x, int y)
{

}

void OnMouseMove(int x, int y)
{

}

void OnKeyDown(WPARAM wParam)
{

}

void OnKeyUp(WPARAM wParam)
{

}
