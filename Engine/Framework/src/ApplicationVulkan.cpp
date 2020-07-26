#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_vulkan.h"

#include "Application.h"
#include "Framework.h"


static CGfxCommandBufferPtr ptrComputeCommandBuffers[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
static CGfxCommandBufferPtr ptrGraphicCommandBuffers[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];


CApplicationVulkan::CApplicationVulkan(void)
{

}

CApplicationVulkan::~CApplicationVulkan(void)
{

}

bool CApplicationVulkan::Create(void* hInstance, void* hWnd, void* hDC, int width, int height)
{
	m_hDC = hDC;
	m_width = width;
	m_height = height;

	//
	// 1. Setup Engine
	//
	CreateEngine(GFX_API_VULKAN, hInstance, hWnd, GetDC((HWND)hWnd), width, height, GFX_PIXELFORMAT_BGRA8_UNORM_PACK8, "../Data");
	CreateFramework(width, height);

	ptrComputeCommandBuffers[0] = GfxRenderer()->NewCommandBuffer(0, true);
	ptrComputeCommandBuffers[1] = GfxRenderer()->NewCommandBuffer(0, true);
	ptrComputeCommandBuffers[2] = GfxRenderer()->NewCommandBuffer(0, true);
	ptrGraphicCommandBuffers[0] = GfxRenderer()->NewCommandBuffer(0, true);
	ptrGraphicCommandBuffers[1] = GfxRenderer()->NewCommandBuffer(0, true);
	ptrGraphicCommandBuffers[2] = GfxRenderer()->NewCommandBuffer(0, true);

	//
	// 2. Setup ImGui
	//
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

	//
	// 3. Setup ImGui style
	//
//	ImGui::StyleColorsDark();
	ImGui::StyleColorsLight();
//	ImGui::StyleColorsClassic();

	//
	// 4. Setup Platform/Renderer bindings
	//
	ImGui_ImplWin32_Init((HWND)hWnd);

	/*
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = g_Instance;
	init_info.PhysicalDevice = g_PhysicalDevice;
	init_info.Device = g_Device;
	init_info.QueueFamily = g_QueueFamily;
	init_info.Queue = g_Queue;
	init_info.PipelineCache = g_PipelineCache;
	init_info.DescriptorPool = g_DescriptorPool;
	init_info.Allocator = g_Allocator;
	init_info.MinImageCount = g_MinImageCount;
	init_info.ImageCount = wd->ImageCount;
	init_info.CheckVkResultFn = check_vk_result;
	ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);
	*/

	return true;
}

void CApplicationVulkan::Destroy(void)
{
	//
	// 1. Destroy ImGui
	//
	ImGui_ImplVulkan_Shutdown();
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

void CApplicationVulkan::UpdateInternal(float deltaTime)
{

}
