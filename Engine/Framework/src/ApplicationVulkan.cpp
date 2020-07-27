#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_vulkan.h"

#include "Application.h"
#include "Framework.h"
#include "VKRenderer.h"


void UpdateInput(void);
void UpdateRenderSolution(void);


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

}

void UpdateInput(void)
{
	// Mouse
	{
		ImVec2 curMousePos = ImGui::GetMousePos();
		int x = (int)curMousePos.x;
		int y = (int)curMousePos.y;

		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsAnyWindowHovered() == false) {
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

	Settings()->SetValue("RenderSystem.Shadow.Factor", 1.0f);
	Settings()->SetValue("RenderSystem.Shadow.SplitFactor0", exp(-4.0f));
	Settings()->SetValue("RenderSystem.Shadow.SplitFactor1", exp(-3.0f));
	Settings()->SetValue("RenderSystem.Shadow.SplitFactor2", exp(-2.0f));
	Settings()->SetValue("RenderSystem.Shadow.SplitFactor3", exp(-1.0f));
}
