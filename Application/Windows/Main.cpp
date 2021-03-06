#include "stdafx.h"
#include "Main.h"
#include "EngineHeader.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;
HWND hWnd;
DWORD dwViewWidth;
DWORD dwViewHeight;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
ATOM RegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
BOOL InitOpenGL(HDC);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

float angleX = 0.0f;
float angleY = 90.0f;
BOOL bLButtonDown = FALSE;
BOOL bMoveForward = FALSE;
BOOL bMoveBackward = FALSE;
BOOL bMoveLeft = FALSE;
BOOL bMoveRight = FALSE;
BOOL bMoveUp = FALSE;
BOOL bMoveDown = FALSE;
POINT ptLastPoint = { -1, -1 };
void OnLButtonDown(int x, int y);
void OnLButtonUp(int x, int y);
void OnMouseMove(int x, int y, int ppi = 100);
void OnKeyDown(WPARAM wParam);
void OnKeyUp(WPARAM wParam);
void UpdateCamera(CCamera* pCamera, float deltaTime);
void UpdateRenderSolution(void);

#define RAND() ((rand() % 10000) / 10000.0f)


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	InitAllocator();

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_ARESENGINE, szWindowClass, MAX_LOADSTRING);
	RegisterClass(hInstance);

	if (InitInstance(hInstance, nCmdShow) == FALSE) {
		return FALSE;
	}
	/*
	if (InitOpenGL(GetDC(hWnd)) == FALSE) {
		return FALSE;
	}
	*/
	CGfxProfiler::SetEnableProfiler(true);

