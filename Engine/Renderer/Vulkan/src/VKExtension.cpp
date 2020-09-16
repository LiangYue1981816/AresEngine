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
	CALL_VK_FUNCTION_RETURN_RESULT(vkBeginCommandBuffer(vkCommandBuffer, &beginInfo));
	return VK_SUCCESS;
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
	CALL_VK_FUNCTION_RETURN_RESULT(vkBeginCommandBuffer(vkCommandBuffer, &beginInfo));
	return VK_SUCCESS;
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
	CALL_VK_FUNCTION_RETURN_RESULT(vkCmdBufferMemoryBarrier(vkCommandBuffer, vkBuffer, srcAccessFlags, dstAccessFlags, srcPipelineStageFlags, dstPipelineStageFlags, offset, size));
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

	CALL_VK_FUNCTION_RETURN_RESULT(vkCmdImageMemoryBarrier(vkCommandBuffer, vkImage, srcLayout, dstLayout, srcAccessFlags, dstAccessFlags, srcPipelineStageFlags, dstPipelineStageFlags, range));
	return VK_SUCCESS;
}

VkResult vkCmdTransferBufferData(VkCommandBuffer vkCommandBuffer, VkBuffer vkSrcBuffer, VkBuffer vkDstBuffer, VkBufferUsageFlags vkDstBufferUsageFlags, VkDeviceSize offset, VkDeviceSize size)
{
	VkBufferCopy region = {};
	region.srcOffset = 0;
	region.dstOffset = offset;
	region.size = size;
	vkCmdCopyBuffer(vkCommandBuffer, vkSrcBuffer, vkDstBuffer, 1, &region);

	VkAccessFlags dstAccessFlags = CVKHelper::GetAccessMaskByBufferUsage(vkDstBufferUsageFlags);
	VkPipelineStageFlags dstPipelineStageFlags = CVKHelper::GetPipelineStageFlagsByBufferUsage(vkDstBufferUsageFlags);
	CALL_VK_FUNCTION_RETURN_RESULT(vkCmdBufferMemoryBarrier(vkCommandBuffer, vkDstBuffer, VK_ACCESS_TRANSFER_WRITE_BIT, dstAccessFlags, VK_PIPELINE_STAGE_TRANSFER_BIT, dstPipelineStageFlags, offset, size));
	return VK_SUCCESS;
}

VkResult vkCmdTextureData(VkCommandBuffer vkCommandBuffer, VkBuffer vkSrcBuffer, VkImage vkDstImage, VkImageLayout vkSrcImageLayout, VkImageLayout vkDstImageLayout, VkImageAspectFlags vkAspectMask, int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height, uint32_t level, uint32_t layer)
{
	VkBufferImageCopy region = {};
	region.bufferOffset = 0;
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

	VkImageSubresourceRange range = {};
	range.aspectMask = vkAspectMask;
	range.baseMipLevel = level;
	range.levelCount = 1;
	range.baseArrayLayer = layer;
	range.layerCount = 1;

	CALL_VK_FUNCTION_RETURN_RESULT(vkCmdImageMemoryBarrier(vkCommandBuffer, vkDstImage, vkSrcImageLayout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, range));
	vkCmdCopyBufferToImage(vkCommandBuffer, vkSrcBuffer, vkDstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
	CALL_VK_FUNCTION_RETURN_RESULT(vkCmdImageMemoryBarrier(vkCommandBuffer, vkDstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, vkDstImageLayout, range));

	return VK_SUCCESS;
}

VkResult vkSubmitCommandBuffer(VkQueue vkQueue, VkCommandBuffer vkCommandBuffer, VkFence vkFence)
{
	CALL_VK_FUNCTION_RETURN_RESULT(vkSubmitCommandBuffer(vkQueue, vkCommandBuffer, vkFence, VK_NULL_HANDLE, 0, VK_NULL_HANDLE));
	return VK_SUCCESS;
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
	CALL_VK_FUNCTION_RETURN_RESULT(vkQueueSubmit(vkQueue, 1, &submitInfo, vkFence));
	return VK_SUCCESS;
}
