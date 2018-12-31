#pragma once
#include "VKRenderer.h"


extern VkResult vkBeginCommandBufferPrimary(VkCommandBuffer vkCommandBuffer, VkCommandBufferUsageFlags flags);
extern VkResult vkBeginCommandBufferSecondary(VkCommandBuffer vkCommandBuffer, VkCommandBufferUsageFlags flags, VkFramebuffer vkFrameBuffer, VkRenderPass vkRenderPass, uint32_t indexSubPass, VkBool32 occlusionQueryEnable, VkQueryControlFlags queryFlags, VkQueryPipelineStatisticFlags pipelineStatistics);

extern void vkCmdSetImageLayout(VkCommandBuffer vkCommandBuffer, VkImage vkImage, VkImageLayout oldLayout, VkImageLayout newLayout, const VkImageSubresourceRange *range);

extern void vkCmdTransferImage(VkCommandBuffer vkCommandBuffer, VkImage vkSrcImage, VkImage vkDstImage, const VkImageCopy *region);
extern void vkCmdTransferBuffer(VkCommandBuffer vkCommandBuffer, VkBuffer vkSrcBuffer, VkBuffer vkDstBuffer, VkAccessFlags dstAccessMask, VkPipelineStageFlags dstStageMask, VkDeviceSize srcOffset, VkDeviceSize dstOffset, VkDeviceSize size);

extern VkResult vkSubmitCommandBuffer(VkQueue vkQueue, VkCommandBuffer vkCommandBuffer, VkFence vkFence);
extern VkResult vkSubmitCommandBuffer(VkQueue vkQueue, VkCommandBuffer vkCommandBuffer, VkFence vkFence, VkSemaphore vkWaitSemaphore, VkPipelineStageFlags waitStageFlags, VkSemaphore vkSignalSemaphore);