//	CreateEngine(GFX_API_GLES3, hInstance, hWnd, GetDC(hWnd), dwViewWidth, dwViewHeight, GFX_PIXELFORMAT_BGRA8_UNORM_PACK8);
	CreateEngine(GFX_API_VULKAN, hInstance, hWnd, GetDC(hWnd), dwViewWidth, dwViewHeight, GFX_PIXELFORMAT_BGRA8_UNORM_PACK8);
	{
		FileManager()->SetPath("../Data", ".xml");
		FileManager()->SetPath("../Data", ".png");
		FileManager()->SetPath("../Data", ".tga");
		FileManager()->SetPath("../Data", ".dds");
		FileManager()->SetPath("../Data", ".glsl");
		FileManager()->SetPath("../Data", ".vert");
		FileManager()->SetPath("../Data", ".frag");
		FileManager()->SetPath("../Data", ".comp");
		FileManager()->SetPath("../Data", ".mesh");
		FileManager()->SetPath("../Data", ".material");
		//FileManager()->SetPath("../Data/Engine", ".vert");
		//FileManager()->SetPath("../Data/Engine", ".frag");
		//FileManager()->SetPack("../Data/Scene/Sponza.zip", ".xml");
		//FileManager()->SetPack("../Data/Scene/Sponza.zip", ".png");
		//FileManager()->SetPack("../Data/Scene/Sponza.zip", ".tga");
		//FileManager()->SetPack("../Data/Scene/Sponza.zip", ".dds");
		//FileManager()->SetPack("../Data/Scene/Sponza.zip", ".glsl");
		//FileManager()->SetPack("../Data/Scene/Sponza.zip", ".vert");
		//FileManager()->SetPack("../Data/Scene/Sponza.zip", ".frag");
		//FileManager()->SetPack("../Data/Scene/Sponza.zip", ".comp");
		//FileManager()->SetPack("../Data/Scene/Sponza.zip", ".mesh");
		//FileManager()->SetPack("../Data/Scene/Sponza.zip", ".material");

		CCamera* pCamera = new CCamera;

		CScene *pLightScene = SceneManager()->CreateScene(HashValue("LightScene"));
		{
			float maxSize = 1.0f;
			float maxRangeX = 13.5f;
			float maxRangeY = 11.0f;
			float maxRangeZ = 6.0f;

			srand(0x0816);

			pLightScene->GetRootNode()->SetWorldScale(1.0f, 1.0f, 1.0f);
			pLightScene->GetRootNode()->SetWorldPosition(-0.5f, 0.0f, 0.0f);

			for (int index = 0; index < 200; index++) {
				CSceneNode *pPointLightNode = SceneManager()->CreateNode(SceneManager()->GetNextNodeName());
				float scale = RAND() * maxSize;
				float positionx = (2.0f * RAND() - 1.0f) * maxRangeX;
				float positiony = RAND() * maxRangeY;
				float positionz = (2.0f * RAND() - 1.0f) * maxRangeZ;
				pPointLightNode->SetWorldScale(scale, scale, scale);
				pPointLightNode->SetWorldPosition(positionx, positiony, positionz);
				pPointLightNode->AttachComponentPointLight(SceneManager()->CreateComponentPointLight(SceneManager()->GetNextComponentPointLightName()));
				pLightScene->GetRootNode()->AttachNode(pPointLightNode);
			}
		}

		CScene *pMainScene = SceneManager()->CreateScene(HashValue("MainScene"));
		{
			pMainScene->GetRootNode()->SetWorldScale(1.0f, 1.0f, 1.0f);
			pMainScene->GetRootNode()->SetWorldPosition(0.0f, 0.0f, 0.0f);

			CSceneNode *pSponzaSceneNode = ResourceLoader()->LoadSceneMesh("Sponza.xml", pMainScene->GetRootNode(), VERTEX_BINDING, INSTANCE_BINDING);
			pSponzaSceneNode->SetWorldScale(1.0f, 1.0f, 1.0f);
			pSponzaSceneNode->SetWorldPosition(0.0f, 0.0f, 0.0f);

			CSceneNode *pMarcusSceneNode = ResourceLoader()->LoadSceneMesh("Marcus.xml", pMainScene->GetRootNode(), VERTEX_BINDING, INSTANCE_BINDING);
			pMarcusSceneNode->SetWorldScale(1.0f, 1.0f, 1.0f);
			pMarcusSceneNode->SetWorldPosition(0.0f, 0.0f, 0.0f);
			pMarcusSceneNode->SetWorldDirection(-1.0f, 0.0f, -1.0);

			CSceneNode *pHeadSceneNode = ResourceLoader()->LoadSceneMesh("MaleHead.xml", pMainScene->GetRootNode(), VERTEX_BINDING, INSTANCE_BINDING);
			pHeadSceneNode->SetWorldScale(1.0f, 1.0f, 1.0f);
			pHeadSceneNode->SetWorldPosition(-1.0f, 1.0f, 0.0f);
			pHeadSceneNode->SetWorldDirection(-1.0f, 0.0f, -1.0f);
		}

		uint32_t frameTime = 0;
		uint32_t frameCount = 0;
		uint32_t lastEngineTick = 0;
		uint32_t lastProfilerTick = 0;

		MSG msg;
		HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ARESENGINE));

		do {
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				if (TranslateAccelerator(msg.hwnd, hAccelTable, &msg) == false) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else {
				uint32_t currTick = Tick();

				float deltaEngineTick = (currTick - lastEngineTick) / 1000000.0f;
				float deltaProfilerTick = (currTick - lastProfilerTick) / 1000000.0f;

				// Engine 60FPS
				if (deltaEngineTick > 1.0f / 60.0f) {
					uint32_t begin = Tick();
					{
						Engine()->Wait();
						{
							UpdateRenderSolution();
							UpdateCamera(pCamera, deltaEngineTick);
						}
						Engine()->Update();

						static bool bFirstFrame = true;
						{
							if (bFirstFrame == false) {
//								Engine()->RenderDefault(pCamera, true);
								Engine()->RenderForwardLighting(pCamera, true);
//								Engine()->RenderDeferredLighting(pCamera, true);
//								Engine()->RenderTileBaseDeferredLighting(pCamera, true);
							}
						}
						bFirstFrame = false;
					}
					uint32_t end = Tick();

					frameTime += end - begin;
					frameCount += 1;
					lastEngineTick = currTick;
				}

				// Profiler 1FPS
				if (deltaProfilerTick > 1.0f) {
					CGfxProfiler::LogGfxMemory();
					CGfxProfiler::LogProfiler(frameCount);
					CGfxProfiler::ResetSamples();

					LogOutput("Frame", "Time = %fms\n", frameTime / frameCount / 1000.0f);

					frameTime = 0;
					frameCount = 0;
					lastProfilerTick = currTick;
				}

				Sleep(1);
			}
		} while (msg.message != WM_QUIT);

		SceneManager()->DestroyScene(pMainScene);
		SceneManager()->DestroyScene(pLightScene);
		delete pCamera;

	}
	DestroyEngine();

