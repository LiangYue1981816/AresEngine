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

void vkCmdSetImageLayout(VkCommandBuffer vkCommandBuffer, VkImage vkImage, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageSubresourceRange range)
{
	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.pNext = nullptr;
	barrier.srcAccessMask = CVKHelper::GetAccessMask(oldLayout);
	barrier.dstAccessMask = CVKHelper::GetAccessMask(newLayout);
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = vkImage;
	barrier.subresourceRange = range;

	VkPipelineStageFlags srcStageMask = CVKHelper::GetPipelineStageFlags(barrier.srcAccessMask);
	VkPipelineStageFlags dstStageMask = CVKHelper::GetPipelineStageFlags(barrier.dstAccessMask);

	if (oldLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
		srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	}
	else if (newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
		srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	}

	vkCmdPipelineBarrier(vkCommandBuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}

void vkCmdTransferImage(VkCommandBuffer vkCommandBuffer, VkImage vkSrcImage, VkImage vkDstImage, VkImageCopy region)
{
	VkImageSubresourceRange srcRange = {};
	srcRange.aspectMask = region.srcSubresource.aspectMask;
	srcRange.baseMipLevel = region.srcSubresource.mipLevel;
	srcRange.levelCount = 1;
	srcRange.baseArrayLayer = region.srcSubresource.baseArrayLayer;
	srcRange.layerCount = region.srcSubresource.layerCount;

	VkImageSubresourceRange dstRange = {};
	dstRange.aspectMask = region.dstSubresource.aspectMask;
	dstRange.baseMipLevel = region.dstSubresource.mipLevel;
	dstRange.levelCount = 1;
	dstRange.baseArrayLayer = region.dstSubresource.baseArrayLayer;
	dstRange.layerCount = region.dstSubresource.layerCount;

	vkCmdSetImageLayout(vkCommandBuffer, vkSrcImage, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, srcRange);
	vkCmdSetImageLayout(vkCommandBuffer, vkDstImage, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, dstRange);
	vkCmdCopyImage(vkCommandBuffer, vkSrcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, vkDstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}

void vkCmdTransferImage(VkCommandBuffer vkCommandBuffer, VkBuffer vkSrcBuffer, VkImage vkDstImage, uint32_t levels, uint32_t layers, const VkBufferImageCopy* regions, uint32_t count)
{
	VkImageSubresourceRange dstRange;
	dstRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	dstRange.baseMipLevel = 0;
	dstRange.levelCount = levels;
	dstRange.baseArrayLayer = 0;
	dstRange.layerCount = layers;

	vkCmdSetImageLayout(vkCommandBuffer, vkDstImage, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, dstRange);
	vkCmdCopyBufferToImage(vkCommandBuffer, vkSrcBuffer, vkDstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, count, regions);
}

void vkCmdTransferBuffer(VkCommandBuffer vkCommandBuffer, VkBuffer vkSrcBuffer, VkBuffer vkDstBuffer, VkAccessFlags dstAccessMask, VkPipelineStageFlags dstStageMask, VkDeviceSize srcOffset, VkDeviceSize dstOffset, VkDeviceSize size)
{
	VkBufferCopy region;
	region.srcOffset = srcOffset;
	region.dstOffset = dstOffset;
	region.size = size;
	vkCmdCopyBuffer(vkCommandBuffer, vkSrcBuffer, vkDstBuffer, 1, &region);

	VkBufferMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
	barrier.pNext = nullptr;
	barrier.srcAccessMask = VK_ACCESS_MEMORY_WRITE_BIT;
	barrier.dstAccessMask = dstAccessMask;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.buffer = vkDstBuffer;
	barrier.offset = dstOffset;
	barrier.size = size;
	vkCmdPipelineBarrier(vkCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, dstStageMask, 0, 0, nullptr, 1, &barrier, 0, nullptr);
}

void vkCmdBindVertexBuffer(VkCommandBuffer vkCommandBuffer, uint32_t firstBinding, VkBuffer vkBuffer, VkDeviceSize offset)
{
	vkCmdBindVertexBuffers(vkCommandBuffer, firstBinding, 1, &vkBuffer, &offset);
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
