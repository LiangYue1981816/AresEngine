#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_vulkan.h"

#include "Application.h"
#include "Framework.h"
#include "VKRenderer.h"


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

static void ImGui_ImplVulkan_DestritWindow(VkAllocationCallbacks* allocator, VkRenderPass* pRenderPass, VkImageView* imageViews[3], VkFramebuffer* framebuffers[3])
{
	for (uint32_t i = 0; i < 3; i++) {
		vkDestroyFramebuffer(VKRenderer()->GetDevice(), *framebuffers[i], allocator);
		framebuffers[i] = VK_NULL_HANDLE;
	}

	for (uint32_t i = 0; i < 3; i++) {
		vkDestroyImageView(VKRenderer()->GetDevice(), *imageViews[i], allocator);
		imageViews[i] = VK_NULL_HANDLE;
	}

	vkDestroyRenderPass(VKRenderer()->GetDevice(), *pRenderPass, allocator);
	*pRenderPass = VK_NULL_HANDLE;
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
	ptrImGuiCommandBuffers[0].Release();
	ptrImGuiCommandBuffers[1].Release();
	ptrImGuiCommandBuffers[2].Release();

	DestroyFramework();
	DestroyEngine();
}

void CApplicationVulkan::UpdateInternal(float deltaTime)
{

}