//	ExitAllocator();
	//*/
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
	rcView.left = 50; rcView.right = rcView.left + 1024;
	rcView.top = 50; rcView.bottom = rcView.top + 576;
	AdjustWindowRectEx(&rcView, WS_TILED | WS_SYSMENU, TRUE, 0);

	hInst = hInstance;
	hWnd = CreateWindowW(szWindowClass, szTitle, WS_TILED | WS_SYSMENU, rcView.left, rcView.top, rcView.right - rcView.left, rcView.bottom - rcView.top, nullptr, nullptr, hInstance, nullptr);

	if (hWnd == NULL) {
		return FALSE;
	}

	ImmAssociateContext(hWnd, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	GetClientRect(hWnd, &rcView);
	dwViewWidth = rcView.right - rcView.left;
	dwViewHeight = rcView.bottom - rcView.top;

	return TRUE;
}

struct ContextAttribs
{
	int major;
	int minor;
	int profile;
};

const ContextAttribs& GetContextAttrib(int index)
{
	static const ContextAttribs table[] = {
		{2, 0, WGL_CONTEXT_ES_PROFILE_BIT_EXT},            // 0 kGfxLevelES2,
		{3, 0, WGL_CONTEXT_ES_PROFILE_BIT_EXT},            // 1 kGfxLevelES3,
		{3, 1, WGL_CONTEXT_ES_PROFILE_BIT_EXT},            // 2 kGfxLevelES31,
		{3, 1, WGL_CONTEXT_ES_PROFILE_BIT_EXT},            // 3 kGfxLevelES31AEP

		{3, 2, WGL_CONTEXT_CORE_PROFILE_BIT_ARB},          // 4 kGfxLevelCore32,
		{3, 3, WGL_CONTEXT_CORE_PROFILE_BIT_ARB},          // 5 kGfxLevelCore33,
		{4, 0, WGL_CONTEXT_CORE_PROFILE_BIT_ARB},          // 6 kGfxLevelCore40,
		{4, 1, WGL_CONTEXT_CORE_PROFILE_BIT_ARB},          // 7 kGfxLevelCore41,
		{4, 2, WGL_CONTEXT_CORE_PROFILE_BIT_ARB},          // 8 kGfxLevelCore42,
		{4, 3, WGL_CONTEXT_CORE_PROFILE_BIT_ARB},          // 9 kGfxLevelCore43,
		{4, 4, WGL_CONTEXT_CORE_PROFILE_BIT_ARB},          // 10 kGfxLevelCore44,
		{4, 5, WGL_CONTEXT_CORE_PROFILE_BIT_ARB},          // 11 kGfxLevelCore45

		{3, 2, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB}, // 12 kGfxLevelCompatibility32,
		{3, 3, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB}, // 13 kGfxLevelCompatibility33,
		{4, 0, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB}, // 14 kGfxLevelCompatibility40,
		{4, 1, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB}, // 15 kGfxLevelCompatibility41,
		{4, 2, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB}, // 16 kGfxLevelCompatibility42,
		{4, 3, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB}, // 17 kGfxLevelCompatibility43,
		{4, 4, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB}, // 18 kGfxLevelCompatibility44,
		{4, 5, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB}, // 19 kGfxLevelCompatibility45
	};

	return table[index];
}

BOOL InitOpenGL(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelFormatDescriptor = {
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd 
		1,                              // version number 
		PFD_DRAW_TO_WINDOW |            // support window 
		PFD_SUPPORT_OPENGL |            // support OpenGL 
		PFD_DOUBLEBUFFER,               // double buffered 
		PFD_TYPE_RGBA,                  // RGBA type 
		32,                             // 32-bit color depth 
		0, 0, 0, 0, 0, 0,               // color bits ignored 
		0,                              // no alpha buffer 
		0,                              // shift bit ignored 
		0,                              // no accumulation buffer 
		0, 0, 0, 0,                     // accum bits ignored 
		24,                             // 24-bit z-buffer 
		8,                              // 8-bit stencil buffer 
		0,                              // no auxiliary buffer 
		PFD_MAIN_PLANE,                 // main layer 
		0,                              // reserved 
		0, 0, 0                         // layer masks ignored 
	};

	int format = ChoosePixelFormat(hDC, &pixelFormatDescriptor);
	SetPixelFormat(hDC, format, &pixelFormatDescriptor);

	HGLRC hOldRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hOldRC);

	const ContextAttribs& contextAttribs = GetContextAttrib(2);  // kGfxLevelES31
