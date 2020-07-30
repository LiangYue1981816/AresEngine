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

static void ImGui_ImplVulkan_CreateDescriptorPool(VkAllocationCallbacks* allocator, VkDescriptorPool* descriptor_pool)
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
	check_vk_result(vkCreateDescriptorPool(VKRenderer()->GetDevice(), &info, allocator, descriptor_pool));
}

static void ImGui_ImplVulkan_DestroyDescriptorPool(VkAllocationCallbacks* allocator, VkDescriptorPool* descriptor_pool)
{
	vkDestroyDescriptorPool(VKRenderer()->GetDevice(), *descriptor_pool, allocator);
	*descriptor_pool = VK_NULL_HANDLE;
}

static void ImGui_ImplVulkan_CreateWindow(uint32_t width, uint32_t height, VkFormat format, VkAllocationCallbacks* allocator, VkRenderPass* render_pass, VkImageView* image_views, VkFramebuffer* frame_buffers)
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
		check_vk_result(vkCreateRenderPass(VKRenderer()->GetDevice(), &info, allocator, render_pass));
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
			check_vk_result(vkCreateImageView(VKRenderer()->GetDevice(), &info, allocator, &image_views[i]));
		}
	}
	{
		VkImageView attachment[1];
		VkFramebufferCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		info.renderPass = *render_pass;
		info.attachmentCount = 1;
		info.pAttachments = attachment;
		info.width = width;
		info.height = height;
		info.layers = 1;
		for (uint32_t i = 0; i < 3; i++)
		{
			attachment[0] = image_views[i];
			check_vk_result(vkCreateFramebuffer(VKRenderer()->GetDevice(), &info, allocator, &frame_buffers[i]));
		}
	}
}

static void ImGui_ImplVulkan_DestroyWindow(VkAllocationCallbacks* allocator, VkRenderPass* render_pass, VkImageView* image_views, VkFramebuffer* frame_buffers)
{
	for (uint32_t i = 0; i < 3; i++) {
		vkDestroyFramebuffer(VKRenderer()->GetDevice(), frame_buffers[i], allocator);
		frame_buffers[i] = VK_NULL_HANDLE;
	}

	for (uint32_t i = 0; i < 3; i++) {
		vkDestroyImageView(VKRenderer()->GetDevice(), image_views[i], allocator);
		image_views[i] = VK_NULL_HANDLE;
	}

	vkDestroyRenderPass(VKRenderer()->GetDevice(), *render_pass, allocator);
	*render_pass = VK_NULL_HANDLE;
}

static void ImGui_ImplVulkan_FrameRender(VkCommandBuffer command_buffer, VkFence fence, VkSemaphore wait_semaphore, VkSemaphore signal_semaphore, VkRenderPass render_pass, VkFramebuffer frame_buffer, int width, int height, ImDrawData* draw_data)
{
	{
		check_vk_result(vkWaitForFences(VKRenderer()->GetDevice(), 1, &fence, VK_TRUE, UINT64_MAX));
		check_vk_result(vkResetFences(VKRenderer()->GetDevice(), 1, &fence));
	}
	{
		check_vk_result(vkResetCommandBuffer(command_buffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT));

		VkCommandBufferBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		check_vk_result(vkBeginCommandBuffer(command_buffer, &info));
	}
	{
		VkClearValue clear_value;
		VkRenderPassBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.renderPass = render_pass;
		info.framebuffer = frame_buffer;
		info.renderArea.extent.width = width;
		info.renderArea.extent.height = height;
		info.clearValueCount = 1;
		info.pClearValues = &clear_value;
		vkCmdBeginRenderPass(command_buffer, &info, VK_SUBPASS_CONTENTS_INLINE);
	}
	ImGui_ImplVulkan_RenderDrawData(draw_data, command_buffer);
	{
		vkCmdEndRenderPass(command_buffer);

		VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkSubmitInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &wait_semaphore;
		info.pWaitDstStageMask = &wait_stage;
		info.commandBufferCount = 1;
		info.pCommandBuffers = &command_buffer;
		info.signalSemaphoreCount = 1;
		info.pSignalSemaphores = &signal_semaphore;
		check_vk_result(vkEndCommandBuffer(command_buffer));
		check_vk_result(vkQueueSubmit(VKRenderer()->GetQueue(), 1, &info, fence));
	}
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

	//
	// 5. Upload Fonts
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

	// Upload Fonts
	{
		VkCommandBuffer command_buffer = ((CVKCommandBuffer*)ptrImGuiCommandBuffers[0].GetPointer())->GetCommandBuffer();
		check_vk_result(vkResetCommandBuffer(command_buffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT));

		VkCommandBufferBeginInfo begin_info = {};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		check_vk_result(vkBeginCommandBuffer(command_buffer, &begin_info));

		ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

		VkSubmitInfo end_info = {};
		end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		end_info.commandBufferCount = 1;
		end_info.pCommandBuffers = &command_buffer;
		check_vk_result(vkEndCommandBuffer(command_buffer));
		check_vk_result(vkQueueSubmit(VKRenderer()->GetQueue(), 1, &end_info, VK_NULL_HANDLE));

		check_vk_result(vkDeviceWaitIdle(VKRenderer()->GetDevice()));
		ImGui_ImplVulkan_DestroyFontUploadObjects();
	}

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
		ImGui_ImplVulkan_FrameRender(
			((CVKCommandBuffer*)ptrImGuiCommandBuffer.GetPointer())->GetCommandBuffer(), 
			((CVKCommandBuffer*)ptrImGuiCommandBuffer.GetPointer())->GetFence(), 
			((CVKSemaphore*)((CVKCommandBuffer*)ptrGraphicCommandBuffer.GetPointer())->GetSemaphore())->GetSemaphore(), 
			((CVKSemaphore*)((CVKCommandBuffer*)ptrImGuiCommandBuffer.GetPointer())->GetSemaphore())->GetSemaphore(),
			vkRenderPass, vkFramebuffers[GfxRenderer()->GetSwapChain()->GetFrameIndex()], 
			m_width, m_height,
			ImGui::GetDrawData());
	}
	GfxRenderer()->Present(ptrImGuiCommandBuffer->GetSemaphore());
}