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
float angleY = 0.0f;
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
void UpdateCamera(CGfxCamera *pCamera, float deltaTime);

void TestAllocator(void);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	/*
	InitAllocator();
	TestAllocator();
	ExitAllocator();
	/*/
	InitAllocator();

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_ARESENGINE, szWindowClass, MAX_LOADSTRING);
	RegisterClass(hInstance);

	if (InitInstance(hInstance, nCmdShow) == FALSE) {
		return FALSE;
	}

	if (InitOpenGL(GetDC(hWnd)) == FALSE) {
		return FALSE;
	}

	CGfxProfiler::SetEnableProfiler(true);

	CreateEngine(GFX_API_GLES3, RENDER_SOLUTION_DEFAULT, hInstance, hWnd, GetDC(hWnd), dwViewWidth, dwViewHeight, GFX_PIXELFORMAT_RGBA8_UNORM_PACK8);
//	CreateEngine(GFX_API_VULKAN, RENDER_SOLUTION_DEFAULT, hInstance, hWnd, GetDC(hWnd), dwViewWidth, dwViewHeight, GFX_PIXELFORMAT_RGBA8_UNORM_PACK8);
	{
		GfxRenderer()->SetShaderCachePath("../Data/Engine/ShaderCache");
		GfxRenderer()->SetShaderIncludePath("../Data/Engine/Shader/inc");

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

		RenderSolution()->SetMainDirectLightColor(3.0f, 3.0f, 3.0f);
		RenderSolution()->SetMainDirectLightDirection(-1.0f, -1.0f, 1.0f);

		CSceneNode *pPointLightNode = SceneManager()->CreateNode(HashValue("PointLight"));
		pPointLightNode->SetWorldScale(1.0f, 1.0f, 1.0f);
		pPointLightNode->SetWorldPosition(0.0f, 0.0f, 0.0f);

		CComponentPointLightPtr ptrComponentPointLight = SceneManager()->CreateComponentPointLight(SceneManager()->GetNextComponentPointLightName());
		pPointLightNode->AttachComponentPointLight(ptrComponentPointLight);

		MainCamera()->SetScissor(0.0f, 0.0f, 1.0f * dwViewWidth, 1.0f * dwViewHeight);
		MainCamera()->SetViewport(0.0f, 0.0f, 1.0f * dwViewWidth, 1.0f * dwViewHeight);
		MainCamera()->SetPerspective(60.0f, 1.0f * dwViewWidth / dwViewHeight, 0.1f, 100.0f);
		MainCamera()->SetLookat(0.0f, 20.0f, 5.0f, 0.0f, 10.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		CScene *pMainScene[5][5] = { nullptr };
		for (int y = -2; y <= 2; y++) {
			for (int x = -2; x <= 2; x++) {
				pMainScene[y + 2][x + 2] = SceneManager()->CreateScene((y + 2) * 5 + (x + 2));
				pMainScene[y + 2][x + 2]->GetRootNode()->SetWorldScale(0.5f, 0.5f, 0.5f);
				pMainScene[y + 2][x + 2]->GetRootNode()->SetWorldPosition(x * 17.5f, 0.0f, y * 10.0f);
				ResourceLoader()->LoadSceneMesh("sponza.xml", pMainScene[y + 2][x + 2]->GetRootNode(), INSTANCE_FORMAT_TRANSFORM, 0, 1);
			}
		}
		{
			uint32_t frameCount = 0;
			uint32_t lastEngineTick = 0;
			uint32_t lastProfilerTick = 0;

			uint32_t frameTime = 0;

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
						UpdateCamera(MainCamera(), deltaEngineTick);

						uint32_t begin = Tick();
						{
							Engine()->Update();
							Engine()->Present();
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
		}
		for (int y = -2; y <= 2; y++) {
			for (int x = -2; x <= 2; x++) {
				SceneManager()->DestroyScene(pMainScene[y + 2][x + 2]);
			}
		}
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
	rcView.left = 50; rcView.right = rcView.left + 850;
	rcView.top = 50; rcView.bottom = rcView.top + 480;
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
	dwViewWidth = rcView.right - rcView.left + 1;
	dwViewHeight = rcView.bottom - rcView.top + 1;

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

	int pixelFormat = ChoosePixelFormat(hDC, &pixelFormatDescriptor);
	SetPixelFormat(hDC, pixelFormat, &pixelFormatDescriptor);

	HGLRC hOldRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hOldRC);

	const ContextAttribs& contextAttribs = GetContextAttrib(5);  // kGfxLevelCore33
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
	case VK_SPACE: bMoveUp = TRUE; break;
	case VK_SHIFT: bMoveDown = TRUE; break;
	}
}

void OnKeyUp(WPARAM wParam)
{
	switch (wParam) {
	case 'W': bMoveForward = FALSE; break;
	case 'S': bMoveBackward = FALSE; break;
	case 'A': bMoveLeft = FALSE; break;
	case 'D': bMoveRight = FALSE; break;
	case VK_SPACE: bMoveUp = FALSE; break;
	case VK_SHIFT: bMoveDown = FALSE; break;
	}
}

