#include "VKRenderer.h"


VkShaderStageFlagBits vkGetShaderStageFlagBits(shader_kind kind)
{
	switch ((int)kind) {
	case vertex_shader:
		return VK_SHADER_STAGE_VERTEX_BIT;

	case fragment_shader:
		return VK_SHADER_STAGE_FRAGMENT_BIT;

	case compute_shader:
		return VK_SHADER_STAGE_COMPUTE_BIT;

	default:
		return VK_SHADER_STAGE_ALL;
	}
}

VkResult vkBeginCommandBufferPrimary(VkCommandBuffer vkCommandBuffer, VkCommandBufferUsageFlags flags)
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.pNext = nullptr;
	beginInfo.flags = flags;
	beginInfo.pInheritanceInfo = nullptr;

	return vkBeginCommandBuffer(vkCommandBuffer, &beginInfo);
}

VkResult vkBeginCommandBufferSecondary(VkCommandBuffer vkCommandBuffer, VkCommandBufferUsageFlags flags, VkFramebuffer vkFrameBuffer, VkRenderPass vkRenderPass, uint32_t indexSubpass, VkBool32 occlusionQueryEnable, VkQueryControlFlags queryFlags, VkQueryPipelineStatisticFlags pipelineStatistics)
{
	VkCommandBufferInheritanceInfo inheritanceInfo = {};
	inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
	inheritanceInfo.pNext = nullptr;
	inheritanceInfo.renderPass = vkRenderPass;
	inheritanceInfo.subpass = indexSubpass;
	inheritanceInfo.framebuffer = vkFrameBuffer;
	inheritanceInfo.occlusionQueryEnable = occlusionQueryEnable;
	inheritanceInfo.queryFlags = queryFlags;
	inheritanceInfo.pipelineStatistics = pipelineStatistics;

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.pNext = nullptr;
	beginInfo.flags = flags;
	beginInfo.pInheritanceInfo = &inheritanceInfo;

	return vkBeginCommandBuffer(vkCommandBuffer, &beginInfo);
}

VkResult vkCmdBindVertexBuffer(VkCommandBuffer vkCommandBuffer, uint32_t binding, VkBuffer vkBuffer, VkDeviceSize offset)
{
	vkCmdBindVertexBuffers(vkCommandBuffer, binding, 1, &vkBuffer, &offset);
	return VK_SUCCESS;
}

VkResult vkCmdBufferMemoryBarrier(VkCommandBuffer vkCommandBuffer, VkBuffer vkBuffer, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkPipelineStageFlags srcPipelineStageFlags, VkPipelineStageFlags dstPipelineStageFlags, VkDeviceSize offset, VkDeviceSize size)
{
	VkBufferMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
	barrier.pNext = nullptr;
	barrier.srcAccessMask = srcAccessFlags;
	barrier.dstAccessMask = dstAccessFlags;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.buffer = vkBuffer;
	barrier.offset = offset;
	barrier.size = size;

	vkCmdPipelineBarrier(vkCommandBuffer, srcPipelineStageFlags, dstPipelineStageFlags, 0, 0, nullptr, 1, &barrier, 0, nullptr);
	return VK_SUCCESS;
}

VkResult vkCmdBufferMemoryBarrier(VkCommandBuffer vkCommandBuffer, VkBuffer vkBuffer, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkDeviceSize offset, VkDeviceSize size)
{
	VkPipelineStageFlags srcPipelineStageFlags = CVKHelper::GetPipelineStageFlagsByAccessFlags(srcAccessFlags);
	VkPipelineStageFlags dstPipelineStageFlags = CVKHelper::GetPipelineStageFlagsByAccessFlags(dstAccessFlags);

	vkCmdBufferMemoryBarrier(vkCommandBuffer, vkBuffer, srcAccessFlags, dstAccessFlags, srcPipelineStageFlags, dstPipelineStageFlags, offset, size);
	return VK_SUCCESS;
}