//	const ContextAttribs& contextAttribs = GetContextAttrib(5);  // kGfxLevelCore33
//	const ContextAttribs& contextAttribs = GetContextAttrib(13); // kGfxLevelCompatibility33
	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, contextAttribs.major,
		WGL_CONTEXT_MINOR_VERSION_ARB, contextAttribs.minor,
		WGL_CONTEXT_FLAGS_ARB,         0,
		WGL_CONTEXT_PROFILE_MASK_ARB,  contextAttribs.profile,
		0
	};

	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)gl3wGetProcAddress("wglCreateContextAttribsARB");
	HGLRC hNewRC = wglCreateContextAttribsARB(hDC, hOldRC, attribs);

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hOldRC);
	wglMakeCurrent(hDC, hNewRC);

	uint32_t err = gl3wInit();
	return err == NO_ERROR ? TRUE : FALSE;
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

		case ID_RENDERSOLUTION_DEFAULT:
			SetWindowText(hWnd, "Default");
			break;

		case ID_RENDERSOLUTION_FORWARD:
			SetWindowText(hWnd, "Forward");
			break;

		case ID_RENDERSOLUTION_DEFERRED:
			SetWindowText(hWnd, "Deferred");
			break;

		case ID_RENDERSOLUTION_TILED_BASE_DEFERRED:
			SetWindowText(hWnd, "TiledBaseDeferred");
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_LBUTTONDOWN:
		OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;

	case WM_LBUTTONUP:
		OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;

	case WM_MOUSEMOVE:
		OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;

	case WM_KEYDOWN:
		OnKeyDown(wParam);
		break;

	case WM_KEYUP:
		OnKeyUp(wParam);
		break;

	case WM_PAINT:
		ValidateRect(hWnd, NULL);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
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
	ptLastPoint.x = x;
	ptLastPoint.y = y;

	bLButtonDown = TRUE;
}

void OnLButtonUp(int x, int y)
{
	ptLastPoint.x = -1;
	ptLastPoint.y = -1;

	bMoveForward = FALSE;
	bMoveBackward = FALSE;
	bMoveLeft = FALSE;
	bMoveRight = FALSE;
	bMoveUp = FALSE;
	bMoveDown = FALSE;

	bLButtonDown = FALSE;
}

void OnMouseMove(int x, int y, int ppi)
{
	if (bLButtonDown == TRUE) {
		if (ptLastPoint.x != -1 && ptLastPoint.y != -1) {
#ifndef PLATFORM_WINDOWS
			if (x < dwViewWidth / 2) {
				if (y > ptLastPoint.y) {
					bMoveForward = FALSE;
					bMoveBackward = TRUE;
				}
				if (y < ptLastPoint.y) {
					bMoveForward = TRUE;
					bMoveBackward = FALSE;
				}

				if (x > ptLastPoint.x) {
					bMoveLeft = FALSE;
					bMoveRight = TRUE;
				}
				if (x < ptLastPoint.x) {
					bMoveLeft = TRUE;
					bMoveRight = FALSE;
				}
			}
			else
#endif
			{
				angleX += 0.5f * (y - ptLastPoint.y) * 100.0f / ppi;
				angleY += 0.5f * (ptLastPoint.x - x) * 100.0f / ppi;

				ptLastPoint.x = x;
				ptLastPoint.y = y;
			}
		}
	}
}

void OnKeyDown(WPARAM wParam)
{
	switch (wParam) {
	case 'W': bMoveForward = TRUE; break;
	case 'S': bMoveBackward = TRUE; break;
	case 'A': bMoveLeft = TRUE; break;
	case 'D': bMoveRight = TRUE; break;
	case 'E': bMoveUp = TRUE; break;
	case 'Q': bMoveDown = TRUE; break;
	}
}

