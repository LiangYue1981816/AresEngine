#include "gl3w.h"
#include "WGLExtensions.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"

#include "Application.h"
#include "Framework.h"


void UpdateInput(void);
void UpdateRenderSolution(void);

CGfxCommandBufferPtr ptrComputeCommandBuffers[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
CGfxCommandBufferPtr ptrGraphicCommandBuffers[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];


CApplicationOpenGL::CApplicationOpenGL(void)
{

}

CApplicationOpenGL::~CApplicationOpenGL(void)
{

}

bool CApplicationOpenGL::Create(void* hInstance, void* hWnd, void* hDC, int width, int height)
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

	if (gl3wInit() != NO_ERROR) {
		return false;
	}

	//
	// 2. Setup Engine
	//
	CreateEngine(GFX_API_GLES3, hInstance, hWnd, GetDC((HWND)hWnd), width, height, GFX_PIXELFORMAT_BGRA8_UNORM_PACK8);

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

	ptrComputeCommandBuffers[0] = GfxRenderer()->NewCommandBuffer(0, true);
	ptrComputeCommandBuffers[1] = GfxRenderer()->NewCommandBuffer(0, true);
	ptrComputeCommandBuffers[2] = GfxRenderer()->NewCommandBuffer(0, true);
	ptrGraphicCommandBuffers[0] = GfxRenderer()->NewCommandBuffer(0, true);
	ptrGraphicCommandBuffers[1] = GfxRenderer()->NewCommandBuffer(0, true);
	ptrGraphicCommandBuffers[2] = GfxRenderer()->NewCommandBuffer(0, true);

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
	if (ImGui_ImplWin32_Init((HWND)hWnd) == false) {
		return false;
	}

	if (ImGui_ImplOpenGL3_Init("#version 300 es") == false) {
		return false;
	}

	return true;
}

void CApplicationOpenGL::Destroy(void)
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
	ptrComputeCommandBuffers[0].Release();
	ptrComputeCommandBuffers[1].Release();
	ptrComputeCommandBuffers[2].Release();
	ptrGraphicCommandBuffers[0].Release();
	ptrGraphicCommandBuffers[1].Release();
	ptrGraphicCommandBuffers[2].Release();

	DestroyFramework();
	DestroyEngine();
}

void CApplicationOpenGL::UpdateInternal(float deltaTime)
{
	const CGfxSemaphore* pWaitSemaphore = GfxRenderer()->GetSwapChain()->GetAcquireSemaphore();
	const CGfxCommandBufferPtr ptrComputeCommandBuffer = ptrComputeCommandBuffers[GfxRenderer()->GetSwapChain()->GetFrameIndex()];
	const CGfxCommandBufferPtr ptrGraphicCommandBuffer = ptrGraphicCommandBuffers[GfxRenderer()->GetSwapChain()->GetFrameIndex()];

	GfxRenderer()->AcquireNextFrame();
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		static bool show_demo_window = true;
		static bool show_another_window = false;
		static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		// Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// Update
		Engine()->Wait();
		{
			UpdateInput();
			UpdateRenderSolution();
			Framework()->Update(deltaTime);
		}
		Engine()->Update();

		// Render Scene
		Framework()->Render(ptrComputeCommandBuffer, ptrGraphicCommandBuffer, pWaitSemaphore);

		// Render ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	GfxRenderer()->Present(ptrGraphicCommandBuffer->GetSemaphore());
}

void UpdateInput(void)
{
	// Mouse
	{
		ImVec2 curMousePos = ImGui::GetMousePos();
		int x = (int)curMousePos.x;
		int y = (int)curMousePos.y;

		if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
			Framework()->OnLButtonDown(x, y);
		}

		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
			Framework()->OnLButtonRelease(x, y);
		}

		Framework()->OnMouseMove(x, y);
	}

	// Key
	{
		if (ImGui::IsKeyDown('W')) Framework()->OnKeyDown('W');
		if (ImGui::IsKeyDown('S')) Framework()->OnKeyDown('S');
		if (ImGui::IsKeyDown('A')) Framework()->OnKeyDown('A');
		if (ImGui::IsKeyDown('D')) Framework()->OnKeyDown('D');
		if (ImGui::IsKeyDown('E')) Framework()->OnKeyDown('E');
		if (ImGui::IsKeyDown('Q')) Framework()->OnKeyDown('Q');

		if (ImGui::IsKeyReleased('W')) Framework()->OnKeyRelease('W');
		if (ImGui::IsKeyReleased('S')) Framework()->OnKeyRelease('S');
		if (ImGui::IsKeyReleased('A')) Framework()->OnKeyRelease('A');
		if (ImGui::IsKeyReleased('D')) Framework()->OnKeyRelease('D');
		if (ImGui::IsKeyReleased('E')) Framework()->OnKeyRelease('E');
		if (ImGui::IsKeyReleased('Q')) Framework()->OnKeyRelease('Q');
	}
}

void UpdateRenderSolution(void)
{
	float shRed[9] = { 0.588794f, 0.254506f, 0.082485f, 0.028278f, 0.036788f, 0.086514f, 0.018185f, 0.021567f, 0.006525f };
	float shGreen[9] = { 0.599826f, 0.275154f, 0.076468f, 0.028166f, 0.032798f, 0.082039f, 0.014064f, 0.020624f, 0.001984f };
	float shBlue[9] = { 0.558644f, 0.387983f, 0.073534f, 0.024760f, 0.029393f, 0.076786f, 0.008863f, 0.017515f, -0.004231f };

	static float angle = 0.0f; angle += 0.002f;
	glm::vec4 directLightDirection = glm::rotate(glm::mat4(), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(0.35f, -1.0f, 0.0f, 0.0f);

	RenderSystem()->SetEnvLightFactor(1.5f);

	RenderSystem()->SetAmbientLightFactor(0.5f);
	RenderSystem()->SetAmbientLightSH(shRed, shGreen, shBlue);

	RenderSystem()->SetMainDirectLightFactor(1.5f);
	RenderSystem()->SetMainDirectLightColor(3.5f, 3.5f, 3.5f);
	RenderSystem()->SetMainDirectLightDirection(directLightDirection.x, directLightDirection.y, directLightDirection.z);

	RenderSystem()->SetMainPointLightFactor(1.0f);
	RenderSystem()->SetMainPointLightColor(1.0f, 1.0f, 1.0f);
	RenderSystem()->SetMainPointLightPosition(0.0f, 0.0f, 0.0f, 1.0f);
	RenderSystem()->SetMainPointLightAttenuation(1.0f, 1.0f, 1.0f);

	Settings()->SetValue("RenderSystem.Shadow.Factor", 1.0f);
	Settings()->SetValue("RenderSystem.Shadow.SplitFactor0", exp(-4.0f));
	Settings()->SetValue("RenderSystem.Shadow.SplitFactor1", exp(-3.0f));
	Settings()->SetValue("RenderSystem.Shadow.SplitFactor2", exp(-2.0f));
	Settings()->SetValue("RenderSystem.Shadow.SplitFactor3", exp(-1.0f));
}