VkResult vkCmdImageMemoryBarrier(VkCommandBuffer vkCommandBuffer, VkImage vkImage, VkImageLayout srcLayout, VkImageLayout dstLayout, VkAccessFlags srcAccessFlags, VkAccessFlags dstAccessFlags, VkPipelineStageFlags srcPipelineStageFlags, VkPipelineStageFlags dstPipelineStageFlags, VkImageSubresourceRange range)
{
	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.pNext = nullptr;
	barrier.srcAccessMask = srcAccessFlags;
	barrier.dstAccessMask = dstAccessFlags;
	barrier.oldLayout = srcLayout;
	barrier.newLayout = dstLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = vkImage;
	barrier.subresourceRange = range;

	vkCmdPipelineBarrier(vkCommandBuffer, srcPipelineStageFlags, dstPipelineStageFlags, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	return VK_SUCCESS;
}

VkResult vkCmdImageMemoryBarrier(VkCommandBuffer vkCommandBuffer, VkImage vkImage, VkImageLayout srcLayout, VkImageLayout dstLayout, VkImageSubresourceRange range)
{
	VkAccessFlags srcAccessFlags = CVKHelper::GetAccessMaskByImageLayout(srcLayout);
	VkAccessFlags dstAccessFlags = CVKHelper::GetAccessMaskByImageLayout(dstLayout);
	VkPipelineStageFlags srcPipelineStageFlags = CVKHelper::GetPipelineStageFlagsByImageLayout(srcLayout);
	VkPipelineStageFlags dstPipelineStageFlags = CVKHelper::GetPipelineStageFlagsByImageLayout(dstLayout);

	if (srcLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
		srcPipelineStageFlags |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dstPipelineStageFlags |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	}
	else if (dstLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
		srcPipelineStageFlags |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dstPipelineStageFlags |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	}

	vkCmdImageMemoryBarrier(vkCommandBuffer, vkImage, srcLayout, dstLayout, srcAccessFlags, dstAccessFlags, srcPipelineStageFlags, dstPipelineStageFlags, range);
	return VK_SUCCESS;
}

VkResult vkCmdBufferData(VkCommandBuffer vkCommandBuffer, VkBuffer vkSrcBuffer, VkDeviceSize srcOffset, VkBuffer vkDstBuffer, VkDeviceSize dstOffset, VkDeviceSize size)
{
	VkBufferCopy region = {};
	region.srcOffset = srcOffset;
	region.dstOffset = dstOffset;
	region.size = size;

	vkCmdCopyBuffer(vkCommandBuffer, vkSrcBuffer, vkDstBuffer, 1, &region);
	return VK_SUCCESS;
}

VkResult vkCmdTextureData(VkCommandBuffer vkCommandBuffer, VkBuffer vkSrcBuffer, VkDeviceSize bufferOffset, VkImage vkDstImage, VkImageAspectFlags vkAspectMask, int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height, uint32_t level, uint32_t layer)
{
	VkBufferImageCopy region = {};
	region.bufferOffset = bufferOffset;
	region.imageOffset.x = xoffset;
	region.imageOffset.y = yoffset;
	region.imageOffset.z = 0;
	region.imageExtent.width = width;
	region.imageExtent.height = height;
	region.imageExtent.depth = 1;
	region.imageSubresource.aspectMask = vkAspectMask;
	region.imageSubresource.mipLevel = level;
	region.imageSubresource.baseArrayLayer = layer;
	region.imageSubresource.layerCount = 1;

	vkCmdCopyBufferToImage(vkCommandBuffer, vkSrcBuffer, vkDstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
	return VK_SUCCESS;
}

VkResult vkSubmitCommandBuffer(VkQueue vkQueue, VkCommandBuffer vkCommandBuffer, VkFence vkFence)
{
	return vkSubmitCommandBuffer(vkQueue, vkCommandBuffer, vkFence, VK_NULL_HANDLE, 0, VK_NULL_HANDLE);
}

VkResult vkSubmitCommandBuffer(VkQueue vkQueue, VkCommandBuffer vkCommandBuffer, VkFence vkFence, VkSemaphore vkWaitSemaphore, VkPipelineStageFlags waitStageFlags, VkSemaphore vkSignalSemaphore)
{
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.pNext = nullptr;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &vkCommandBuffer;
	submitInfo.waitSemaphoreCount = vkWaitSemaphore != VK_NULL_HANDLE ? 1 : 0;
	submitInfo.pWaitSemaphores = vkWaitSemaphore != VK_NULL_HANDLE ? &vkWaitSemaphore : nullptr;
	submitInfo.pWaitDstStageMask = vkWaitSemaphore != VK_NULL_HANDLE ? &waitStageFlags : nullptr;
	submitInfo.signalSemaphoreCount = vkSignalSemaphore != VK_NULL_HANDLE ? 1 : 0;
	submitInfo.pSignalSemaphores = vkSignalSemaphore != VK_NULL_HANDLE ? &vkSignalSemaphore : nullptr;

	return vkQueueSubmit(vkQueue, 1, &submitInfo, vkFence);
}