void OnKeyUp(WPARAM wParam)
{
	switch (wParam) {
	case 'W': bMoveForward = FALSE; break;
	case 'S': bMoveBackward = FALSE; break;
	case 'A': bMoveLeft = FALSE; break;
	case 'D': bMoveRight = FALSE; break;
	case 'E': bMoveUp = FALSE; break;
	case 'Q': bMoveDown = FALSE; break;
	}
}

void UpdateCamera(CCamera* pCamera, float deltaTime)
{
	static float speed = 2.5f;
	static glm::vec3 position(0.0f, 0.0f, 0.0f);
	static glm::vec3 forward(0.0f, 0.0f, 1.0f);
	static glm::vec3 up(0.0f, 1.0f, 0.0f);
	static glm::vec3 left(1.0f, 0.0f, 0.0f);

	// Viewport and Perspective
	{
		pCamera->SetScissor(0.0f, 0.0f, 1.0f * dwViewWidth, 1.0f * dwViewHeight);
		pCamera->SetViewport(0.0f, 0.0f, 1.0f * dwViewWidth, 1.0f * dwViewHeight);
		pCamera->SetPerspective(45.0f, 1.0f * dwViewWidth / dwViewHeight, 0.1f, 250.0f);
	}

	// Rotate
	{
		if (angleX < -85.0f) angleX = -85.0f;
		if (angleX >  85.0f) angleX =  85.0f;

		glm::mat4 rotatex = glm::rotate(glm::mat4(), glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotatey = glm::rotate(glm::mat4(), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotation = rotatey * rotatex;

		forward = glm::mat3(rotation) * glm::vec3(0.0f, 0.0f, 1.0f);
		left = glm::cross(up, forward);

		pCamera->SetLookat(position.x, position.y, position.z, position.x + forward.x, position.y + forward.y, position.z + forward.z, 0.0f, 1.0f, 0.0f);
	}

	// Translate
	{
		if (bMoveForward) position += forward * speed * deltaTime;
		if (bMoveBackward) position -= forward * speed * deltaTime;
		if (bMoveLeft) position += left * speed * deltaTime;
		if (bMoveRight) position -= left * speed * deltaTime;
		if (bMoveUp) position += up * speed * deltaTime;
		if (bMoveDown) position -= up * speed * deltaTime;

		pCamera->SetLookat(position.x, position.y, position.z, position.x + forward.x, position.y + forward.y, position.z + forward.z, 0.0f, 1.0f, 0.0f);
	}
}

void UpdateRenderSolution(void)
{
	float shRed[9] = { 0.588794f, 0.254506f, 0.082485f, 0.028278f, 0.036788f, 0.086514f, 0.018185f, 0.021567f, 0.006525f };
	float shGreen[9] = { 0.599826f, 0.275154f, 0.076468f, 0.028166f, 0.032798f, 0.082039f, 0.014064f, 0.020624f, 0.001984f };
	float shBlue[9] = { 0.558644f, 0.387983f, 0.073534f, 0.024760f, 0.029393f, 0.076786f, 0.008863f, 0.017515f, -0.004231f };

	RenderSystem()->SetEnvLightFactor(1.5f);

	RenderSystem()->SetAmbientLightFactor(1.0f);
	RenderSystem()->SetAmbientLightSH(shRed, shGreen, shBlue);

	RenderSystem()->SetMainDirectLightFactor(2.0f);
	RenderSystem()->SetMainDirectLightColor(1.0f, 1.0f, 1.0f);
	RenderSystem()->SetMainDirectLightDirection(0.45f, -1.0f, 0.15f);

	RenderSystem()->SetMainPointLightFactor(0.0f);
	RenderSystem()->SetMainPointLightColor(1.0f, 1.0f, 1.0f);
	RenderSystem()->SetMainPointLightPosition(0.0f, 0.0f, 0.0f, 1.0f);
	RenderSystem()->SetMainPointLightAttenuation(1.0f, 1.0f, 1.0f);

	Settings()->SetValue("RenderSystem.Shadow.Factor", 0.75f);
}
