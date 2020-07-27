#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_vulkan.h"

#include "Application.h"
#include "Framework.h"
#include "VKRenderer.h"


extern void UpdateInput(void);
extern void UpdateRenderSolution(void);

static VkDescriptorPool vkDescriptorPool = VK_NULL_HANDLE;
static VkRenderPass vkRenderPass = VK_NULL_HANDLE;
static VkImageView vkImageViews[3] = { VK_NULL_HANDLE , VK_NULL_HANDLE , VK_NULL_HANDLE };
static VkFramebuffer vkFramebuffers[3] = { VK_NULL_HANDLE , VK_NULL_HANDLE , VK_NULL_HANDLE };

static CGfxCommandBufferPtr ptrComputeCommandBuffers[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
static CGfxCommandBufferPtr ptrGraphicCommandBuffers[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];
static CGfxCommandBufferPtr ptrImGuiCommandBuffers[CGfxSwapChain::SWAPCHAIN_FRAME_COUNT];


static void check_vk_result(VkResult err)
{
	if (err == 0)
		return;

	printf("[vulkan] Error: VkResult = %d\n", err);

	if (err < 0)
		abort();
}

static void ImGui_ImplVulkan_CreateDescriptorPool(VkAllocationCallbacks* allocator, VkDescriptorPool* pool)
{
	VkDescriptorPoolSize pool_sizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};
	VkDescriptorPoolCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
	info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
	info.pPoolSizes = pool_sizes;
	check_vk_result(vkCreateDescriptorPool(VKRenderer()->GetDevice(), &info, allocator, pool));
}

static void ImGui_ImplVulkan_DestroyDescriptorPool(VkAllocationCallbacks* allocator, VkDescriptorPool* pool)
{
	vkDestroyDescriptorPool(VKRenderer()->GetDevice(), *pool, allocator);
	*pool = VK_NULL_HANDLE;
}

static void ImGui_ImplVulkan_CreateWindow(uint32_t width, uint32_t height, VkFormat format, VkAllocationCallbacks* allocator, VkRenderPass* renderPass, VkImageView* imageViews, VkFramebuffer* framebuffers)
{
	{
		VkAttachmentDescription attachment = {};
		attachment.format = format;
		attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		attachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		VkAttachmentReference color_attachment = {};
		color_attachment.attachment = 0;
		color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &color_attachment;
		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		VkRenderPassCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		info.attachmentCount = 1;
		info.pAttachments = &attachment;
		info.subpassCount = 1;
		info.pSubpasses = &subpass;
		info.dependencyCount = 1;
		info.pDependencies = &dependency;
		check_vk_result(vkCreateRenderPass(VKRenderer()->GetDevice(), &info, allocator, renderPass));
	}
	{
		VkImageViewCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		info.format = format;
		info.components.r = VK_COMPONENT_SWIZZLE_R;
		info.components.g = VK_COMPONENT_SWIZZLE_G;
		info.components.b = VK_COMPONENT_SWIZZLE_B;
		info.components.a = VK_COMPONENT_SWIZZLE_A;
		VkImageSubresourceRange image_range = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
		info.subresourceRange = image_range;
		for (uint32_t i = 0; i < 3; i++)
		{
			info.image = VKRenderer()->GetSwapchainImage(i);
			check_vk_result(vkCreateImageView(VKRenderer()->GetDevice(), &info, allocator, &imageViews[i]));
		}
	}
	{
		VkImageView attachment[1];
		VkFramebufferCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		info.renderPass = *renderPass;
		info.attachmentCount = 1;
		info.pAttachments = attachment;
		info.width = width;
		info.height = height;
		info.layers = 1;
		for (uint32_t i = 0; i < 3; i++)
		{
			attachment[0] = imageViews[i];
			check_vk_result(vkCreateFramebuffer(VKRenderer()->GetDevice(), &info, allocator, &framebuffers[i]));
		}
	}
}

static void ImGui_ImplVulkan_DestroyWindow(VkAllocationCallbacks* allocator, VkRenderPass* renderPass, VkImageView* imageViews, VkFramebuffer* framebuffers)
{
	for (uint32_t i = 0; i < 3; i++) {
		vkDestroyFramebuffer(VKRenderer()->GetDevice(), framebuffers[i], allocator);
		framebuffers[i] = VK_NULL_HANDLE;
	}

	for (uint32_t i = 0; i < 3; i++) {
		vkDestroyImageView(VKRenderer()->GetDevice(), imageViews[i], allocator);
		imageViews[i] = VK_NULL_HANDLE;
	}

	vkDestroyRenderPass(VKRenderer()->GetDevice(), *renderPass, allocator);
	*renderPass = VK_NULL_HANDLE;
}


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
	ptrImGuiCommandBuffers[0] = GfxRenderer()->NewCommandBuffer(0, true);
	ptrImGuiCommandBuffers[1] = GfxRenderer()->NewCommandBuffer(1, true);
	ptrImGuiCommandBuffers[2] = GfxRenderer()->NewCommandBuffer(2, true);

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
	ImGui_ImplVulkan_CreateDescriptorPool(nullptr, &vkDescriptorPool);
	ImGui_ImplVulkan_CreateWindow(m_width, m_height, (VkFormat)GFX_PIXELFORMAT_BGRA8_UNORM_PACK8, nullptr, &vkRenderPass, vkImageViews, vkFramebuffers);

	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = VKRenderer()->GetInstance();
	init_info.PhysicalDevice = VKRenderer()->GetPhysicalDevice();
	init_info.Device = VKRenderer()->GetDevice();
	init_info.QueueFamily = VKRenderer()->GetQueueFamilyIndex();
	init_info.Queue = VKRenderer()->GetQueue();
	init_info.PipelineCache = VK_NULL_HANDLE;
	init_info.DescriptorPool = vkDescriptorPool;
	init_info.Allocator = nullptr;
	init_info.MinImageCount = 3;
	init_info.ImageCount = 3;
	init_info.CheckVkResultFn = check_vk_result;
	ImGui_ImplVulkan_Init(&init_info, vkRenderPass);

	return true;
}

void CApplicationVulkan::Destroy(void)
{
	//
	// 1. Destroy ImGui
	//
	ImGui_ImplVulkan_DestroyDescriptorPool(nullptr, &vkDescriptorPool);
	ImGui_ImplVulkan_DestroyWindow(nullptr, &vkRenderPass, vkImageViews, vkFramebuffers);
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
	ptrImGuiCommandBuffers[0].Release();
	ptrImGuiCommandBuffers[1].Release();
	ptrImGuiCommandBuffers[2].Release();

	DestroyFramework();
	DestroyEngine();
}

void CApplicationVulkan::UpdateInternal(float deltaTime)
{
	const CGfxSemaphore* pWaitSemaphore = GfxRenderer()->GetSwapChain()->GetAcquireSemaphore();
	const CGfxCommandBufferPtr ptrComputeCommandBuffer = ptrComputeCommandBuffers[GfxRenderer()->GetSwapChain()->GetFrameIndex()];
	const CGfxCommandBufferPtr ptrGraphicCommandBuffer = ptrGraphicCommandBuffers[GfxRenderer()->GetSwapChain()->GetFrameIndex()];
	const CGfxCommandBufferPtr ptrImGuiCommandBuffer = ptrImGuiCommandBuffers[GfxRenderer()->GetSwapChain()->GetFrameIndex()];

	GfxRenderer()->AcquireNextFrame();
	{
		ImGui_ImplVulkan_NewFrame();
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
//		ImGui::Render();
//		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	GfxRenderer()->Present(ptrGraphicCommandBuffer->GetSemaphore());
}
