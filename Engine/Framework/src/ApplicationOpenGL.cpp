#include "gl3w.h"
#include "WGLExtensions.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"

#include "Application.h"
#include "Framework.h"


#define RAND() ((rand() % 10000) / 10000.0f)


float speed = 1.5f;
float angleX = 0.0f;
float angleY = 135.0f;
BOOL bLButtonDown = FALSE;
BOOL bMoveForward = FALSE;
BOOL bMoveBackward = FALSE;
BOOL bMoveLeft = FALSE;
BOOL bMoveRight = FALSE;
BOOL bMoveUp = FALSE;
BOOL bMoveDown = FALSE;
POINT ptLastPoint = { -1, -1 };

CCamera* pMainCamera = nullptr;
CScene* pLightScene = nullptr;
CScene* pMainScene = nullptr;

CGfxCommandBufferPtr ptrComputeCommandBuffers[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
CGfxCommandBufferPtr ptrGraphicCommandBuffers[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];


void OnLButtonDown(int x, int y);
void OnLButtonUp(int x, int y);
void OnMouseMove(int x, int y, int ppi = 100);
void OnKeyDown(WPARAM wParam);
void OnKeyUp(WPARAM wParam);

void UpdateInput(void);
void UpdateCamera(CCamera* pCamera, int width, int height, float deltaTime);
void UpdateRenderSolution(void);


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

	pMainCamera = new CCamera;

	pLightScene = SceneManager()->GetOrCreateScene(HashValue("LightScene"));
	{
		float maxSize = 1.0f;
		float maxRangeX = 13.5f;
		float maxRangeY = 11.0f;
		float maxRangeZ = 6.0f;

		srand(0x0816);

		pLightScene->GetRootNode()->SetWorldScale(1.0f, 1.0f, 1.0f);
		pLightScene->GetRootNode()->SetWorldPosition(-0.5f, 0.0f, 0.0f);

		for (int index = 0; index < 200; index++) {
			float scale = RAND() * maxSize;
			float positionx = (2.0f * RAND() - 1.0f) * maxRangeX;
			float positiony = RAND() * maxRangeY;
			float positionz = (2.0f * RAND() - 1.0f) * maxRangeZ;

			CComponentPointLightPtr ptrPointLight = SceneManager()->GetOrCreateComponentPointLight(SceneManager()->GetNextComponentPointLightName());
			ptrPointLight->SetColor(4.0f, 4.0f, 4.0f);
			ptrPointLight->SetAttenuation(3.0f, 2.0f, 1.0f, scale);

			CSceneNode *pPointLightNode = SceneManager()->GetOrCreateNode(SceneManager()->GetNextNodeName());
			pPointLightNode->SetWorldScale(scale, scale, scale);
			pPointLightNode->SetWorldPosition(positionx, positiony, positionz);
			pPointLightNode->AttachComponentPointLight(ptrPointLight);
			pLightScene->GetRootNode()->AttachNode(pPointLightNode);
		}
	}

	pMainScene = SceneManager()->GetOrCreateScene(HashValue("MainScene"));
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
	SceneManager()->DestroyScene(pMainScene);
	SceneManager()->DestroyScene(pLightScene);
	delete pMainCamera;

	ptrComputeCommandBuffers[0].Release();
	ptrComputeCommandBuffers[1].Release();
	ptrComputeCommandBuffers[2].Release();
	ptrGraphicCommandBuffers[0].Release();
	ptrGraphicCommandBuffers[1].Release();
	ptrGraphicCommandBuffers[2].Release();

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

		Engine()->Wait();
		{
			UpdateInput();
			UpdateRenderSolution();
			UpdateCamera(pMainCamera, m_width, m_height, deltaTime);
		}
		Engine()->Update();
		Engine()->RenderTileDeferredShading(pMainCamera, ptrComputeCommandBuffer, ptrGraphicCommandBuffer, pWaitSemaphore);

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	GfxRenderer()->Present(ptrGraphicCommandBuffer->GetSemaphore());
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
			angleX += 0.5f * (y - ptLastPoint.y) * 100.0f / ppi;
			angleY += 0.5f * (ptLastPoint.x - x) * 100.0f / ppi;

			ptLastPoint.x = x;
			ptLastPoint.y = y;
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

void UpdateInput(void)
{
	// Mouse
	{
		ImVec2 curMousePos = ImGui::GetMousePos();
		int x = (int)curMousePos.x;
		int y = (int)curMousePos.y;

		if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
			OnLButtonDown(x, y);
		}

		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
			OnLButtonUp(x, y);
		}

		OnMouseMove(x, y);
	}

	// Key
	{
		if (ImGui::IsKeyDown('W')) OnKeyDown('W');
		if (ImGui::IsKeyDown('S')) OnKeyDown('S');
		if (ImGui::IsKeyDown('A')) OnKeyDown('A');
		if (ImGui::IsKeyDown('D')) OnKeyDown('D');
		if (ImGui::IsKeyDown('E')) OnKeyDown('E');
		if (ImGui::IsKeyDown('Q')) OnKeyDown('Q');

		if (ImGui::IsKeyReleased('W')) OnKeyUp('W');
		if (ImGui::IsKeyReleased('S')) OnKeyUp('S');
		if (ImGui::IsKeyReleased('A')) OnKeyUp('A');
		if (ImGui::IsKeyReleased('D')) OnKeyUp('D');
		if (ImGui::IsKeyReleased('E')) OnKeyUp('E');
		if (ImGui::IsKeyReleased('Q')) OnKeyUp('Q');
	}
}

void UpdateCamera(CCamera* pCamera, int width, int height, float deltaTime)
{
	static glm::vec3 position(-2.09197688f, 1.52254741f, 1.16247618f);
	static glm::vec3 forward(0.0f, 0.0f, 1.0f);
	static glm::vec3 up(0.0f, 1.0f, 0.0f);
	static glm::vec3 left(1.0f, 0.0f, 0.0f);

	// Viewport and Perspective
	{
		pCamera->SetScissor(0.0f, 0.0f, 1.0f * width, 1.0f * height);
		pCamera->SetViewport(0.0f, 0.0f, 1.0f * width, 1.0f * height);
		pCamera->SetPerspective(45.0f, 1.0f * width / height, 0.01f, 150.0f);
	}

	// Rotate
	{
		if (angleX < -85.0f) angleX = -85.0f;
		if (angleX > 85.0f) angleX = 85.0f;

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
