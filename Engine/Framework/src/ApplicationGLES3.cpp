#include "Framework.h"
#include "ApplicationGLES3.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"

#include "gl3w.h"
#include "WGLExtensions.h"


extern void UpdateInput(void);
extern void UpdateRenderSolution(void);


CApplicationGLES3::CApplicationGLES3(void)
	: m_hDC(nullptr)
	, m_width(0)
	, m_height(0)
{

}

CApplicationGLES3::~CApplicationGLES3(void)
{

}

bool CApplicationGLES3::Create(void* hInstance, void* hWnd, void* hDC, int width, int height)
{
	m_hDC = hDC;
	m_width = width;
	m_height = height;

	//
	// 1. Setup OpenGL
	//
	struct ContextAttribs {
		int major;
		int minor;
		int profile;
	};

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

	int format = ChoosePixelFormat((HDC)hDC, &pixelFormatDescriptor);
	SetPixelFormat((HDC)hDC, format, &pixelFormatDescriptor);

	HGLRC hOldRC = wglCreateContext((HDC)hDC);
	wglMakeCurrent((HDC)hDC, hOldRC);

	const ContextAttribs& contextAttribs = table[2];  // kGfxLevelES31
//	const ContextAttribs& contextAttribs = table[5];  // kGfxLevelCore33
//	const ContextAttribs& contextAttribs = table[13]; // kGfxLevelCompatibility33
	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, contextAttribs.major,
		WGL_CONTEXT_MINOR_VERSION_ARB, contextAttribs.minor,
		WGL_CONTEXT_FLAGS_ARB,         0,
		WGL_CONTEXT_PROFILE_MASK_ARB,  contextAttribs.profile,
		0
	};

	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)gl3wGetProcAddress("wglCreateContextAttribsARB");
	HGLRC hNewRC = wglCreateContextAttribsARB((HDC)hDC, hOldRC, attribs);

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hOldRC);
	wglMakeCurrent((HDC)hDC, hNewRC);

	gl3wInit();

	//
	// 2. Setup Engine
	//
	CreateEngine(GFX_API_GLES3, hInstance, hWnd, GetDC((HWND)hWnd), width, height, GFX_PIXELFORMAT_BGRA8_UNORM_PACK8, "../Data");
	CreateFramework(width, height);

	//
	// 3. Setup ImGui
	//
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

	//
	// 4. Setup ImGui style
	//
//	ImGui::StyleColorsDark();
	ImGui::StyleColorsLight();
//	ImGui::StyleColorsClassic();

	//
	// 5. Setup Platform/Renderer bindings
	//
	ImGui_ImplWin32_Init((HWND)hWnd);
	ImGui_ImplOpenGL3_Init("#version 300 es");

	//
	// 6. Upload Fonts
	//
	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);
	io.Fonts->AddFontFromFileTTF("VeraMono.ttf", 14.0f);

	return true;
}

void CApplicationGLES3::Destroy(void)
{
	//
	// 1. Destroy ImGui
	//
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//
	// 2. Destroy Engine
	//
	DestroyFramework();
	DestroyEngine();
}

void CApplicationGLES3::UpdateInternal(float deltaTime)
{
	const CGfxSemaphore* pWaitSemaphore = GfxRenderer()->GetSwapChain()->GetAcquireSemaphore();
	const CGfxCommandBufferPtr ptrComputeCommandBuffer = Framework()->GetComputeCommandBuffer();
	const CGfxCommandBufferPtr ptrGraphicCommandBuffer = Framework()->GetGraphicCommandBuffer();

	GfxRenderer()->AcquireNextFrame();
	{
		// Update
		Engine()->Wait();
		{
			UpdateInput();
			UpdateRenderSolution();
			Framework()->Update(deltaTime);
		}
		Engine()->Update();

		// Render
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// Render Scene
		Framework()->Render(ptrComputeCommandBuffer, ptrGraphicCommandBuffer, pWaitSemaphore);

		// Render ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	GfxRenderer()->Present(ptrGraphicCommandBuffer->GetSemaphore());
}