void UpdateCamera(CGfxCamera *pCamera, float deltaTime)
{
	// Rotate
	{
		if (angleX < -85.0f) angleX = -85.0f;
		if (angleX >  85.0f) angleX =  85.0f;

		glm::mat4 rotatex = glm::rotate(glm::mat4(), glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotatey = glm::rotate(glm::mat4(), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotation = rotatey * rotatex;

		glm::vec3 position = pCamera->GetPosition();
		glm::vec3 forward = glm::mat3(rotation) * glm::vec3(0.0f, 0.0f, 1.0f);
		pCamera->SetLookat(position.x, position.y, position.z, position.x + forward.x, position.y + forward.y, position.z + forward.z, 0.0f, 1.0f, 0.0f);
	}

	// Translate
	{
		float speed = 10.0f;

		glm::vec3 position = pCamera->GetPosition();
		glm::vec3 forward = pCamera->GetForwardDirection();
		glm::vec3 up = pCamera->GetUpDirection();
		glm::vec3 left = glm::cross(up, forward);

		if (bMoveForward) position += forward * speed * deltaTime;
		if (bMoveBackward) position -= forward * speed * deltaTime;
		if (bMoveLeft) position += left * speed * deltaTime;
		if (bMoveRight) position -= left * speed * deltaTime;
		if (bMoveUp) position += up * speed * deltaTime;
		if (bMoveDown) position -= up * speed * deltaTime;

		pCamera->SetLookat(position.x, position.y, position.z, position.x + forward.x, position.y + forward.y, position.z + forward.z, 0.0f, 1.0f, 0.0f);
	}
}


static const uint32_t loop = 10000;
static const uint32_t count = 50000;
static uint32_t size[count] = { 0 };
static void *pointer[count] = { nullptr };
static bool allocate[count] = { false };

class CMemTestTask : public CTask
{
public:
	CMemTestTask(void)
	{

	}
	~CMemTestTask(void)
	{

	}

public:
	void SetParams(uint32_t minSize, uint32_t maxSize, uint32_t indexBegin, uint32_t indexEnd)
	{
		m_minSize = minSize;
		m_maxSize = maxSize;
		m_indexBegin = indexBegin;
		m_indexEnd = indexEnd;
	}

	void TaskFunc(void *pParams)
	{
		if (pParams) {
			AllocatorThread(m_minSize, m_maxSize, m_indexBegin, m_indexEnd);
		}
		else {
			SystemOSThread(m_minSize, m_maxSize, m_indexBegin, m_indexEnd);
		}
	}

private:
	void AllocatorThread(uint32_t minSize, uint32_t maxSize, uint32_t indexBegin, uint32_t indexEnd)
	{
		uint32_t begin = Tick();
		{
			for (uint32_t indexLoop = 0; indexLoop < loop; indexLoop++) {
				for (uint32_t index = indexBegin; index < indexEnd; index++) {
					if (allocate[index]) {
						allocate[index] = false;
						pointer[index] = AllocMemory(size[index]);
					}
					else {
						allocate[index] = true;
						if (pointer[index]) {
							FreeMemory(pointer[index]);
							pointer[index] = nullptr;
						}
					}
				}
			}
		}
		uint32_t end = Tick();

		for (uint32_t index = indexBegin; index < indexEnd; index++) {
			if (pointer[index]) {
				FreeMemory(pointer[index]);
				pointer[index] = nullptr;
			}
		}

		LogOutput("MemTest Allocator", "[%d, %d] = %f\n", minSize, maxSize, (end - begin) / 1000000.0f);
	}

	void SystemOSThread(uint32_t minSize, uint32_t maxSize, uint32_t indexBegin, uint32_t indexEnd)
	{
		uint32_t begin = Tick();
		{
			for (uint32_t indexLoop = 0; indexLoop < loop; indexLoop++) {
				for (uint32_t index = indexBegin; index < indexEnd; index++) {
					if (allocate[index]) {
						allocate[index] = false;
						pointer[index] = malloc(size[index]);
					}
					else {
						allocate[index] = true;
						if (pointer[index]) {
							free(pointer[index]);
							pointer[index] = nullptr;
						}
					}
				}
			}
		}
		uint32_t end = Tick();

		for (uint32_t index = indexBegin; index < indexEnd; index++) {
			if (pointer[index]) {
				free(pointer[index]);
				pointer[index] = nullptr;
			}
		}

		LogOutput("MemTest SystemOS", "[%d, %d] = %f\n", minSize, maxSize, (end - begin) / 1000000.0f);
	}

private:
	uint32_t m_minSize;
	uint32_t m_maxSize;
	uint32_t m_indexBegin;
	uint32_t m_indexEnd;
};

void TestAllocator(uint32_t minSize, uint32_t maxSize)
{
	srand(Tick());
	for (uint32_t index = 0; index < count; index++) {
		size[index] = (rand() % (maxSize - minSize)) + minSize;
		allocate[index] = (rand() % 2) == 0;
	}

	CTaskGraph taskGraph("");
	CMemTestTask tasks[THREAD_COUNT];
	uint32_t step = count / THREAD_COUNT;

	for (uint32_t indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
		tasks[indexThread].SetParams(minSize, maxSize, indexThread * step, (indexThread + 1) * step);
	}

	{
		for (uint32_t indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			taskGraph.Task(&tasks[indexThread], nullptr, nullptr, nullptr);
		}

		taskGraph.Dispatch();
		taskGraph.Wait();
	}
	{
		for (uint32_t indexThread = 0; indexThread < THREAD_COUNT; indexThread++) {
			taskGraph.Task(&tasks[indexThread], &indexThread, nullptr, nullptr);
		}

		taskGraph.Dispatch();
		taskGraph.Wait();
	}
}

void TestAllocator()
{
	TestAllocator(4, 16);
	TestAllocator(16, 64);
	TestAllocator(64, 128);
}
